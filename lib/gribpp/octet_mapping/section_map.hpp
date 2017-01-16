#ifndef _GRIBPP_OCTET_MAPPING_SECTION_HPP_
#define _GRIBPP_OCTET_MAPPING_SECTION_HPP_

#define _HAS_CXX17 1

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory> 
#include <unordered_map>
#include <initializer_list>
#include <experimental/optional>
#include <limits>


#include "../../utility/enum_hash.hpp"
#include "octet_fields.hpp"
#include "../reader/octet_reader.hpp"

namespace gribpp {
	namespace octet_mapping {

		using map_type = std::unordered_map<fields, std::pair<std::size_t, std::size_t>, utility::enum_hash<fields>>;

		enum class grib_edition {
			V1 = 1,
			V2 = 2
		};

		namespace _stdex = std::experimental;

		class section_map {
		public:
			section_map() noexcept :
				mMapping()
			{};

			section_map(const grib_edition edition) noexcept :
				mMapping()
			{};

			section_map(const section_map& other) noexcept :
				mMapping(other.mMapping)
			{};

			section_map(section_map &&other) noexcept :
				mMapping(std::move(other.mMapping))
			{};
			section_map(std::initializer_list<typename map_type::value_type> initList) noexcept :
				mMapping(initList)
			{};

			inline section_map& operator= (const section_map& other) {
				mMapping = other.mMapping;
				return (*this);
			};

			inline section_map& operator= (std::initializer_list<typename map_type::value_type> initList) {
				mMapping = initList;
				return *this;
			};

			inline map_type::mapped_type& operator[] (map_type::key_type&& keyVal) {
				return mMapping[keyVal];
			};
			inline map_type::mapped_type& operator[] (const map_type::key_type& keyVal) {
				return mMapping[keyVal];
			};
			inline map_type::mapped_type& at(const map_type::key_type& keyVal) {
				return mMapping.at(keyVal);
			};
			inline const map_type::mapped_type& at(const map_type::key_type& keyVal) const {
				return mMapping.at(keyVal);
			};

			inline const map_type& mapping() const {
				return mMapping;
			};

			inline bool empty() const {
				return mMapping.empty();
			};

			void shift_mapping(const std::size_t pos) {
				for (auto val : mapping()) {
					val.second.first += pos;
					val.second.second += pos;
				}
			};

			_stdex::optional<std::size_t> first_octet() const {
				if (empty())
					return {};
				else {
					std::size_t firstOctet = std::numeric_limits<std::size_t>::max();
					for (auto it = mapping().cbegin(); it != mapping().cend(); ++it) {
						if (firstOctet > it->second.first)
							firstOctet = it->second.first;
					}
					return firstOctet;
				}
			};

			_stdex::optional<std::size_t> last_octet() const {
				if (empty())
					return {};
				else {
					std::size_t lastOctet = std::numeric_limits<std::size_t>::min();
					for (auto it = mapping().cbegin(); it != mapping().cend(); ++it) {
						if (lastOctet < it->second.second)
							lastOctet = it->second.second;
					}
					return lastOctet;
				}
			};

		protected:
			map_type mMapping;

		};


		template<grib_edition V, unsigned N>
		class section_map_ne :
			public section_map
		{
		public:
			using section_map::section_map;

			constexpr unsigned section_number() const {
				return N;
			}

			constexpr grib_edition edition() const {
				return V;
			}
		};



		template<grib_edition Version, unsigned Number>
		_stdex::optional<section_map_ne<Version, Number>>
			make_section_map(reader::octet_reader& reader)
		{
			static_assert(false, "Not implemented for grib version or section number");
			return {};
		};

		//-- GRIB V1 --

		template<>
		_stdex::optional<section_map_ne<grib_edition::V1, 0>>
			make_section_map(reader::octet_reader& reader)
		{
			static_assert(false, "Not yet implemented");
			return {};
		};

