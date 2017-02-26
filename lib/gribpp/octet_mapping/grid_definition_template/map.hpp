/*
 * template_definition_map.hpp
 *
 *  Created on: Jan 25, 2017
 *      Author: ilion
 */

#ifndef _GRIBPP_OCTET_MAPPING_TEMPLATE_DEFINITION_MAP_HPP_
#define _GRIBPP_OCTET_MAPPING_TEMPLATE_DEFINITION_MAP_HPP_

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <initializer_list>


#include "../../../utility/enum_hash.hpp"
#include "../../reader/octet_reader.hpp"
#include "../octet_map.hpp"
#include "../grib_edition.hpp"
#include "number.hpp"


namespace gribpp {
	namespace octet_mapping {
		namespace grid_definition_template {

			namespace _stdex = std::experimental;

			using std::size_t;
			using std::uint8_t;
			using std::uint16_t;
			using std::uint32_t;


			enum class grid_definition_template_octets : uint32_t {
				//-- 20 --
//				SHAPE_OF_THE_EARTH
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 0,
//
//				SCALE_FACTOR_OF_RADIUS_OF_SHPERICAL_EARTH
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 1,
//
//				SCALED_VALUE_OF_RADIUS_OF_SHPERICAL_EARTH
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 2,
//
//				SCALE_FACTOR_OF_MAJOR_AXIS_OF_OBLATE_SHPEROID_EARTH
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 3,
//
//				SCALED_VALUE_OF_MAJOR_AXIS_OF_OBLATE_SHPEROID_EARTH
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 4,
//
//				SCALE_FACTOR_OF_MINOR_AXIS_OF_OBLATE_SHPEROID_EARTH
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 5,
//
//				SCALED_VALUE_OF_MINOR_AXIS_OF_OBLATE_SHPEROID_EARTH
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 6,
//
//				NUMBER_OF_POINTS_ALONG_X_AXIS
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 7,
//
//				NUMBER_OF_POINTS_ALONG_Y_AXIS
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 8,
//
//				LATITUDE_OF_FIRST_GRID_POINT
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 9,
//
//				LONGITUDE_OF_FIRST_GRID_POINT
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 10,
//
//				RESOLUTION_AND_COMPONENT_FLAG
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 11,
//
//				LATITUDE_WHERE_Dx_AND_Dy_ARE_SPECIFIED
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 12,
//
//				ORIENTATION_OF_THE_GRID
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 13,
//
//				X_DIRECTION_GRID_LENGTH
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 14,
//
//				Y_DIRECTION_GRID_LENGTH
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 15,
//
//				PROJECTION_CENTER_FLAG
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 16,
//
//				SCANNING_MODE
//					= ((uint32_t)number::polar_stereographic_projection << 16) + 17


			};


