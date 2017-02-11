/*
 * byte_reverse.hpp
 *
 *  Created on: Feb 9, 2017
 *      Author: ilion
 */

#ifndef _UTILITY_BYTE_REVERSE_HPP_
#define _UTILITY_BYTE_REVERSE_HPP_

#include <cstdint>

namespace utility {

	template<typename T>
	T byte_reverse(register T x);

	template<>
	inline std::uint8_t byte_reverse(register std::uint8_t x) {
		return x;
	}

	template<>
	inline std::uint16_t byte_reverse(register std::uint16_t x) {
		return (x >> 8) | (x << 8);
	}

	template<>
	inline std::uint32_t byte_reverse(register std::uint32_t x) {
		return (x >> 24) | ((x & 0x00FF0000u) >> 8) | ((x & 0x0000FF00u) << 8) | (x << 24);
	}

	template<>
	inline std::uint64_t byte_reverse(register std::uint64_t x) {
		return (x >> 56) | ((x & 0x00FF000000000000ul) >> 40) | ((x & 0x0000FF0000000000ul) >> 24) | ((x & 0x000000FF00000000ul) >> 8) |
				((x & 0x00000000FF000000ul) << 8) | ((x & 0x0000000000FF0000ul) << 24) | ((x & 0x000000000000FF00ul) << 40) | ((x & 0x00000000000000FFul) << 56);
	}

};

#endif /* _UTILITY_BYTE_REVERSE_HPP_ */
