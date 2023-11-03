#include "input_reader.h"

namespace transport_catalog {
	
	using namespace detail;

	void ReadData(std::istream& in, TransportCatalogue& catalog) {
		std::vector<input_detail::InputDataBus> all_buses;
		std::vector<Stop> stops;
		std::vector<Bus> buses;
		std::vector<std::pair<std::string, std::string>> stop_distanse;
		std::pair<std::string, std::string> stop_to_distanse;
		size_t requests;
		in >> requests >> std::ws;
		for (size_t i = 0; i < requests; ++i) {
			std::string line;
			std::getline(in, line);
			if (line[0] == 'S') {
				Stop stop;
				stop.name = line.substr(5, line.find_first_of(':') - line.find_first_of(' ') - 1);
				double lat = std::stod(line.substr(line.find_first_of(':') + 2, 9));
				double lng = std::stod(line.substr(line.find_first_of(',') + 2, 9));
				stop.coordinates.lat = lat;
				stop.coordinates.lng = lng;
				stops.push_back(stop);
				if (line.find("m to ") != line.npos) {
					std::string distanse = line.substr(line.find(',') + 2);
					stop_to_distanse = { stop.name, distanse };
					stop_distanse.push_back(stop_to_distanse);
				}

			}
			if (line[0] == 'B') {
				input_detail::InputDataBus bus;
				std::string stop_name;

				bus.circle = true;
				bus.name = line.substr(4, line.find_first_of(':') - line.find_first_of(' ') - 1);
				line.erase(0, line.find_first_of(':') + 2);
				auto pos = line.find('>') != line.npos ? '>' : '-';
				if (pos == '>') bus.circle = false;
				while (line.find(pos) != line.npos) {
					stop_name = line.substr(0, line.find_first_of(pos) - 1);
					bus.stops.push_back(stop_name);
					line.erase(0, line.find_first_of(pos) + 2);
				}

				stop_name = line.substr(0, line.npos - 1);
				bus.stops.push_back(stop_name);
				all_buses.push_back(bus);

			}
		}
		for (const auto& stop : stops) {
			catalog.AddStop(stop);
		}

		for (const auto bus : all_buses) {
			Bus new_bus;
			new_bus.name = std::move(bus.name);
			new_bus.circle = bus.circle;
			for (const auto stop : bus.stops) {
				new_bus.stops.push_back(std::move(catalog.FindStop(stop)));
			}
			buses.push_back(new_bus);
		}

		for (const auto& bus : buses) {
			catalog.AddBus(bus);
		}

		for (auto& stop_ : stop_distanse) {
			FillDistance(stop_, catalog);
		}

	}

	void FillDistance(std::pair<std::string, std::string>& stop, TransportCatalogue& catalog) {
		std::string line = std::move(stop.second);
		line.erase(0, line.find(',') + 1);
		while (!line.empty()) {
			std::pair<Stop*, Stop*> stoper;
			int distanse = 0;
			distanse = std::stoi(line);
			line.erase(0, line.find("m to ") + 5);
			if (line.find("m to ") == line.npos) {
				std::string stop_to_name = line.substr(0, line.npos - 1);
				stoper = { catalog.FindStop(stop.first), catalog.FindStop(stop_to_name) };
				catalog.AddDistance(stoper, distanse);
				line.clear();
			}
			else {
				std::string stop_to_name = line.substr(0, line.find_first_of(','));
				stoper = { catalog.FindStop(stop.first), catalog.FindStop(stop_to_name) };
				catalog.AddDistance(stoper, distanse);
				line.erase(0, line.find_first_of(',') + 2);
			}
		}
	}
}
