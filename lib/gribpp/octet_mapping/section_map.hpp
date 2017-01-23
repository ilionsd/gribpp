#ifndef _GRIBPP_OCTET_MAPPING_SECTION_HPP_
#define _GRIBPP_OCTET_MAPPING_SECTION_HPP_

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory> 
#include <unordered_map>
#include <tuple>
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

			inline map_type& mapping() {
				return mMapping;
			};

			inline const map_type& mapping() const {
				return mMapping;
			};

			inline bool empty() const {
				return mMapping.empty();
			};

			void shift_mapping(const std::size_t pos) {
				for (auto it = mapping().begin(); it != mapping().end(); ++it) {
					it->second.first += pos;
					it->second.second += pos;
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

		using std::uint8_t;
		using std::uint16_t;
		using std::uint32_t;

		using std::size_t;


		auto section_definition(reader::octet_reader& reader) -> std::tuple<uint32_t, uint8_t> {
			return std::make_tuple(reader.read<uint32_t>(), reader.read<uint8_t>());
		}


		template<grib_edition V, unsigned N>
		auto make_section_map(reader::octet_reader &r) -> _stdex::optional<section_map_vn<V, N>>;


		template<>
		auto make_section_map(reader::octet_reader &reader) -> _stdex::optional<section_map_vn<grib_edition::V2, 0>>
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
		auto make_section_map(reader::octet_reader &reader) -> _stdex::optional<section_map_vn<grib_edition::V2, 1>>
		{
			std::size_t pos = reader.get_pos();

			uint32_t sectionSize;
			uint8_t sectionNumber;
			std::tie(sectionSize, sectionNumber) = reader.read_all<uint32_t, uint8_t>();

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
			};

			constexpr uint32_t OPTIONAL_OCTETS_BEGINNING = 21;
			if (sectionSize - 1 > OPTIONAL_OCTETS_BEGINNING)
				sectionMap[fields::RESERVED02] = { OPTIONAL_OCTETS_BEGINNING, sectionSize - 1 };

			sectionMap.shift_mapping(pos);

			reader.set_pos(*sectionMap.last_octet() + 1);

			return sectionMap;
		};

		template<>
		auto make_section_map(reader::octet_reader& reader) -> _stdex::optional<section_map_vn<grib_edition::V2, 2>>
		{
			std::size_t pos = reader.get_pos();

			uint32_t sectionSize;
			uint8_t sectionNumber;
			std::tie(sectionSize, sectionNumber) = reader.read_all<uint32_t, uint8_t>();

			if (sectionNumber != 2) {
				return {};
			}

			section_map_vn<grib_edition::V2, 2> sectionMap {
				{fields::SECTION_LENGTH, {0, 3} },
				{fields::SECTION_NUMBER, {4, 4} },
				{fields::LOCAL_USE, {5, sectionSize - 1} }
			};

			sectionMap.shift_mapping(pos);
			reader.set_pos(*sectionMap.last_octet() + 1);

			return sectionMap;
		};

		template<>
		auto make_section_map(reader::octet_reader& reader) -> _stdex::optional<section_map_vn<grib_edition::V2, 3>>
		{
			std::size_t pos = reader.get_pos();

			uint32_t sectionSize;
			uint8_t sectionNumber;
			std::tie(sectionSize, sectionNumber) = reader.read_all<uint32_t, uint8_t>();


			if (sectionNumber != 3) {
				//-- not 1st section --
				return {};
			}
			//-- Source of GRIB definition (1) --
			uint8_t sourceOfGribDefinition, optionalListLength, optionalListInterpretation;
			uint16_t templateNumber;
			std::tie(
					sourceOfGribDefinition,
					std::ignore,
					optionalListLength,
					optionalListInterpretation,
					templateNumber) = reader.read_all<uint8_t, uint32_t, uint8_t, uint8_t, uint16_t>();

			section_map_vn<grib_edition::V2, 3> sectionMap {
				{fields::SECTION_LENGTH, {0, 3} },
				{fields::SECTION_NUMBER, {4, 4} },
				{fields::SOURCE_OF_GRID_DEFINITION, {5, 5} },
				{fields::NUMBER_OF_DATA_POINTS, {6, 9} },
				{fields::LENGTH_FOR_OPTIONAL_LIST_OF_NUMBER_OF_POINTS, {10, 10} },
				{fields::INTERPRETATION_FOR_LIST_OF_NUMBER_OF_POINTS, {11, 11} },
				{fields::GRID_DEFINITION_TEMPLATE_NUMBER, {12, 13} },

			};

			std::size_t xx;
			if (sourceOfGribDefinition && templateNumber == 0xFF) {
				//-- if octet 6 is not zero and template number is set to 0xFF, then GRIB definition template may not be supplied --

			}
			else {

			}






			sectionMap.shift_mapping(pos);
			reader.set_pos(*sectionMap.last_octet() + 1);

			return sectionMap;
		};





	};	//-- namespace octet_mapping --
};	//-- namespace gribpp --

#endif	//-- _GRIBPP_OCTATE_MAPPING_SECTION_HPP_ --
















