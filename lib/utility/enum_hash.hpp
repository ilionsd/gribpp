/*
 * enum_hash.hpp
 *
 *  Created on: Jan 17, 2017
 *      Author: ilion
 */

#ifndef _UTILITY_ENUM_HASH_HPP_
#define _UTILITY_ENUM_HASH_HPP_

#include <bits/functional_hash.h>
#include <type_traits>

namespace utility {

	template<typename EnumType>
	struct enum_hash :
		public std::hash<typename std::underlying_type<EnumType>::type>
	{
		using enum_type = EnumType;
		using enum_underlying_type = typename std::underlying_type<enum_type>::type;
		using base_type = std::hash<enum_underlying_type>;

		std::size_t
		operator() (EnumType val) const noexcept {
			return base_type::operator() (static_cast<enum_underlying_type>(val));
		};
	};	//-- struct enum_hash --

};	//-- namespace utility --



#endif /* _UTILITY_ENUM_HASH_HPP_ */
