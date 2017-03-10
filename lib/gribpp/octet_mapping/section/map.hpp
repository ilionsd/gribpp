#ifndef _GRIBPP_OCTET_MAPPING_SECTION_MAP_HPP_
#define _GRIBPP_OCTET_MAPPING_SECTION_MAP_HPP_

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory> 
#include <unordered_map>
#include <tuple>
#include <initializer_list>
#include <experimental/optional>
#include <limits>


#include "../../reader/octet_reader.hpp"
#include "../grib_edition.hpp"
#include "../octet_map.hpp"
#include "../grid_definition_template/number.hpp"
#include "../grid_definition_template/map.hpp"


namespace gribpp {
	namespace octet_mapping {
		namespace section {

			namespace stdex = std::experimental;

			using std::uint8_t;
			using std::uint16_t;
			using std::uint32_t;

			using std::size_t;

			enum class octets : uint32_t {
				//-- Section 0 (Indicator Section) --
				GRIB_MESSAGE,
				RESERVED01,
				MASTER_TABLE_NUMBER,
				EDITION_NUMBER,
				TOTAL_LENGTH,

				//-- Section 1 - 6 --
				SECTION_LENGTH,
				SECTION_NUMBER,

				//-- Section 1 (Identification Section) --
				ORIGINATING_CENTER_ID,
				ORIGINATING_SUBCENTER_ID,
				MASTER_TABLES_VERSION_NUMBER,
				LOCAL_TABLES_VERSION_NUMBER,
				SIGNIFICANCE_OF_REFERENCE_TIME,
				YEAR,
				MONTH,
				DAY,
				HOUR,
				MINUTE,
				SECOND,
				PRODUCTION_STATUS_OF_DATA,
				TYPE_OF_DATA,
				RESERVED02,

				//-- Section 2 (Local Use Section) --
				LOCAL_USE,

				//-- Section 3 (Grid Definition Section) --
				SOURCE_OF_GRID_DEFINITION,
				NUMBER_OF_DATA_POINTS,
				LENGTH_FOR_OPTIONAL_LIST_OF_NUMBER_OF_POINTS,
				INTERPRETATION_FOR_LIST_OF_NUMBER_OF_POINTS,
				GRID_DEFINITION_TEMPLATE_NUMBER,
				GRID_DEFINITION_TEMPLATE,
				OPTIONAL_LIST_OF_NUMBERS_DEFINING_NUMBER_OF_POINTS,

				//-- Section 4 (Product Definition Section) --
				NUMBER_OF_COORDINATES_VALUES,
				PRODUCT_DEFINITION_TEMPLATE_NUMBER,
				PRODUCT_DEFINITION_TEMPLATE,
				OPTIONAL_LIST_OF_COORDINATES_VALUES,

				//-- Section 5 (Data Representation Section) --
				NUMBER_OF_DATA_POINTS_WITH_BIT_MAP_PRESENT,
				DATA_REPRESENTATION_TEMPLATE_NUMBER,
				DATA_REPRESENTATION_TEMPLATE,

				//-- Section 6 (Bit-Map Section) --
				BIT_MAP_INDICATOR,
				BIT_MAP,

				//-- Section 7 (Data Section) --
				DATA,

				//-- Section 8 (End Section) --
				END_MESSAGE
			};

			template<grib_edition V, unsigned N>
			struct map :
				public octet_map<octets>
			{
				static constexpr grib_edition grib_version = V;
				static constexpr unsigned section_number = N;

				using base_type 	= octet_map<octets>;
				using map_type 		= typename base_type::map_type;
				using value_type 	= typename map_type::value_type;
				using key_type 		= typename map_type::key_type;
				using mapped_type 	= typename map_type::mapped_type;

				inline map() :
					base_type()
				{};
				inline map(const map_type& m) :
					base_type(m)
				{};
				inline map(const map& other) :
					base_type(other)
				{};
				inline map(map&& other) :
					base_type(other)
				{};
				inline map(std::initializer_list<value_type> initList) :
					base_type(initList)
				{};

				inline map& operator= (const map& other) {
					base_type::operator =(other);
					return (*this);
				};
				inline map& operator= (map&& other) {
					base_type::operator =(other);
					return (*this);
				};
				inline map& operator= (std::initializer_list<value_type> initList) {
					base_type::operator =(initList);
					return (*this);
				};

				inline unsigned number() const {
					return section_number;
				};

				inline grib_edition version() const {
					return grib_version;
				};

			};


