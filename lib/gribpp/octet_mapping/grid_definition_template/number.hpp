/*
 * number.hpp
 *
 *  Created on: Feb 18, 2017
 *      Author: ilion
 */

#ifndef _GRIBPP_OCTET_MAPPING_GRID_DEFINITION_TEMPLATE_NUMBER_HPP_
#define _GRIBPP_OCTET_MAPPING_GRID_DEFINITION_TEMPLATE_NUMBER_HPP_

#include <cstddef>
#include <cstdint>
#include <array>
#include <bits/functional_hash.h>
#include <tuple>
#include <type_traits>


#include "../../../utility/fn/sequence_fn.hpp"


namespace gribpp {
	namespace octet_mapping {
		namespace grid_definition_template {


			using std::size_t;
			using std::uint16_t;
			using std::uint32_t;





			enum class number_base : uint16_t {
				latitude_longitude								= 0,
				mercator										= 10,
				polar_stereographic_projection					= 20,
				lambert_conformal								= 30,
				albert_equal_area								= 31,
				gaussian_latitude_longitude						= 40,
				shperical_harmonic_coefficients					= 50,
				space_view_perspective							= 90,
				triangilar_grid_based_on_icosahedron			= 100,
				equatorial_azimuthal_equidestant_projection		= 110,
				azimuth_range_projection						= 120,
				cross_section_grid								= 1000,
				hovmjoller_diagram_grid							= 1100,
				time_section_grid								= 1200
			};




			enum class deformation : uint16_t {
				none 		= 0,
				rotated		= 1 << 0,
				stretched	= 1 << 1
			};


			constexpr deformation operator | (const deformation x, const deformation y) {
				return static_cast<deformation>(static_cast<uint16_t>(x) | static_cast<uint16_t>(y));
			};
			constexpr deformation operator & (const deformation x, const deformation y) {
				return static_cast<deformation>(static_cast<uint16_t>(x) & static_cast<uint16_t>(y));
			};


			class number;
			constexpr number to_number(const uint16_t val);
			constexpr number to_number(const number_base val);

			class number {
			public:
				static constexpr std::array<uint16_t, 12> deformable_templates { 0, 1, 2, 3, 40, 41, 42, 43, 50, 51, 52, 53 };
				static constexpr std::array<uint16_t, 3> variadic_templates { 120, 1000, 1200 };

				friend constexpr number to_number(const uint16_t val);

				constexpr number(const number& other) :
					mNumberInfo { other.mNumberInfo },
					mDeformationFlags { other.mDeformationFlags }
				{};

				constexpr explicit operator uint16_t() const {
					return static_cast<uint16_t>(base()) + static_cast<uint16_t>(is_deformable() ? flags() : deformation::none);
				};
				constexpr explicit operator uint32_t() const {
					return static_cast<uint16_t>(*this);
				};

				constexpr deformation flags() const {
					return mDeformationFlags;
				};
				constexpr number_base base() const {
					return std::get<0>(mNumberInfo);
				};
				constexpr bool is_deformable() const {
					return std::get<1>(mNumberInfo);
				};
				constexpr bool is_variadic() const {
					return std::get<2>(mNumberInfo);
				};

				constexpr number operator | (const deformation rhs) const {
					return { mNumberInfo, is_deformable() ? flags() | rhs : deformation::none };
				};
				constexpr number operator & (const deformation rhs) const {
					return { mNumberInfo, flags() & rhs };
				};


			private:
				constexpr number(const std::tuple<number_base, bool, bool>& numberInfo, const deformation deformationFlags) :
					mNumberInfo { numberInfo },
					mDeformationFlags { deformationFlags }
				{};

				const std::tuple<number_base, bool, bool> mNumberInfo;
				deformation mDeformationFlags;
			};

			constexpr std::array<uint16_t, 12> number::deformable_templates;
			constexpr std::array<uint16_t, 3> number::variadic_templates;


			constexpr number to_number(const uint16_t val) {
				namespace fn = utility::fn;
				auto map 	= fn::make_apply( fn::make_map( fn::unary<std::equal_to, decltype(val)>{ val } ) );
				auto reduce	= fn::make_apply( fn::make_reduce( std::logical_or<bool>{} ) );

				uint16_t base = val;
				deformation flags = deformation::none;
				bool isDeformable = false;
				bool isVariadic = false;
				if ( reduce( map(number::deformable_templates) ) ) {
					base = val / 10 * 10;
					flags = static_cast<deformation>(val % 10);
					isDeformable = true;
				}
				if ( reduce( map(number::variadic_templates) ) ) {
					isVariadic = true;
				}

				return { std::make_tuple(static_cast<number_base>(base), isDeformable, isVariadic), flags };
			};

			constexpr number to_number(const number_base val) { return to_number(static_cast<uint16_t>(val)); };


			constexpr number operator | (const number_base n, const deformation x) {
				return to_number(static_cast<uint16_t>(n)) | x;
			};
			constexpr number operator & (const number_base n, const deformation x) {
				return to_number(static_cast<uint16_t>(n)) & x;
			};



			struct number_hash : public std::__hash_base<size_t, number_base> {
				size_t operator() (const number& val) const noexcept {
					return static_cast<size_t>( static_cast<uint16_t>(val) );
				};
			};

			struct number_equal_to : public std::binary_function<number, number, bool> {
				bool
				operator() (const number& x, const number& y) const {
					return static_cast<uint16_t>(x) == static_cast<uint16_t>(y);
				};
			};


		};	//-- namespace grid_definition_template --
	};	//-- namespace octet_mapping --
};	//-- namespace gribpp --



#endif /* _GRIBPP_OCTET_MAPPING_GRID_DEFINITION_TEMPLATE_NUMBER_HPP_ */
