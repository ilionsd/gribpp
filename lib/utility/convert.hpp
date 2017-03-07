/*
 * convert.hpp
 *
 *  Created on: Feb 26, 2017
 *      Author: ilion
 */

#ifndef _UTILITY_CONVERT_HPP_
#define _UTILITY_CONVERT_HPP_


namespace utility {

	template<typename TypeTo, typename TypeThrough, typename TypeFrom>
	constexpr TypeTo static_convert(TypeFrom val) {
		return static_cast<TypeTo>( static_cast<TypeThrough>(val) );
	};

};	//-- namespace utility --


#endif /* _UTILITY_CONVERT_HPP_ */