		template<>
		_stdex::optional<section_map_ne<grib_edition::V1, 1>>
			make_section_map(reader::octet_reader& reader)
		{
			static_assert(false, "Not yet implemented");
			return {};
		}



		//-- GRIB V2 --

		template<>
		_stdex::optional<section_map_ne<grib_edition::V2, 0>>
			make_section_map(reader::octet_reader& reader)
		{
			//-- Section<0> constants definition --
			constexpr std::size_t GRIB_MESSAGE_SIZE = 4;
			constexpr const char* GRIB_MESSAGE = "GRIB";

			//--  --
			std::unique_ptr<char[]> octets = std::make_unique<char[]>(GRIB_MESSAGE_SIZE);
			std::size_t pos = reader.get_pos();

			reader(GRIB_MESSAGE_SIZE) >> octets;
			int cmp = strncmp(GRIB_MESSAGE, octets.get(), GRIB_MESSAGE_SIZE);
			if (cmp) {
				//-- not a grib file --
				return {};
			}

			section_map_ne<grib_edition::V2, 0> sectionMap = {
				{ fields::GRIB_MESSAGE,{ 0, 3 } },
				{ fields::RESERVED01,{ 4, 5 } },
				{ fields::MASTER_TABLE_NUMBER,{ 6, 6 } },
				{ fields::EDITION_NUMBER,{ 7, 7 } },
				{ fields::TOTAL_LENGTH,{ 8, 15 } }
			};

			sectionMap.shift_mapping(pos);
			return sectionMap;
		};

		template<>
		_stdex::optional<section_map_ne<grib_edition::V2, 1>>
			make_section_map<grib_edition::V2, 1>(reader::octet_reader& reader)
		{
			constexpr std::size_t CONTENT_MAX_SIZE = 4;

			std::size_t sectonPos = reader.get_pos();
			std::unique_ptr<char[]> octets = std::make_unique<char[]>(CONTENT_MAX_SIZE);

			//-- Length of section (4) --
			reader(4) >> octets;
			std::size_t _0 = octets[0];
			std::size_t _1 = octets[1] << 8;
			std::size_t _2 = octets[2] << 16;
			std::size_t _3 = octets[3] << 24;
			std::size_t sectionSize = octets[0] | octets[1] << 8 | octets[2] << 16 | octets[3] << 24;

			//-- Section number (1) --
			char sectionNumber;
			reader >> sectionNumber;
			if (sectionNumber != 1) {
				//-- not 1st section --
				return {};
			}

			section_map_ne<grib_edition::V2, 1> sectionMap = {
				{ fields::SECTION_LENGTH,{ 0, 3 } },
				{ fields::SECTION_NUMBER,{ 4, 4 } },
				{ fields::ORIGINATING_CENTER_ID,{ 5, 6 } },
				{ fields::ORIGINATING_SUBCENTER_ID,{ 7, 8 } },
				{ fields::MASTER_TABLES_VERSION_NUMBER,{ 9, 9 } },
				{ fields::LOCAL_TABLES_VERSION_NUMBER,{ 10, 10 } },
				{ fields::SIGNIFICANCE_OF_REFERENCE_TIME,{ 11, 11 } },
				{ fields::YEAR,{ 12, 13 } },
				{ fields::MONTH,{ 14, 14 } },
				{ fields::DAY,{ 15, 15 } },
				{ fields::HOUR,{ 16, 16 } },
				{ fields::MINUTE,{ 17, 17 } },
				{ fields::SECOND,{ 18, 18 } },
				{ fields::PRODUCTION_STATUS_OF_DATA,{ 19, 19 } },
				{ fields::TYPE_OF_DATA,{ 20, 20 } },
				{ fields::RESERVED02,{ 21, sectionSize - 21 } }
			};
			sectionMap.shift_mapping(sectonPos);
			return sectionMap;
		};



		
	};	//-- namespace octet_mapping --
};	//-- namespace gribpp --

#endif	//-- _GRIBPP_OCTATE_MAPPING_SECTION_HPP_ --