			template<grib_edition V, unsigned N>
			auto make_map(reader::octet_reader &r) -> stdex::optional<map<V, N>>;


			template<>
			auto make_map(reader::octet_reader &reader) -> stdex::optional<map<grib_edition::V2, 0>>
			{
				//-- Section<0> constants definition --
				constexpr std::size_t GRIB_MESSAGE_SIZE = 4;
				constexpr const char* GRIB_MESSAGE = "GRIB";

				//--  --
				std::unique_ptr<char[]> octets = std::make_unique<char[]>(GRIB_MESSAGE_SIZE);
				size_t pos = reader.get_pos();

				reader(GRIB_MESSAGE_SIZE) >> octets;
				int cmp = strncmp(GRIB_MESSAGE, octets.get(), GRIB_MESSAGE_SIZE);
				if (cmp) {
					//-- not a grib file --
					return {};
				}

				map<grib_edition::V2, 0> sectionMap {
					{ octets::GRIB_MESSAGE,{ 0, 3 } },
					{ octets::RESERVED01,{ 4, 5 } },
					{ octets::MASTER_TABLE_NUMBER,{ 6, 6 } },
					{ octets::EDITION_NUMBER,{ 7, 7 } },
					{ octets::TOTAL_LENGTH,{ 8, 15 } }
				};

				sectionMap.shift_mapping(pos);

				reader.set_pos(*sectionMap.last_octet() + 1);

				return sectionMap;
			};

			template<>
			auto make_map(reader::octet_reader &reader) -> stdex::optional<map<grib_edition::V2, 1>>
			{
				size_t pos = reader.get_pos();

				uint32_t sectionSize;
				uint8_t sectionNumber;
				std::tie(sectionSize, sectionNumber) = reader.read_all<uint32_t, uint8_t>();

				if (sectionNumber != 1) {
					//-- not 1st section --
					return {};
				}

				map<grib_edition::V2, 1> sectionMap {
					{ octets::SECTION_LENGTH,{ 0, 3 } },
					{ octets::SECTION_NUMBER,{ 4, 4 } },
					{ octets::ORIGINATING_CENTER_ID,{ 5, 6 } },
					{ octets::ORIGINATING_SUBCENTER_ID,{ 7, 8 } },
					{ octets::MASTER_TABLES_VERSION_NUMBER,{ 9, 9 } },
					{ octets::LOCAL_TABLES_VERSION_NUMBER,{ 10, 10 } },
					{ octets::SIGNIFICANCE_OF_REFERENCE_TIME,{ 11, 11 } },
					{ octets::YEAR,{ 12, 13 } },
					{ octets::MONTH,{ 14, 14 } },
					{ octets::DAY,{ 15, 15 } },
					{ octets::HOUR,{ 16, 16 } },
					{ octets::MINUTE,{ 17, 17 } },
					{ octets::SECOND,{ 18, 18 } },
					{ octets::PRODUCTION_STATUS_OF_DATA,{ 19, 19 } },
					{ octets::TYPE_OF_DATA,{ 20, 20 } },
				};

				constexpr uint32_t OPTIONAL_OCTETS_BEGINNING = 21;
				if (sectionSize - 1 > OPTIONAL_OCTETS_BEGINNING)
					sectionMap[octets::RESERVED02] = { OPTIONAL_OCTETS_BEGINNING, sectionSize - 1 };

				sectionMap.shift_mapping(pos);

				reader.set_pos(*sectionMap.last_octet() + 1);

				return sectionMap;
			};

