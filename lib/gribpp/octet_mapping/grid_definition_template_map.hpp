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


#include "../../utility/enum_hash.hpp"
#include "octet_map.hpp"
#include "grib_edition.hpp"
#include "map_inheritance_helper.hpp"
#include "../reader/octet_reader.hpp"


namespace gribpp {
	namespace octet_mapping {

		namespace _stdex = std::experimental;

		using std::uint8_t;
		using std::uint16_t;
		using std::uint32_t;

		using std::size_t;


		enum class grid_definition_template_number : uint16_t {
			//-- 0x --
			LATITUDE_LONGITUDE = 0,
			ROTATED_LATITUDE_LONGITUDE = 1,
			STRETCHED_LATITUDE_LONGITUDE = 2,
			STRETCHED_AND_ROTATED_LATITUDE_LONGITUDE = 3,
			//-- 1x --
			MERCATOR = 10,
			//-- 2x --
			POLAR_STEREOGRAPHIC_PROJECTION = 20,
			//-- 3x --
			LAMBERT_CONFORMAL = 30,
			ALBERS_EQUAL_AREA = 31,
			//-- 4x --
			GAUSSIAN_LATITUDE_LONGITUDE = 40,
			ROTATED_GAUSSIAN_LATITUDE_LONGITUDE = 41,
			STRETCHED_GAUSSIAN_LATITUDE_LONGITUDE = 42,
			STRETCHED_AND_ROTATED_GAUSSIAN_LATITUDE_LONGITUDE = 43,
			//-- 5x --
			SHPERICAL_HARMONIC_COEFFICIENTS = 50,
			ROTATED_SHPERICAL_HARMONIC_COEFFICIENTS = 51,
			STRETCHED_SHPERICAL_HARMONIC_COEFFICIENTS = 52,
			STRETCHED_AND_ROTATED_SHPERICAL_HARMONIC_COEFFICIENTS = 53,
			//-- 9x --
			SPACE_VIEW_PERSPECTIVE = 90,
			//-- 10x --
			TRIANGULAR_GRID_BASED_ON_ICOSAHEDRON = 100,
			//-- 11x --
			EQUATORIAL_AZIMUTHAL_EQUIDISTANT_PROJECTION = 110,
			//-- 12x --
			AZIMUTH_RANGE_PROJECTION = 120,
			//-- 100x --
			CROSS_SECTION_GRID = 1000,
			//-- 110x --
			HOVMJOLLER_DIAGRAM_GRID = 1100,
			//-- 120x --
			TIME_SECTION_GRID = 1200

		};


		enum class grid_definition_template : uint32_t {
			//-- 20 --
			SHAPE_OF_THE_EARTH
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 0,

			SCALE_FACTOR_OF_RADIUS_OF_SHPERICAL_EARTH
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 1,

			SCALED_VALUE_OF_RADIUS_OF_SHPERICAL_EARTH
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 2,

			SCALE_FACTOR_OF_MAJOR_AXIS_OF_OBLATE_SHPEROID_EARTH
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 3,

			SCALED_VALUE_OF_MAJOR_AXIS_OF_OBLATE_SHPEROID_EARTH
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 4,

			SCALE_FACTOR_OF_MINOR_AXIS_OF_OBLATE_SHPEROID_EARTH
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 5,

			SCALED_VALUE_OF_MINOR_AXIS_OF_OBLATE_SHPEROID_EARTH
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 6,

			NUMBER_OF_POINTS_ALONG_X_AXIS
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 7,

			NUMBER_OF_POINTS_ALONG_Y_AXIS
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 8,

			LATITUDE_OF_FIRST_GRID_POINT
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 9,

			LONGITUDE_OF_FIRST_GRID_POINT
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 10,

			RESOLUTION_AND_COMPONENT_FLAG
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 11,

			LATITUDE_WHERE_Dx_AND_Dy_ARE_SPECIFIED
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 12,

			ORIENTATION_OF_THE_GRID
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 13,

