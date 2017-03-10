/*
 * map.hpp
 *
 *  Created on: Jan 25, 2017
 *      Author: ilion
 */

#ifndef _GRIBPP_OCTET_MAPPING_GRID_DEFINITION_TEMPLATE_MAP_HPP_
#define _GRIBPP_OCTET_MAPPING_GRID_DEFINITION_TEMPLATE_MAP_HPP_

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <bits/functional_hash.h>
#include <bits/stl_function.h>


#include "../../../utility/convert.hpp"
#include "../../reader/octet_reader.hpp"
#include "../octet_map.hpp"
#include "../grib_edition.hpp"
#include "number.hpp"


namespace gribpp {
	namespace octet_mapping {
		namespace grid_definition_template {

			namespace stdex = std::experimental;

			using std::size_t;
			using std::uint8_t;
			using std::uint16_t;
			using std::uint32_t;


			enum class octets : uint32_t {
				//-- 20 --
				SHAPE_OF_THE_EARTH
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 0,

				SCALE_FACTOR_OF_RADIUS_OF_SHPERICAL_EARTH
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 1,

				SCALED_VALUE_OF_RADIUS_OF_SHPERICAL_EARTH
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 2,

				SCALE_FACTOR_OF_MAJOR_AXIS_OF_OBLATE_SHPEROID_EARTH
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 3,

				SCALED_VALUE_OF_MAJOR_AXIS_OF_OBLATE_SHPEROID_EARTH
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 4,

				SCALE_FACTOR_OF_MINOR_AXIS_OF_OBLATE_SHPEROID_EARTH
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 5,

				SCALED_VALUE_OF_MINOR_AXIS_OF_OBLATE_SHPEROID_EARTH
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 6,

				NUMBER_OF_POINTS_ALONG_X_AXIS
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 7,

				NUMBER_OF_POINTS_ALONG_Y_AXIS
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 8,

				LATITUDE_OF_FIRST_GRID_POINT
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 9,

				LONGITUDE_OF_FIRST_GRID_POINT
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 10,

				RESOLUTION_AND_COMPONENT_FLAG
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 11,

				LATITUDE_WHERE_Dx_AND_Dy_ARE_SPECIFIED
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 12,

				ORIENTATION_OF_THE_GRID
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 13,

				X_DIRECTION_GRID_LENGTH
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 14,

				Y_DIRECTION_GRID_LENGTH
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 15,

				PROJECTION_CENTER_FLAG
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 16,

				SCANNING_MODE
					= ((uint32_t)number_base::polar_stereographic_projection << 16) + 17


			};


