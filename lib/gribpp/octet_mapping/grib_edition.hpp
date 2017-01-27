/*
 * grib_edition.hpp
 *
 *  Created on: Jan 26, 2017
 *      Author: ilion
 */

#ifndef _GRIBPP_GRIB_EDITION_HPP_
#define _GRIBPP_GRIB_EDITION_HPP_

#include <cstdint>

namespace gribpp {
	namespace octet_mapping {

		enum class grib_edition : std::uint8_t {
			V1 = 1,
			V2 = 2
		};

	};
};

#endif /* _GRIBPP_GRIB_EDITION_HPP_ */
