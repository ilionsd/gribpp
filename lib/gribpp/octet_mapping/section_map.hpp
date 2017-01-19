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

		enum class grib_edition : std::uint8_t {
			V1 = 1,
			V2 = 2
		};

		namespace _stdex = std::experimental;

		class section_map {
		public:
			section_map() :
				mMapping()
			{};

			section_map(const section_map& other) :
				mMapping(other.mMapping)
			{};

			section_map(section_map &&other) :
				mMapping(std::move(other.mMapping))
			{};
			section_map(std::initializer_list<typename map_type::value_type> initList) :
				mMapping(initList)
			{};

			inline section_map& operator= (const section_map& other) {
				mMapping = other.mMapping;
				return (*this);
			};

			inline section_map& operator= (section_map&& other) {
				mMapping = std::move(other.mMapping);
				return *this;
			}

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
		struct section_map_vn :
			public section_map
		{
			section_map_vn() :
				section_map()
			{};

			section_map_vn(const section_map_vn<V, N> &other) :
				section_map(other)
			{};

			section_map_vn(section_map_vn<V, N> &&other) :
				section_map(other)
			{};

			section_map_vn(std::initializer_list<typename map_type::value_type> initList) :
				section_map(initList)
			{};

			inline section_map_vn<V, N>& operator= (const section_map_vn<V, N> &other) {
				section_map::operator =(other);
				return *this;
			}

			inline section_map_vn<V, N> &operator= (section_map_vn<V, N> &&other) {
				section_map::operator =(other);
				return *this;
			}

			inline section_map_vn<V, N>& operator=(std::initializer_list<typename map_type::value_type> initList) {
				section_map::operator =(initList);
				return *this;
			}


			inline unsigned number() const {
				return N;
			};

			inline grib_edition version() const {
				return V;
			};

		};




		template<grib_edition V, unsigned N>
		auto make_section_map(reader::octet_reader &r) -> _stdex::optional<section_map_vn<V, N>> {
			return {};
		};

		template<>
		auto make_section_map(reader::octet_reader &reader) -> _stdex::optional<section_map_vn<grib_edition::V2, 0>> {
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

			section_map_vn<grib_edition::V2, 0> sectionMap {
				{ fields::GRIB_MESSAGE,{ 0, 3 } },
				{ fields::RESERVED01,{ 4, 5 } },
				{ fields::MASTER_TABLE_NUMBER,{ 6, 6 } },
				{ fields::EDITION_NUMBER,{ 7, 7 } },
				{ fields::TOTAL_LENGTH,{ 8, 15 } }
			};

			sectionMap.shift_mapping(pos);

			reader.set_pos(*sectionMap.last_octet() + 1);

			return sectionMap;
		};

		template<>
		auto make_section_map(reader::octet_reader &reader) -> _stdex::optional<section_map_vn<grib_edition::V2, 1>> {
			constexpr std::size_t CONTENT_MAX_SIZE = 4;

			std::size_t sectonPos = reader.get_pos();
			std::unique_ptr<char[]> octets = std::make_unique<char[]>(CONTENT_MAX_SIZE);

			//-- Length of section (4) --
			reader(4) >> octets;
			std::size_t sectionSize = octets[0] | octets[1] << 8 | octets[2] << 16 | octets[3] << 24;

			//-- Section number (1) --
			char sectionNumber;
			reader >> sectionNumber;
			if (sectionNumber != 1) {
				//-- not 1st section --
				return {};
			}

			section_map_vn<grib_edition::V2, 1> sectionMap {
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

			reader.set_pos(*sectionMap.last_octet() + 1);

			return sectionMap;
		};


	};	//-- namespace octet_mapping --
};	//-- namespace gribpp --

#endif	//-- _GRIBPP_OCTATE_MAPPING_SECTION_HPP_ --