			X_DIRECTION_GRID_LENGTH
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 14,

			Y_DIRECTION_GRID_LENGTH
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 15,

			PROJECTION_CENTER_FLAG
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 16,

			SCANNING_MODE
				= ((uint32_t)grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION << 16) + 17


		};

		template<grib_edition V>
		struct grid_definition_template_size {
			using grid_definition_template_static_size_map = std::unordered_map<
					grid_definition_template_number,
					size_t,
					utility::enum_hash<grid_definition_template_number>>;

			static const grid_definition_template_static_size_map value;


		};


		template<grib_edition V>
		class grid_definition_template_map :
			public octet_map<grid_definition_template>
		{
		public:
			static constexpr grib_edition grib_version = V;

			using base_type = octet_map<grid_definition_template>;
			using map_type 		= typename base_type::map_type;
			using value_type 	= typename map_type::value_type;
			using key_type 		= typename map_type::key_type;
			using mapped_type 	= typename map_type::mapped_type;

			using cached_grid_definition_template_maps = std::unordered_map<grid_definition_template_number, map_type, utility::enum_hash<grid_definition_template_number>>;


			inline grid_definition_template_map(grid_definition_template_number n) :
				base_type(),
				mNumber(n)
			{};
			inline grid_definition_template_map(grid_definition_template_number n, const map_type& m) :
				base_type(m),
				mNumber(n)
			{};
			inline grid_definition_template_map(const grid_definition_template_map& other) :
				base_type(other),
				mNumber(other.number())
			{};
			inline grid_definition_template_map(grid_definition_template_map&& other) :
				base_type(other),
				mNumber(std::move(other.mNumber))
			{};
			inline grid_definition_template_map(grid_definition_template_number definitionTemplateNummber, std::initializer_list<value_type> initList) :
				base_type(initList),
				mNumber(definitionTemplateNummber)
			{};

			inline grid_definition_template_map& operator= (const grid_definition_template_map& other) {
				base_type::operator =(other);
				return (*this);
			};
			inline grid_definition_template_map& operator= (grid_definition_template_map&& other) {
				base_type::operator =(other);
				return (*this);
			};
			inline grid_definition_template_map& operator= (std::initializer_list<value_type> initList) {
				base_type::operator =(initList);
				return (*this);
			};

			inline grib_edition version() const {
				return grib_version;
			};

			inline grid_definition_template_number number() const {
				return mNumber;
			};

			static const cached_grid_definition_template_maps definitionTemplateMap;

		private:
			grid_definition_template_number mNumber;
		};


		template<>
		const grid_definition_template_map<grib_edition::V2>::cached_grid_definition_template_maps
		grid_definition_template_map<grib_edition::V2>::definitionTemplateMap {
				{grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION,
					{
						{grid_definition_template::SHAPE_OF_THE_EARTH, {0, 0}},
						{grid_definition_template::SCALE_FACTOR_OF_RADIUS_OF_SHPERICAL_EARTH, {1, 1}},
						{grid_definition_template::SCALED_VALUE_OF_RADIUS_OF_SHPERICAL_EARTH, {2, 5}},
						{grid_definition_template::SCALE_FACTOR_OF_MAJOR_AXIS_OF_OBLATE_SHPEROID_EARTH, {6, 6}},
						{grid_definition_template::SCALED_VALUE_OF_MAJOR_AXIS_OF_OBLATE_SHPEROID_EARTH, {7, 10}},
						{grid_definition_template::SCALE_FACTOR_OF_MINOR_AXIS_OF_OBLATE_SHPEROID_EARTH, {11, 11}},
						{grid_definition_template::SCALED_VALUE_OF_MINOR_AXIS_OF_OBLATE_SHPEROID_EARTH, {12, 15}},
						{grid_definition_template::NUMBER_OF_POINTS_ALONG_X_AXIS, {16, 19}},
						{grid_definition_template::NUMBER_OF_POINTS_ALONG_Y_AXIS, {20, 23}},
						{grid_definition_template::LATITUDE_OF_FIRST_GRID_POINT, {24, 27}},
						{grid_definition_template::LONGITUDE_OF_FIRST_GRID_POINT, {28, 31}},
						{grid_definition_template::RESOLUTION_AND_COMPONENT_FLAG, {32, 32}},
						{grid_definition_template::LATITUDE_WHERE_Dx_AND_Dy_ARE_SPECIFIED, {33, 36}},
						{grid_definition_template::ORIENTATION_OF_THE_GRID, {37, 40}},
						{grid_definition_template::X_DIRECTION_GRID_LENGTH, {41, 44}},
						{grid_definition_template::Y_DIRECTION_GRID_LENGTH, {45, 48}},
						{grid_definition_template::PROJECTION_CENTER_FLAG, {49, 49}},
						{grid_definition_template::SCANNING_MODE, {50, 50}}
					}
				}
		};

