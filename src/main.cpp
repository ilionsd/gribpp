#include <cstdlib>
#include <iostream>
#include <memory>
#include <functional>
#include <type_traits>


#include "../include/gribpp_mapping.hpp"
#include "../lib/gribpp/type_traits.hpp"

namespace reading = gribpp::reader;
namespace mapping = gribpp::octet_mapping;

int main() {
	const char *dataFileName = "data/grib2/RDPA - CaPA/CMC_RDPA_APCP-006-0100cutoff_SFC_0_ps10km_2016110106_000.grib2";
	auto reader = reading::open_grib(dataFileName);
		
	auto readerF = [](reading::octet_reader& r) { mapping::section_map_vn<mapping::grib_edition::V2, 0>::make_map(r); };

	auto optRes0 = reading::try_read(mapping::section_map_vn<mapping::grib_edition::V2, 0>::make_map, reader);

	/*
	mapping::section_map_n<0> sectionMap0;
	if (optRes0)
		sectionMap0 = optRes0.value();
	*/

	//system("PAUSE");
	return 0;
}