			template<grib_edition V>
			class map :
				public octet_map<octets>
			{
			public:
				static constexpr grib_edition grib_version = V;

				using base_type = octet_map<octets>;
				using map_type 		= typename base_type::map_type;
				using value_type 	= typename map_type::value_type;
				using key_type 		= typename map_type::key_type;
				using mapped_type 	= typename map_type::mapped_type;

				using cached_grid_definition_template_maps = std::unordered_map<number, map_type, number_hash, number_equal_to>;
				using grid_definition_template_const_size_map = std::unordered_map<number, size_t, number_hash, number_equal_to>;


				inline map(const number& n) :
					base_type(),
					mGridDefinitionTemplateNumber(n)
				{};
				inline map(const number& n, const map_type& m) :
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

				inline map& operator= (const map_type& other) {
					base_type::operator=(other);
					return (*this);
				};
				inline map& operator= (const map& other) {
					base_type::operator =(other);
					mGridDefinitionTemplateNumber = other.mGridDefinitionTemplateNumber;
					return (*this);
				};
				inline map& operator= (map&& other) {
					base_type::operator =(other);
					mGridDefinitionTemplateNumber = other.mGridDefinitionTemplateNumber;
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

				static const cached_grid_definition_template_maps gridDefinitionTemplateConstMap;
				static const grid_definition_template_const_size_map gridDefinitionTemplateConstSize;

			private:
				number mGridDefinitionTemplateNumber;
			};


			template<>
			const map<grib_edition::V2>::cached_grid_definition_template_maps
			map<grib_edition::V2>::gridDefinitionTemplateConstMap {
					{to_number(number_base::polar_stereographic_projection),
						{
							{octets::SHAPE_OF_THE_EARTH, {0, 0}},
							{octets::SCALE_FACTOR_OF_RADIUS_OF_SHPERICAL_EARTH, {1, 1}},
							{octets::SCALED_VALUE_OF_RADIUS_OF_SHPERICAL_EARTH, {2, 5}},
							{octets::SCALE_FACTOR_OF_MAJOR_AXIS_OF_OBLATE_SHPEROID_EARTH, {6, 6}},
							{octets::SCALED_VALUE_OF_MAJOR_AXIS_OF_OBLATE_SHPEROID_EARTH, {7, 10}},
							{octets::SCALE_FACTOR_OF_MINOR_AXIS_OF_OBLATE_SHPEROID_EARTH, {11, 11}},
							{octets::SCALED_VALUE_OF_MINOR_AXIS_OF_OBLATE_SHPEROID_EARTH, {12, 15}},
							{octets::NUMBER_OF_POINTS_ALONG_X_AXIS, {16, 19}},
							{octets::NUMBER_OF_POINTS_ALONG_Y_AXIS, {20, 23}},
							{octets::LATITUDE_OF_FIRST_GRID_POINT, {24, 27}},
							{octets::LONGITUDE_OF_FIRST_GRID_POINT, {28, 31}},
							{octets::RESOLUTION_AND_COMPONENT_FLAG, {32, 32}},
							{octets::LATITUDE_WHERE_Dx_AND_Dy_ARE_SPECIFIED, {33, 36}},
							{octets::ORIENTATION_OF_THE_GRID, {37, 40}},
							{octets::X_DIRECTION_GRID_LENGTH, {41, 44}},
							{octets::Y_DIRECTION_GRID_LENGTH, {45, 48}},
							{octets::PROJECTION_CENTER_FLAG, {49, 49}},
							{octets::SCANNING_MODE, {50, 50}}
						}
					}
			};

			template<>
			const map<grib_edition::V2>::grid_definition_template_const_size_map
			map<grib_edition::V2>::gridDefinitionTemplateConstSize {
				{static_cast<number>(number_base::latitude_longitude), 58},
				{number_base::latitude_longitude | deformation::rotated, 70},
				{number_base::latitude_longitude | deformation::stretched, 70},
				{number_base::latitude_longitude | deformation::rotated | deformation::stretched, 82},

				{static_cast<number>(number_base::mercator), 58},

				{static_cast<number>(number_base::polar_stereographic_projection), 41},

				{static_cast<number>(number_base::lambert_conformal), 67},

				{static_cast<number>(number_base::albert_equal_area), 67},

				{static_cast<number>(number_base::gaussian_latitude_longitude), 58},
				{number_base::gaussian_latitude_longitude | deformation::rotated, 70},
				{number_base::gaussian_latitude_longitude | deformation::stretched, 70},
				{number_base::gaussian_latitude_longitude | deformation::rotated | deformation::stretched, 82},

				{static_cast<number>(number_base::shperical_harmonic_coefficients), 14},
				{number_base::shperical_harmonic_coefficients | deformation::rotated, 26},
				{number_base::shperical_harmonic_coefficients | deformation::stretched, 26},
				{number_base::shperical_harmonic_coefficients | deformation::rotated | deformation::stretched, 38},

				{static_cast<number>(number_base::space_view_perspective), 66},

				{static_cast<number>(number_base::triangilar_grid_based_on_icosahedron), 24},

				{static_cast<number>(number_base::equatorial_azimuthal_equidestant_projection), 11},

				{static_cast<number>(number_base::hovmjoller_diagram_grid), 68}

			};


			template<grib_edition V>
			auto make_map(const number& n, reader::octet_reader& r)
				-> map<V>;

			template<>
			auto make_map(const number& n, reader::octet_reader& r)
				-> map<grib_edition::V2>
			{
				size_t pos = r.get_pos();

				map<grib_edition::V2> gtdMap(n);
				if (!n.is_variadic()) {
					//-- getting mapping from predefined map --
					gtdMap = map<grib_edition::V2>::gridDefinitionTemplateConstMap.at(n);
				}
				else {
					//-- reading to calculate correct map (to-do) --
				}
				gtdMap.shift_mapping(pos);
				r.set_pos(*gtdMap.last_octet() + 1);

				return gtdMap;
			};

		};	//-- namespace grid_definition_template --
	};	//-- namespace octet_mapping --
};	//-- namespace gribpp --




#endif /* _GRIBPP_OCTET_MAPPING_GRID_DEFINITION_TEMPLATE_MAP_HPP_ */
