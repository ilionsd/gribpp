#ifndef _GRIBPP_OCTET_MAPPING_GRIB_ERROR_HPP_
#define _GRIBPP_OCTET_MAPPING_GRIB_ERROR_HPP_

#include <exception>

namespace gribpp {
	namespace octet_mapping {

		class not_grib_error : public std::exception {
		public:
			not_grib_error(const char *message) :
				exception(message)
			{};

			not_grib_error(const not_grib_error& other) :
				exception(other)
			{};

		private:

		};

	};	//-- namespace octet_mapping --
};	//-- namespace gribpp --

#endif	//-- _GRIBPP_OCTET_MAPPING_GRIB_ERROR_HPP_ --