		template<>
		const grid_definition_template_size<grib_edition::V2>::grid_definition_template_static_size_map
		grid_definition_template_size<grib_edition::V2>::value {
			{grid_definition_template_number::LATITUDE_LONGITUDE, 58},
			{grid_definition_template_number::ROTATED_LATITUDE_LONGITUDE, 70},
			{grid_definition_template_number::STRETCHED_LATITUDE_LONGITUDE, 70},
			{grid_definition_template_number::STRETCHED_AND_ROTATED_LATITUDE_LONGITUDE, 82},

			{grid_definition_template_number::MERCATOR, 58},

			{grid_definition_template_number::POLAR_STEREOGRAPHIC_PROJECTION, 41},

			{grid_definition_template_number::LAMBERT_CONFORMAL, 67},

			{grid_definition_template_number::ALBERS_EQUAL_AREA, 67},

			{grid_definition_template_number::GAUSSIAN_LATITUDE_LONGITUDE, 58},
			{grid_definition_template_number::ROTATED_GAUSSIAN_LATITUDE_LONGITUDE, 70},
			{grid_definition_template_number::STRETCHED_GAUSSIAN_LATITUDE_LONGITUDE, 70},
			{grid_definition_template_number::STRETCHED_AND_ROTATED_GAUSSIAN_LATITUDE_LONGITUDE, 82},

			{grid_definition_template_number::SHPERICAL_HARMONIC_COEFFICIENTS, 14},
			{grid_definition_template_number::ROTATED_SHPERICAL_HARMONIC_COEFFICIENTS, 26},
			{grid_definition_template_number::STRETCHED_SHPERICAL_HARMONIC_COEFFICIENTS, 26},
			{grid_definition_template_number::STRETCHED_AND_ROTATED_SHPERICAL_HARMONIC_COEFFICIENTS, 38},

			{grid_definition_template_number::SPACE_VIEW_PERSPECTIVE, 66},

			{grid_definition_template_number::TRIANGULAR_GRID_BASED_ON_ICOSAHEDRON, 24},

			{grid_definition_template_number::EQUATORIAL_AZIMUTHAL_EQUIDISTANT_PROJECTION, 11},

			{grid_definition_template_number::HOVMJOLLER_DIAGRAM_GRID, 68}

		};


		template<grib_edition V>
		auto make_grid_definition_template_map(grid_definition_template_number n, reader::octet_reader& r)
			-> grid_definition_template_map<V>;

		template<>
		auto make_grid_definition_template_map(grid_definition_template_number n, reader::octet_reader& r)
			-> grid_definition_template_map<grib_edition::V2>
		{
			size_t pos = r.get_pos();

			grid_definition_template_map<grib_edition::V2> tdMap {n, grid_definition_template_map<grib_edition::V2>::definitionTemplateMap.at(n)};
			tdMap.shift_mapping(pos);
			r.set_pos(*tdMap.last_octet() + 1);

			return tdMap;
		};


	};
};




#endif /* _GRIBPP_OCTET_MAPPING_TEMPLATE_DEFINITION_MAP_HPP_ */
