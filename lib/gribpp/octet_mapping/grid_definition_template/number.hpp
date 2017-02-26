/*
 * grid_definition_template_number.hpp
 *
 *  Created on: Feb 18, 2017
 *      Author: ilion
 */

#ifndef _GRIBPP_OCTET_MAPPING_GRID_DEFINITION_TEMPLATE_NUMBER_HPP_
#define _GRIBPP_OCTET_MAPPING_GRID_DEFINITION_TEMPLATE_NUMBER_HPP_

#include <cstdint>
#include <unordered_set>


namespace gribpp {
	namespace octet_mapping {
		namespace grid_definition_template {

			using std::uint8_t;
			using std::uint16_t;


			struct flag {
				using value_type = uint16_t;

				static const value_type rotated			{1 << 0};
				static const value_type stretched		{1 << 1};
				static const value_type deformable		{1 << 2};
				static const value_type variadic_size	{1 << 3};
			};


			class number {
			public:
				using value_type = uint16_t;
				using flag_type = flag::value_type;

				static const number latitude_longitude;
				static const number mercator;
				static const number polar_stereographic_projection;
				static const number lambert_conformal;
				static const number albert_equal_area;
				static const number gaussian_latitude_longitude;
				static const number shperical_harmonic_coefficients;
				static const number space_view_perspective;
				static const number triangilar_grid_based_on_icosahedron;
				static const number equatorial_azimuthal_equidestant_projection;
				static const number azimuth_range_projection;
				static const number cross_section_grid;
				static const number hovmjoller_diagram_grid;
				static const number time_section_grid;

				static const std::unordered_set<value_type> deformable_templates;
				static const std::unordered_set<value_type> variadic_templates;

				static value_type base_builder(const value_type val) {
					value_type type = val;
					if (deformable_templates.find(val) != deformable_templates.cend()) {
						type = val / 10 * 10;
					}
					return type;
				};

				static flag_type flag_builder(const value_type val) {
					flag_type flags = 0;
					value_type div = val / 10;
					value_type rem = val % 10;
					if (deformable_templates.find(div * 10) != deformable_templates.cend()) {
						flags |= flag::deformable;
						if (rem & (1 << 0))
							flags |= flag::rotated;
						if (rem & (1 << 1))
							flags |= flag::stretched;
					}
					if (variadic_templates.find(div * 10) != variadic_templates.cend()) {
						flags |= flag::variadic_size;
					}
					return flags;
				};

				inline explicit number(const value_type n) :
					mBase(base_builder(n)),
					mFlags(flag_builder(n))
				{};


				inline explicit operator value_type() const {
					return base() + (flags() & (flag::rotated | flag::stretched));
				};
				inline value_type base() const {
					return mBase;
				};
				inline flag_type flags() const {
					return mFlags;
				};

				inline bool is_rotated() const {
					return (flags() & flag::rotated);
				};
				inline bool is_stretched() const {
					return (flags() & flag::stretched);
				};
				inline bool is_deformable() const {
					return (flags() & flag::deformable);
				};
				inline bool is_variadic() const {
					return (flags() & flag::variadic_size);
				}

				number operator | (const flag_type rhs) const {
					return number {	base(), flags() | rhs };
				};
				number operator & (const flag_type rhs) const {
					return number { base(), flags() & rhs };
				};


			private:
				number(const value_type base, const flag_type flags) :
					mBase(base),
					mFlags(flags)
				{};

				value_type mBase;
				flag_type mFlags;
			};


			const number number::latitude_longitude								{	0,		flag::deformable};
			const number number::mercator 										{	10	};
			const number number::polar_stereographic_projection					{	20	};
			const number number::lambert_conformal 								{	30	};
			const number number::albert_equal_area								{	31	};
			const number number::gaussian_latitude_longitude					{	40,	flag::deformable};
			const number number::shperical_harmonic_coefficients				{	50,	flag::deformable};
			const number number::space_view_perspective							{	90	};
			const number number::triangilar_grid_based_on_icosahedron			{	100	};
			const number number::equatorial_azimuthal_equidestant_projection	{	110	};
			const number number::azimuth_range_projection						{	120,	flag::variadic_size};
			const number number::cross_section_grid								{	1000,	flag::variadic_size};
			const number number::hovmjoller_diagram_grid						{	1100	};
			const number number::time_section_grid								{	1200,	flag::variadic_size};


			const std::unordered_set<number::value_type>
			number::deformable_templates {
				number::latitude_longitude.base(),
				number::gaussian_latitude_longitude.base(),
				number::shperical_harmonic_coefficients.base()
			};

			const std::unordered_set<number::value_type>
			number::variadic_templates {
				number::azimuth_range_projection.base(),
				number::cross_section_grid.base(),
				number::time_section_grid.base()
			};



		};	//-- namespace grid_definition_template --
	};	//-- namespace octet_mapping --
};	//-- namespace gribpp --



#endif /* _GRIBPP_OCTET_MAPPING_GRID_DEFINITION_TEMPLATE_NUMBER_HPP_ */
