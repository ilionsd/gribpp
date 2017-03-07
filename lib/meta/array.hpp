/*
 * array.hpp
 *
 *  Created on: Feb 28, 2017
 *      Author: ilion
 */

#ifndef _META_ARRAY_HPP_
#define _META_ARRAY_HPP_

#include <cstddef>

namespace meta {

	template<typename T, T... Values>
	struct array {
		using value_type = T;
		constexpr static std::size_t size { sizeof...(Values) };
		constexpr static value_type data[size] { Values... };
	};

};



#endif /* _META_ARRAY_HPP_ */
