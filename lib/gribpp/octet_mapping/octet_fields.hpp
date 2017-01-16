#ifndef _GRIBPP_OCTET_MAPPING_OCTET_FIELDS_HPP_
#define _GRIBPP_OCTET_MAPPING_OCTET_FIELDS_HPP_

#include <cstdint>
#include <type_traits>

namespace gribpp {
	namespace octet_mapping {

		enum class fields : std::uint32_t {
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

	};	//-- namespace octet_mapping --
};	//-- namespace gribpp --

#endif	//-- _GRIBPP_OCTET_MAPPING_OCTET_FIELDS_HPP_ --