			template<grib_edition V>
			class map :
				public octet_map<grid_definition_template_octets>
			{
			public:
				static constexpr grib_edition grib_version = V;

				using base_type = octet_map<grid_definition_template_octets>;
				using map_type 		= typename base_type::map_type;
				using value_type 	= typename map_type::value_type;
				using key_type 		= typename map_type::key_type;
				using mapped_type 	= typename map_type::mapped_type;

				using cached_grid_definition_template_maps = std::unordered_map<number, map_type>;


				inline map(number n) :
					base_type(),
					mGridDefinitionTemplateNumber(n)
				{};
				inline map(number n, const map_type& m) :
					base_type(m),
					mGridDefinitionTemplateNumber(n)
				{};
				inline map(const map& rhs) :
					base_type(rhs),
					mGridDefinitionTemplateNumber(rhs.grid_definition_template_number())
				{};
				inline map(map&& rhs) :
					base_type(rhs),
					mGridDefinitionTemplateNumber(std::move(rhs.mGridDefinitionTemplateNumber))
				{};
				inline map(number n, std::initializer_list<value_type> initList) :
					base_type(initList),
					mGridDefinitionTemplateNumber(n)
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

				inline grib_edition version() const {
					return grib_version;
				};

				inline number grid_definition_template_number() const {
					return mGridDefinitionTemplateNumber;
				};

				static const cached_grid_definition_template_maps definitionTemplateMap;

			private:
				number mGridDefinitionTemplateNumber;
			};


//			template<>
//			const map<grib_edition::V2>::cached_grid_definition_template_maps
//			map<grib_edition::V2>::definitionTemplateMap {
//					{number::polar_stereographic_projection,
//						{
//							{grid_definition_template_octets::SHAPE_OF_THE_EARTH, {0, 0}},
//							{grid_definition_template_octets::SCALE_FACTOR_OF_RADIUS_OF_SHPERICAL_EARTH, {1, 1}},
//							{grid_definition_template_octets::SCALED_VALUE_OF_RADIUS_OF_SHPERICAL_EARTH, {2, 5}},
//							{grid_definition_template_octets::SCALE_FACTOR_OF_MAJOR_AXIS_OF_OBLATE_SHPEROID_EARTH, {6, 6}},
//							{grid_definition_template_octets::SCALED_VALUE_OF_MAJOR_AXIS_OF_OBLATE_SHPEROID_EARTH, {7, 10}},
//							{grid_definition_template_octets::SCALE_FACTOR_OF_MINOR_AXIS_OF_OBLATE_SHPEROID_EARTH, {11, 11}},
//							{grid_definition_template_octets::SCALED_VALUE_OF_MINOR_AXIS_OF_OBLATE_SHPEROID_EARTH, {12, 15}},
//							{grid_definition_template_octets::NUMBER_OF_POINTS_ALONG_X_AXIS, {16, 19}},
//							{grid_definition_template_octets::NUMBER_OF_POINTS_ALONG_Y_AXIS, {20, 23}},
//							{grid_definition_template_octets::LATITUDE_OF_FIRST_GRID_POINT, {24, 27}},
//							{grid_definition_template_octets::LONGITUDE_OF_FIRST_GRID_POINT, {28, 31}},
//							{grid_definition_template_octets::RESOLUTION_AND_COMPONENT_FLAG, {32, 32}},
//							{grid_definition_template_octets::LATITUDE_WHERE_Dx_AND_Dy_ARE_SPECIFIED, {33, 36}},
//							{grid_definition_template_octets::ORIENTATION_OF_THE_GRID, {37, 40}},
//							{grid_definition_template_octets::X_DIRECTION_GRID_LENGTH, {41, 44}},
//							{grid_definition_template_octets::Y_DIRECTION_GRID_LENGTH, {45, 48}},
//							{grid_definition_template_octets::PROJECTION_CENTER_FLAG, {49, 49}},
//							{grid_definition_template_octets::SCANNING_MODE, {50, 50}}
//						}
//					}
//			};

//			template<>
//			const grid_definition_template_size<grib_edition::V2>::grid_definition_template_static_size_map
//			grid_definition_template_size<grib_edition::V2>::staticSize {
//				{grid_definition_template_number::LATITUDE_LONGITUDE, 58},
//				{grid_definition_template_number::ROTATED_LATITUDE_LONGITUDE, 70},
//				{grid_definition_template_number::STRETCHED_LATITUDE_LONGITUDE, 70},
//				{grid_definition_template_number::STRETCHED_AND_ROTATED_LATITUDE_LONGITUDE, 82},
//
//				{grid_definition_template_number::MERCATOR, 58},
//
//				{grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION, 41},
//
//				{grid_definition_template_number::LAMBERT_CONFORMAL, 67},
//
//				{grid_definition_template_number::ALBERS_EQUAL_AREA, 67},
//
//				{grid_definition_template_number::GAUSSIAN_LATITUDE_LONGITUDE, 58},
//				{grid_definition_template_number::ROTATED_GAUSSIAN_LATITUDE_LONGITUDE, 70},
//				{grid_definition_template_number::STRETCHED_GAUSSIAN_LATITUDE_LONGITUDE, 70},
//				{grid_definition_template_number::STRETCHED_AND_ROTATED_GAUSSIAN_LATITUDE_LONGITUDE, 82},
//
//				{grid_definition_template_number::SHPERICAL_HARMONIC_COEFFICIENTS, 14},
//				{grid_definition_template_number::ROTATED_SHPERICAL_HARMONIC_COEFFICIENTS, 26},
//				{grid_definition_template_number::STRETCHED_SHPERICAL_HARMONIC_COEFFICIENTS, 26},
//				{grid_definition_template_number::STRETCHED_AND_ROTATED_SHPERICAL_HARMONIC_COEFFICIENTS, 38},
//
//				{grid_definition_template_number::SPACE_VIEW_PERSPECTIVE, 66},
//
//				{grid_definition_template_number::TRIANGULAR_GRID_BASED_ON_ICOSAHEDRON, 24},
//
//				{grid_definition_template_number::EQUATORIAL_AZIMUTHAL_EQUIDISTANT_PROJECTION, 11},
//
//				{grid_definition_template_number::HOVMJOLLER_DIAGRAM_GRID, 68}
//
//			};


			template<grib_edition V>
			auto make_map(number n, reader::octet_reader& r)
				-> map<V>;

			template<>
			auto make_map(number n, reader::octet_reader& r)
				-> map<grib_edition::V2>
			{
				size_t pos = r.get_pos();

				map<grib_edition::V2> tdMap {n, map<grib_edition::V2>::definitionTemplateMap.at(n)};
				tdMap.shift_mapping(pos);
				r.set_pos(*tdMap.last_octet() + 1);

				return tdMap;
			};

		};	//-- namespace grid_definition_template --
	};	//-- namespace octet_mapping --
};	//-- namespace gribpp --




#endif /* _GRIBPP_OCTET_MAPPING_TEMPLATE_DEFINITION_MAP_HPP_ */
