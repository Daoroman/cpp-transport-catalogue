#pragma once

#include "geo.h"

#include <string>
#include <vector>
#include <set>
#include <unordered_map>

namespace transport_catalog {
	
		struct Stop {
			std::string name{};
			geo::Coordinates coordinates{};
			std::set<std::string> buses{};
		};

		struct Bus {
			std::string name{};
			std::vector<Stop*> stops{};
			bool circle{};
		};

		struct BusInfo {
			size_t stops = 0;
			size_t uniqe_stops = 0;
			int route_length;
			double courv = 0.0;
		};

		struct InputDataBus {
			std::string name;
			std::vector<std::string> stops;
			bool circle;
		};
}