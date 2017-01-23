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

	auto optRes0 = reading::try_read(mapping::make_section_map<mapping::grib_edition::V2, 0>, reader);
	mapping::section_map_vn<mapping::grib_edition::V2, 0> sectionMap0;
	if (optRes0)
		sectionMap0 = std::move(*optRes0);

	auto optRes1 = reading::try_read(mapping::make_section_map<mapping::grib_edition::V2, 1>, reader);
	mapping::section_map_vn<mapping::grib_edition::V2, 1> sectionMap1;
	if (optRes1)
		sectionMap1 = std::move(*optRes1);

	auto optRes2 = reading::try_read(mapping::make_section_map<mapping::grib_edition::V2, 2>, reader);
	mapping::section_map_vn<mapping::grib_edition::V2, 2> sectionMap2;
	if (optRes2)
		sectionMap2 = std::move(*optRes2);

	//system("PAUSE");
	return 0;
}
