#include "request_handler.h"


void RequestHandler::RequestOut(const json::Node& requests) const {
	json::Array result; 
	for (auto& request: requests.AsArray()) {
		auto& request_map = request.AsMap();
		if (request_map.at("type").AsString() == "Stop") {
			result.push_back(PrintStop(request_map).AsMap());
		}
		if (request_map.at("type").AsString() == "Bus") {
			result.push_back(PrintBus(request_map).AsMap());
		}
		if (request_map.at("type").AsString() == "Map") {
			result.push_back(PrintMap(request_map).AsMap());
		}
	}
	json::Print(json::Document{ result }, std::cout);
}

const json::Node RequestHandler::PrintStop(const json::Dict& request_map) const {
	json::Dict result;
	std::string stop_name = request_map.at("name").AsString();
	result["request_id"] = request_map.at("id").AsInt();
	if (!catalogue_.FindStop(stop_name)) {
		result["error_message"] = json::Node{ static_cast<std::string>("not found") };
	}
	else {
		json::Array buses;
		for (auto& bus : catalogue_.BusesOnStop(stop_name)) {
			buses.push_back(bus);
		}
		result["buses"] = buses;
	}
	return json::Node { result };
}

const json::Node RequestHandler::PrintBus(const json::Dict& request_map) const {
	json::Dict result;
	std::string bus_name = request_map.at("name").AsString();
	result["request_id"] = request_map.at("id").AsInt();
	if (!catalogue_.FindBus(bus_name)) {
		result["error_message"] = json::Node{ static_cast<std::string>("not found") };
	}
	else {
		result["curvature"] = catalogue_.GetBusinfo(bus_name).courv;
		result["route_length"] = catalogue_.GetBusinfo(bus_name).route_length;
		result["stop_count"] = static_cast<int>(catalogue_.GetBusinfo(bus_name).stops);
		result["unique_stop_count"] = static_cast<int>(catalogue_.GetBusinfo(bus_name).uniqe_stops);
	}
	return json::Node{ result };
}

const json::Node RequestHandler::PrintMap(const json::Dict& request_map) const {
	json::Dict result;
	result["request_id"] = request_map.at("id").AsInt();
	std::ostringstream strm;
	svg::Document map = RenderMap();
	map.Render(strm);
	result["map"] = strm.str();

	return json::Node{ result };
}

svg::Document RequestHandler::RenderMap() const {
	return renderer_.SVG(catalogue_.SortAllBuses());
}
