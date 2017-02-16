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


#include "../reader/octet_reader.hpp"
#include "grib_edition.hpp"
#include "grid_definition_template_map.hpp"
#include "octet_map.hpp"


namespace gribpp {
	namespace octet_mapping {

		namespace _stdex = std::experimental;

		using std::uint8_t;
		using std::uint16_t;
		using std::uint32_t;

		using std::size_t;

		enum class section_octets : uint32_t {
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
		struct section_map :
			public octet_map<section_octets>
		{
			static constexpr grib_edition grib_version = V;
			static constexpr unsigned section_number = N;

			using base_type 	= octet_map<section_octets>;
			using map_type 		= typename base_type::map_type;
			using value_type 	= typename map_type::value_type;
			using key_type 		= typename map_type::key_type;
			using mapped_type 	= typename map_type::mapped_type;

			inline section_map() :
				base_type()
			{};
			inline section_map(const map_type& m) :
				base_type(m)
			{};
			inline section_map(const section_map& other) :
				base_type(other)
			{};
			inline section_map(section_map&& other) :
				base_type(other)
			{};
			inline section_map(std::initializer_list<value_type> initList) :
				base_type(initList)
			{};

			inline section_map& operator= (const section_map& other) {
				base_type::operator =(other);
				return (*this);
			};
			inline section_map& operator= (section_map&& other) {
				base_type::operator =(other);
				return (*this);
			};
			inline section_map& operator= (std::initializer_list<value_type> initList) {
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
		auto make_section_map(reader::octet_reader &r) -> _stdex::optional<section_map<V, N>>;


		template<>
		auto make_section_map(reader::octet_reader &reader) -> _stdex::optional<section_map<grib_edition::V2, 0>>
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

			section_map<grib_edition::V2, 0> sectionMap {
				{ section_octets::GRIB_MESSAGE,{ 0, 3 } },
				{ section_octets::RESERVED01,{ 4, 5 } },
				{ section_octets::MASTER_TABLE_NUMBER,{ 6, 6 } },
				{ section_octets::EDITION_NUMBER,{ 7, 7 } },
				{ section_octets::TOTAL_LENGTH,{ 8, 15 } }
			};

			sectionMap.shift_mapping(pos);

			reader.set_pos(*sectionMap.last_octet() + 1);

			return sectionMap;
		};

		template<>
		auto make_section_map(reader::octet_reader &reader) -> _stdex::optional<section_map<grib_edition::V2, 1>>
		{
			std::size_t pos = reader.get_pos();

			uint32_t sectionSize;
			uint8_t sectionNumber;
			std::tie(sectionSize, sectionNumber) = reader.read_all<uint32_t, uint8_t>();

			if (sectionNumber != 1) {
				//-- not 1st section --
				return {};
			}

			section_map<grib_edition::V2, 1> sectionMap {
				{ section_octets::SECTION_LENGTH,{ 0, 3 } },
				{ section_octets::SECTION_NUMBER,{ 4, 4 } },
				{ section_octets::ORIGINATING_CENTER_ID,{ 5, 6 } },
				{ section_octets::ORIGINATING_SUBCENTER_ID,{ 7, 8 } },
				{ section_octets::MASTER_TABLES_VERSION_NUMBER,{ 9, 9 } },
				{ section_octets::LOCAL_TABLES_VERSION_NUMBER,{ 10, 10 } },
				{ section_octets::SIGNIFICANCE_OF_REFERENCE_TIME,{ 11, 11 } },
				{ section_octets::YEAR,{ 12, 13 } },
				{ section_octets::MONTH,{ 14, 14 } },
				{ section_octets::DAY,{ 15, 15 } },
				{ section_octets::HOUR,{ 16, 16 } },
				{ section_octets::MINUTE,{ 17, 17 } },
				{ section_octets::SECOND,{ 18, 18 } },
				{ section_octets::PRODUCTION_STATUS_OF_DATA,{ 19, 19 } },
				{ section_octets::TYPE_OF_DATA,{ 20, 20 } },
			};

			constexpr uint32_t OPTIONAL_OCTETS_BEGINNING = 21;
			if (sectionSize - 1 > OPTIONAL_OCTETS_BEGINNING)
				sectionMap[section_octets::RESERVED02] = { OPTIONAL_OCTETS_BEGINNING, sectionSize - 1 };

			sectionMap.shift_mapping(pos);

			reader.set_pos(*sectionMap.last_octet() + 1);

			return sectionMap;
		};

		template<>
		auto make_section_map(reader::octet_reader& reader) -> _stdex::optional<section_map<grib_edition::V2, 2>>
		{
			std::size_t pos = reader.get_pos();

			uint32_t sectionSize;
			uint8_t sectionNumber;
			std::tie(sectionSize, sectionNumber) = reader.read_all<uint32_t, uint8_t>();

			if (sectionNumber != 2) {
				return {};
			}

			section_map<grib_edition::V2, 2> sectionMap {
				{section_octets::SECTION_LENGTH, {0, 3} },
				{section_octets::SECTION_NUMBER, {4, 4} },
				{section_octets::LOCAL_USE, {5, sectionSize - 1} }
			};

			sectionMap.shift_mapping(pos);
			reader.set_pos(*sectionMap.last_octet() + 1);

			return sectionMap;
		};

		template<>
		auto make_section_map(reader::octet_reader& reader) -> _stdex::optional<section_map<grib_edition::V2, 3>>
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

			section_map<grib_edition::V2, 3> sectionMap {
				{section_octets::SECTION_LENGTH, {0, 3} },
				{section_octets::SECTION_NUMBER, {4, 4} },
				{section_octets::SOURCE_OF_GRID_DEFINITION, {5, 5} },
				{section_octets::NUMBER_OF_DATA_POINTS, {6, 9} },
				{section_octets::LENGTH_FOR_OPTIONAL_LIST_OF_NUMBER_OF_POINTS, {10, 10} },
				{section_octets::INTERPRETATION_FOR_LIST_OF_NUMBER_OF_POINTS, {11, 11} },
				{section_octets::GRID_DEFINITION_TEMPLATE_NUMBER, {12, 13} },

			};

			size_t xx;
			if (sourceOfGribDefinition && templateNumber == 0xFF) {
				//-- if octet 6 is not zero and template number is set to 0xFF, then grid definition template may not be supplied --

			}
			else if (!sourceOfGribDefinition) {
				auto gridDefinitionTemplateMap = make_grid_definition_template_map<grib_edition::V2>(
						static_cast<grid_definition_template_number>(templateNumber), reader);
				xx = *gridDefinitionTemplateMap.last_octet();
			}
			else {
				//-- something wrong with bytes values --
				return {};
			}

			if (!optionalListLength && !optionalListInterpretation) {
				//-- optional list of number of points is not present --
			}
			else if (optionalListLength && optionalListInterpretation) {

			}





			sectionMap.shift_mapping(pos);
			reader.set_pos(*sectionMap.last_octet() + 1);

			return sectionMap;
		};





	};	//-- namespace octet_mapping --
};	//-- namespace gribpp --

#endif	//-- _GRIBPP_OCTATE_MAPPING_SECTION_HPP_ --
















