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


			enum class number_domain : uint16_t {
				latitude_longitude								= (uint16_t)number_base::latitude_longitude + 0,
				latitude_longitude_rotated						= (uint16_t)number_base::latitude_longitude + 1,
				latitude_longitude_stretched					= (uint16_t)number_base::latitude_longitude + 2,
				latitude_longitude_stretched_and_rotated		= (uint16_t)number_base::latitude_longitude + 3,

				mercator										= (uint16_t)number_base::mercator,

				polar_stereographic_projection					= (uint16_t)number_base::polar_stereographic_projection,

				lambert_conformal								= (uint16_t)number_base::lambert_conformal,

				albert_equal_area								= (uint16_t)number_base::albert_equal_area,

				gaussian_latitude_longitude						= (uint16_t)number_base::gaussian_latitude_longitude + 0,
				gaussian_latitude_longitude_rotated				= (uint16_t)number_base::gaussian_latitude_longitude + 1,
				gaussian_latitude_longitude_stretched			= (uint16_t)number_base::gaussian_latitude_longitude + 2,
				gaussian_latitude_longitude_stretched_and_rotated	= (uint16_t)number_base::gaussian_latitude_longitude + 3,

				shperical_harmonic_coefficients					= (uint16_t)number_base::shperical_harmonic_coefficients + 0,
				shperical_harmonic_coefficients_rotated			= (uint16_t)number_base::shperical_harmonic_coefficients + 1,
				shperical_harmonic_coefficients_stretched		= (uint16_t)number_base::shperical_harmonic_coefficients + 2,
				shperical_harmonic_coefficients_stretched_and_rotated = (uint16_t)number_base::shperical_harmonic_coefficients + 3,

				space_view_perspective							= (uint16_t)number_base::space_view_perspective,

				triangilar_grid_based_on_icosahedron			= (uint16_t)number_base::triangilar_grid_based_on_icosahedron,

				equatorial_azimuthal_equidestant_projection		= (uint16_t)number_base::equatorial_azimuthal_equidestant_projection,

				azimuth_range_projection						= (uint16_t)number_base::azimuth_range_projection,

				cross_section_grid								= (uint16_t)number_base::cross_section_grid,

				hovmjoller_diagram_grid							= (uint16_t)number_base::hovmjoller_diagram_grid,

				time_section_grid								= (uint16_t)number_base::time_section_grid
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

			constexpr auto to_number_info(const number_domain val)	-> std::tuple<std::tuple<number_base, bool, bool>, deformation>;
			constexpr auto to_number_info(const number_base val)	-> std::tuple<std::tuple<number_base, bool, bool>, deformation>;
			constexpr auto to_number_info(const uint16_t val)		-> std::tuple<std::tuple<number_base, bool, bool>, deformation>;

			constexpr number to_number(const number_domain val);
			constexpr number to_number(const number_base val);
			constexpr number to_number(const uint16_t val);


			class number {
			public:
				static constexpr std::array<uint16_t, 12> deformable_templates { 0, 1, 2, 3, 40, 41, 42, 43, 50, 51, 52, 53 };
				static constexpr std::array<uint16_t, 3> variadic_templates { 120, 1000, 1200 };

				friend constexpr number to_number(const number_domain val);

				constexpr explicit number(const number_domain n) :
					number(to_number_info(n))
				{};
				constexpr explicit number(const number_base n) :
					number(to_number_info(n))
				{};
				constexpr explicit number(const uint16_t val) :
					number(to_number_info(val))
				{};

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
					return { mNumberInfo, is_deformable() ? flags() | rhs : flags() };
				};
				constexpr number operator & (const deformation rhs) const {
					return { mNumberInfo, is_deformable() ? flags() & rhs : flags() };
				};


			private:
				constexpr number(const std::tuple<std::tuple<number_base, bool, bool>, deformation>& numberInfoAndFlags) :
					mNumberInfo { std::get<0>(numberInfoAndFlags) },
					mDeformationFlags { std::get<1>(numberInfoAndFlags) }
				{};
				constexpr number(const std::tuple<number_base, bool, bool>& numberInfo, deformation deformationFlags) :
					mNumberInfo { numberInfo },
					mDeformationFlags { deformationFlags }
				{};

				const std::tuple<number_base, bool, bool> mNumberInfo;
				deformation mDeformationFlags;
			};

			constexpr std::array<uint16_t, 12> number::deformable_templates;
			constexpr std::array<uint16_t, 3> number::variadic_templates;


			constexpr auto to_number_info(const number_domain n) -> std::tuple<std::tuple<number_base, bool, bool>, deformation> {
				uint16_t val = static_cast<uint16_t>(n);

				namespace fn = utility::fn;
				auto map 	= fn::make_apply( fn::make_map( fn::unary<std::equal_to<decltype(val)>>{ val } ) );
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

				auto numberInfo = std::make_tuple(static_cast<number_base>(base), isDeformable, isVariadic);
				return std::make_tuple(numberInfo, flags);
			};

			constexpr auto to_number_info(const number_base n) -> std::tuple<std::tuple<number_base, bool, bool>, deformation> {
				return to_number_info(static_cast<uint16_t>(n));
			};

			constexpr auto to_number_info(const uint16_t val) -> std::tuple<std::tuple<number_base, bool, bool>, deformation> {
				return to_number_info(static_cast<number_domain>(val));
			};


			constexpr number to_number(const number_domain n) {
				return { to_number_info(n) };
			};

			constexpr number to_number(const number_base n) {
				return to_number( static_cast<uint16_t>(n) );
			};

			constexpr number to_number(const uint16_t val) {
				return to_number( static_cast<number_domain>(val) );
			};


			constexpr number operator | (const number_base n, const deformation x) {
				return to_number(static_cast<number_domain>(n)) | x;
			};
			constexpr number operator & (const number_base n, const deformation x) {
				return to_number(static_cast<number_domain>(n)) & x;
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
