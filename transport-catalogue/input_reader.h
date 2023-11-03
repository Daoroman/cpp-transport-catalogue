#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <iterator>

#include "geo.h"
#include "transport_catalogue.h"

namespace transport_catalog {
	
	namespace input_detail {

		struct InputDataBus {
			std::string name;
			std::vector<std::string> stops;
			bool circle;
		};

	}
	
	using namespace detail;

	void ReadData(std::istream& in, TransportCatalogue& catalog);
	void FillDistance(std::pair<std::string, std::string>& stop, TransportCatalogue& catalog);
}
