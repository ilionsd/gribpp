/*
 * bit_reverse.hpp
 *
 *  Created on: Feb 9, 2017
 *      Author: ilion
 */

#ifndef _UTILITY_BIT_REVERSE_HPP_
#define _UTILITY_BIT_REVERSE_HPP_

#include <cstdint>

namespace utility {

	template<typename T>
	T bit_reverse(register T val);

	template<>
	std::uint8_t bit_reverse(register std::uint8_t x)
	{
		x = ((x & 0xAA) >> 1) | ((x & 0x55) << 1);
		x = ((x & 0xCC) >> 2) | ((x & 0x33) << 2);
		return (x >> 4) | (x << 4);
	};

	template<>
	std::uint16_t bit_reverse(register std::uint16_t x)
	{
		x = ((x & 0xAAAA) >> 1) | ((x & 0x5555) << 1);
		x = ((x & 0xCCCC) >> 2) | ((x & 0x3333) << 2);
		x = ((x & 0xF0F0) >> 4) | ((x & 0x0F0F) << 4);
		return (x >> 8) | (x << 8);
	};

	template<>
	std::uint32_t bit_reverse(register std::uint32_t x)
	{
		x = (((x & 0xAAAAAAAA) >> 1) | ((x & 0x55555555) << 1));
		x = (((x & 0xCCCCCCCC) >> 2) | ((x & 0x33333333) << 2));
		x = (((x & 0xF0F0F0F0) >> 4) | ((x & 0x0F0F0F0F) << 4));
		x = (((x & 0xFF00FF00) >> 8) | ((x & 0x00FF00FF) << 8));
		return((x >> 16) | (x << 16));
	};

};

#endif /* _UTILITY_BIT_REVERSE_HPP_ */
