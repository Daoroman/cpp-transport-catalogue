#pragma once

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include<unordered_set>
#include <vector>
#include <deque>
#include <stdexcept>
#include <optional>
#include <set>
#include <map>

#include "geo.h"
#include "domain.h"

namespace transport_catalog {

	class TransportCatalogue

	{
	public:

		struct StopHash {
			size_t operator() (const std::pair<Stop*, Stop*>& stops) const
			{
				size_t hash = std::hash<const void*>{}(stops.first);
				hash = hash * 37 + std::hash<const void*>{}(stops.second);
				return hash;
			}
		};

		void AddStop(const Stop& stop);
		void AddBus(const Bus& bus);
		void AddDistance(const std::pair<Stop*, Stop*>& stops, size_t distance);
		Stop* FindStop(const std::string_view stopname);
		Bus* FindBus(const std::string_view busname);
		BusInfo GetBusinfo(const std::string_view busname);
		size_t ComputeFactDistance(const std::pair<Stop*, Stop*>& couple_stops);
		const std::set<std::string> BusesOnStop(const std::string_view stop_name) const;
		const std::map<std::string_view, const Bus*> SortAllBuses() const;


	private:
		std::deque<Bus> buses_;
		std::deque<Stop> stops_;
		std::unordered_map<std::string_view, Stop*> stopname_to_stop_;
		std::unordered_map<std::string_view, Bus*> busname_to_bus_;
		std::unordered_map<std::pair<Stop*, Stop*>, size_t, StopHash> stop_distance_;
	};

} //namespace transport_catalog