			template<>
			auto make_map(reader::octet_reader& reader) -> stdex::optional<map<grib_edition::V2, 2>>
			{
				size_t pos = reader.get_pos();

				uint32_t sectionSize;
				uint8_t sectionNumber;
				std::tie(sectionSize, sectionNumber) = reader.read_all<uint32_t, uint8_t>();

				if (sectionNumber != 2) {
					return {};
				}

				map<grib_edition::V2, 2> sectionMap {
					{octets::SECTION_LENGTH, {0, 3} },
					{octets::SECTION_NUMBER, {4, 4} },
					{octets::LOCAL_USE, {5, sectionSize - 1} }
				};

				sectionMap.shift_mapping(pos);
				reader.set_pos(*sectionMap.last_octet() + 1);

				return sectionMap;
			};

			template<>
			auto make_map(reader::octet_reader& reader) -> stdex::optional<map<grib_edition::V2, 3>>
			{
				size_t pos = reader.get_pos();

				uint32_t sectionSize;
				uint8_t sectionNumber;
				std::tie(sectionSize, sectionNumber) = reader.read_all<uint32_t, uint8_t>();


				if (sectionNumber != 3) {
					//-- not 1st section --
					return {};
				}
				//-- Source of GRIB definition (1) --
				uint8_t sourceOfGribDefinition, optionalListLength, optionalListInterpretation;
				uint16_t gdtNumber;
				std::tie(
						sourceOfGribDefinition,
						std::ignore,
						optionalListLength,
						optionalListInterpretation,
						gdtNumber) = reader.read_all<uint8_t, uint32_t, uint8_t, uint8_t, uint16_t>();

				map<grib_edition::V2, 3> sectionMap {
					{octets::SECTION_LENGTH, {0, 3} },
					{octets::SECTION_NUMBER, {4, 4} },
					{octets::SOURCE_OF_GRID_DEFINITION, {5, 5} },
					{octets::NUMBER_OF_DATA_POINTS, {6, 9} },
					{octets::LENGTH_FOR_OPTIONAL_LIST_OF_NUMBER_OF_POINTS, {10, 10} },
					{octets::INTERPRETATION_FOR_LIST_OF_NUMBER_OF_POINTS, {11, 11} },
					{octets::GRID_DEFINITION_TEMPLATE_NUMBER, {12, 13} }
				};

				sectionMap.shift_mapping(pos);


				namespace gdt = grid_definition_template;

				//-- if octet 6 is not zero and template number is set to 0xFF, then grid definition template may not be supplied --
				if (sourceOfGribDefinition && gdtNumber != 0xFF) {
					//-- something wrong with bytes values --
					return {};
				}
				else if (!sourceOfGribDefinition && gdtNumber != 0xFF) {
					//-- building Grid Definition Template Map --
					auto gdtMap = gdt::make_map<grib_edition::V2>(gdt::to_number(gdtNumber), reader);
					size_t xx = *gdtMap.last_octet();
					sectionMap[octets::GRID_DEFINITION_TEMPLATE] = { *sectionMap.last_octet() + 1, xx };
				}


				//-- if octet 11 and octet 12 is set to 0 then optional list of numbers of points is not present --
				if ((bool)optionalListLength != (bool)optionalListInterpretation) {
					//-- something wrong with bytes values --
					return {};
				}
				else if (optionalListLength && optionalListInterpretation) {
					//-- mapping numbers defining numbers of points --
					size_t xx = *sectionMap.last_octet();
					sectionMap[octets::OPTIONAL_LIST_OF_NUMBERS_DEFINING_NUMBER_OF_POINTS] = { xx + 1, xx + optionalListLength };
				}


				reader.set_pos(*sectionMap.last_octet() + 1);

				return sectionMap;
			};

			template<>
			auto make_map(reader::octet_reader& reader) -> stdex::optional<map<grib_edition::V2, 4>> {
				return {};
			};


		};	//-- namespace section --
	};	//-- namespace octet_mapping --
};	//-- namespace gribpp --

#endif	//-- _GRIBPP_OCTATE_MAPPING_SECTION_MAP_HPP_ --
















