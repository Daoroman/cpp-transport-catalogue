#include "stat_reader.h"

namespace transport_catalog {


	void ProcessRequests(std::ostream& out, TransportCatalogue& catalog) {
		size_t requests;
		std::cin >> requests >> std::ws;
		for (size_t i = 0; i < requests; ++i) {
			std::string line, key;
			std::cin >> key >> std::ws;
			std::getline(std::cin, line);
			if (key == "Bus") {
				std::string bus_numder = line.substr(0, line.npos);
				if (catalog.FindBus(bus_numder)) {
					out << "Bus " << bus_numder << ": " << catalog.GetBusinfo(bus_numder).stops << " stops on route, "
						<< catalog.GetBusinfo(bus_numder).uniqe_stops << " unique stops, " << std::setprecision(6)
						<< catalog.GetBusinfo(bus_numder).route_length << " route length, "
						<< catalog.GetBusinfo(bus_numder).courv << " curvature\n";
				}
				else {
					out << "Bus " << bus_numder << ": not found\n";
				}
			}
			if (key == "Stop") {
				std::string stop_name = line.substr(0, line.npos);
				if (catalog.FindStop(stop_name)) {
					out << "Stop " << stop_name << ": ";
					std::set<std::string> buses = catalog.BusesOnStop(stop_name);
					if (!buses.empty()) {
						out << "buses";
						for (const auto& bus : buses) {
							out << " " << bus;
						}
						out << "\n";
					}
					else {
						out << "no buses\n";
					}
				}
				else {
					out << "Stop " << stop_name << ": not found\n";
				}
			}
		}
	}
}
