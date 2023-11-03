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

#include "geo.h"

namespace transport_catalog {
	
	namespace detail {
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
			double route_length;
			double courv = 0.0;
		};

		struct StopHash {
			size_t operator() (const std::pair<Stop*, Stop*>& stops) const
			{
				size_t hash = std::hash<const void*>{}(stops.first);
				hash = hash * 37 + std::hash<const void*>{}(stops.second);
				return hash;
			}
		};

	}
	class TransportCatalogue
		
	{
	public:

		void AddStop(const detail::Stop& stop);
		void AddBus(const detail::Bus& bus);
		void AddDistance(const std::pair<detail::Stop*, detail::Stop*>& stops, size_t distance);
		detail::Stop* FindStop(const std::string& stopname);
		detail::Bus* FindBus(const std::string& busname);
		detail::BusInfo GetBusinfo(const std::string& busname);
		size_t ComputeFactDistance(const std::pair<detail::Stop*, detail::Stop*>& couple_stops);
		const std::set<std::string> BusesOnStop(const std::string& stop_name) const;



	private:
		std::deque<detail::Bus> buses_;
		std::deque<detail::Stop> stops_;
		std::unordered_map<std::string_view, detail::Stop*> stopname_to_stop_;
		std::unordered_map<std::string_view, detail::Bus*> busname_to_bus_;
		std::unordered_map<std::pair<detail::Stop*, detail::Stop*>, size_t, detail::StopHash> stop_distance_;
	};

}
