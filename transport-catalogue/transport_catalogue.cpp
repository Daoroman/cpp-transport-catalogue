#include "transport_catalogue.h"

namespace transport_catalog {

	void TransportCatalogue::AddStop(const detail::Stop& stop) {
		stops_.push_back(stop);
		stopname_to_stop_[stops_.back().name] = &stops_.back();
	}

	void TransportCatalogue::AddBus(const detail::Bus& bus) {
		buses_.push_back(bus);
		for (const auto& b : bus.stops) {
			for (auto& s : stops_) {
				if (b->name == s.name) {
					s.buses.insert(bus.name);
				}
			}
		}

		busname_to_bus_[buses_.back().name] = &buses_.back();
	}

	void TransportCatalogue::AddDistance(const std::pair<detail::Stop*, detail::Stop*>& stops, size_t distance) {
		stop_distance_[stops] = distance;
	}

	detail::Stop* TransportCatalogue::FindStop(const std::string& stopname) {
		if (!stopname_to_stop_.count(stopname)) {
			return nullptr;
		}
		return stopname_to_stop_.at(stopname);
	}

	detail::Bus* TransportCatalogue::FindBus(const std::string& busname) {
		if (!busname_to_bus_.count(busname)) {
			return nullptr;
		}
		return busname_to_bus_.at(busname);
	}
	size_t TransportCatalogue::ComputeFactDistance(const std::pair<detail::Stop*, detail::Stop*>& couple_stops) {
		if (!stop_distance_.count(couple_stops)) {
			return stop_distance_.at({ couple_stops.second, couple_stops.first });
		}
		return stop_distance_.at(couple_stops);

	}

	detail::BusInfo TransportCatalogue::GetBusinfo(const std::string& busname) {
		detail::BusInfo result{};
		detail::Bus* bus = FindBus(busname);
		if (!bus) {
			throw std::invalid_argument("bus not found");
		}
		if (bus->circle) {
			result.stops = bus->stops.size() * 2 - 1;
		}
		else {
			result.stops = bus->stops.size();
		}

		double fact_length = 0;
		double geo_length = 0.0;
		for (int i = 0; i < bus->stops.size() - 1; ++i) {
			auto stop_one = bus->stops[i];
			auto stop_two = bus->stops[i + 1];
			if (bus->circle) {
				geo_length += geo::ComputeDistance(stop_one->coordinates, stop_two->coordinates) * 2;
				fact_length += ComputeFactDistance({ stop_one, stop_two }) + ComputeFactDistance({ stop_two, stop_one });

			}
			else {
				geo_length += geo::ComputeDistance(stop_one->coordinates, stop_two->coordinates);
				fact_length += ComputeFactDistance({ stop_one, stop_two });
			}

		}
		result.route_length = fact_length;
		result.courv = fact_length / geo_length;

		std::unordered_set<std::string> unique_stops;
		for (const auto& stop : busname_to_bus_.at(busname)->stops) {
			unique_stops.insert(stop->name);
		}

		result.uniqe_stops = unique_stops.size();

		return result;
	}

	const std::set<std::string> TransportCatalogue::BusesOnStop(const std::string& stop_name) const {
		return stopname_to_stop_.at(stop_name)->buses;
	}
}
