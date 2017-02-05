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
#include <initializer_list>


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


		enum class template_definitions : uint16_t {
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


		template<grib_edition V, template_definitions TD>
		struct template_definition_map :
			public map_inheritance_helper<octet_map<template_definitions>, template_definition_map<V, TD>>
		{
			static constexpr grib_edition grib_version = V;
			static constexpr template_definitions template_definition = TD;

			using inheritance_helper = map_inheritance_helper<octet_map<template_definitions>, template_definition_map<grib_version, template_definition>>;
			using inheritance_helper::inheritance_helper;

			inline template_definitions definition() const {
				return template_definition;
			};
			inline grib_edition version() const {
				return grib_version;
			};
		};


		template<grib_edition V, template_definitions TD>
		auto make_template_definition_map(reader::octet_reader& r) -> _stdex::optional<template_definition_map<V, TD>>;




	};
};




#endif /* _GRIBPP_OCTET_MAPPING_TEMPLATE_DEFINITION_MAP_HPP_ */
