#include "json_reader.h"


void JsonReader::FillRequests(){

	for (const auto& [type_request, data] : data_.GetRoot().AsMap()) {
		if (type_request == "base_requests") {
			requests_.base_requests_ = data;
		}
		else if (type_request == "stat_requests") {
			requests_.stat_requests_ = data;
		}
		else if (type_request == "render_settings") {
			requests_.render_settings_ = data;
		}
		else {
			throw json::ParsingError("Wrong Type Request");
		}
	}
}

const json::Node& JsonReader::GetBaseRequests() const {
    return requests_.base_requests_;
}

const json::Node& JsonReader::GetStatRequests() const {
	return requests_.stat_requests_;
}

const json::Node& JsonReader::GetRenderSettings() const {
	return requests_.render_settings_;
}


void JsonReader::FillCatalogue(transport_catalog::TransportCatalogue& catalogue ) {
	std::vector<transport_catalog::InputDataBus> input_buses;
	json::Array requests = GetBaseRequests().AsArray();
	for (auto& request : requests) {
		auto& requests_map = request.AsMap();
		auto& type = requests_map.at("type").AsString();
		if (type == "Stop") {
			catalogue.AddStop(FillStop(request));
		}
		if (type == "Bus") {
			input_buses.push_back(FillDataBus(request));
		}
	}
	FillBus(input_buses, catalogue);
	FillDistance(catalogue);
}

transport_catalog::Stop JsonReader::FillStop(json::Node& request) const {
	auto& requests_map = request.AsMap();
	transport_catalog::Stop stop;
	stop.name = requests_map.at("name").AsString();
	geo::Coordinates coor = { requests_map.at("latitude").AsDouble(), requests_map.at("longitude").AsDouble() };
	stop.coordinates = coor;
	return stop;
}

transport_catalog::InputDataBus JsonReader::FillDataBus(json::Node& request) const {
	auto& requests_map = request.AsMap();
	transport_catalog::InputDataBus bus;
	bus.name = requests_map.at("name").AsString();
	bus.circle = requests_map.at("is_roundtrip").AsBool();
	for (auto& stop : requests_map.at("stops").AsArray()) {
		bus.stops.push_back(stop.AsString());
	}
	return bus;
}

void JsonReader::FillBus(std::vector<transport_catalog::InputDataBus> input_buses, transport_catalog::TransportCatalogue& catalogue) {
	for (const auto& bus : input_buses) {
		transport_catalog::Bus new_bus;
		new_bus.name = bus.name;
		new_bus.circle = bus.circle;
		for (const auto stop : bus.stops) {
			new_bus.stops.push_back(std::move(catalogue.FindStop(stop)));
		}
		catalogue.AddBus(new_bus);
	}
}

void JsonReader::FillDistance(transport_catalog::TransportCatalogue& catalogue ) {
	json::Array result = GetBaseRequests().AsArray();
	for (auto& stops : result) {
		auto& requests_map = stops.AsMap();
		auto& type = requests_map.at("type").AsString();
		if (type == "Stop") {
			for (auto& distance : requests_map.at("road_distances").AsMap()) {
				auto stop_from = catalogue.FindStop(requests_map.at("name").AsString());
				auto stop_to = catalogue.FindStop(distance.first);
				catalogue.AddDistance({ stop_from, stop_to }, distance.second.AsInt());
			}
		}
	}

}

renderer::MapRenderer JsonReader::FillRenderSettings(const json::Dict& request_map) const {
   renderer::RenderSettings render_settings;
	for (const auto& [key_type, value] : request_map) {
		if (key_type == "width") {
			render_settings.width = value.AsDouble();
		}
		else if (key_type == "height") {
			render_settings.height = value.AsDouble();
		}
		else if (key_type == "padding") {
			render_settings.padding = value.AsDouble();
		}
		else if (key_type == "stop_radius") {
			render_settings.stop_radius = value.AsDouble();
		}
		else if (key_type == "line_width") {
			render_settings.line_width = value.AsDouble();
		}
		else if (key_type == "bus_label_font_size") {
			render_settings.bus_label_font_size = value.AsInt();
		}
		else if (key_type == "bus_label_offset") {
			const json::Array& bus_label_offset = value.AsArray();
			render_settings.bus_label_offset = { bus_label_offset[0].AsDouble(), bus_label_offset[1].AsDouble() };
		}
		else if (key_type == "stop_label_font_size") {
			render_settings.stop_label_font_size = value.AsInt();
		}
		else if (key_type == "stop_label_offset") {
			const json::Array& stop_label_offset = value.AsArray();
			render_settings.stop_label_offset = { stop_label_offset[0].AsDouble(), stop_label_offset[1].AsDouble() };
		}
		else if (key_type == "underlayer_width") {
			render_settings.underlayer_width = value.AsDouble();
		}
		else if (key_type == "underlayer_color") {
			render_settings.underlayer_color = FillUnderColor(value).underlayer_color;
		}
		else if (key_type == "color_palette") {
			render_settings.color_palette = FillColor(value).color_palette;
		}
		else {
				throw json::ParsingError("Wrong Color");
		}

	}

	return render_settings;
}

renderer::RenderSettings JsonReader::FillUnderColor(const json::Node& value) const {
	renderer::RenderSettings render_settings;

	if (value.IsString()) {
		render_settings.underlayer_color = value.AsString();
	}
	else if (value.IsArray()) {
		auto& color = value.AsArray();
		if (color.size() == 3) {
			render_settings.underlayer_color = svg::Rgb(color[0].AsInt(), color[1].AsInt(), color[2].AsInt());
		}
		else if (color.size() == 4) {
			render_settings.underlayer_color = svg::Rgba(color[0].AsInt(), color[1].AsInt(), color[2].AsInt(), color[3].AsDouble());
		}
		else {
			throw json::ParsingError("wrong underlayer color type"); 
		}
	} 
	else {
		throw json::ParsingError("wrong underlayer color");
	}
	return render_settings;
}

renderer::RenderSettings JsonReader::FillColor(const json::Node& value) const{
	renderer::RenderSettings render_settings;
	auto& color_palette =value.AsArray();
	for (const auto& elem : color_palette) {
		if (elem.IsString()) {
			render_settings.color_palette.push_back(elem.AsString());
		}
		else if (elem.IsArray()) {
			const json::Array& color = elem.AsArray();
			if (color.size() == 3) {
				render_settings.color_palette.push_back(svg::Rgb(color[0].AsInt(), color[1].AsInt(), color[2].AsInt()));
			}
			else if (color.size() == 4) {
				render_settings.color_palette.push_back(svg::Rgba(color[0].AsInt(), color[1].AsInt(), color[2].AsInt(), color[3].AsDouble()));
			}
			else {
				throw json::ParsingError("wrong color palette type");
			}
		} 
		else {
			throw json::ParsingError("wrong color palette");
		}
	}

	return render_settings;
}

void JsonReader::RequestOut(const json::Node& requests, RequestHandler& requesthandler) const {
	json::Array result;
	for (auto& request : requests.AsArray()) {
		auto& request_map = request.AsMap();
		if (request_map.at("type").AsString() == "Stop") {
			result.push_back(PrintStop(request_map, requesthandler).AsMap());
		}
		if (request_map.at("type").AsString() == "Bus") {
			result.push_back(PrintBus(request_map, requesthandler).AsMap());
		}
		if (request_map.at("type").AsString() == "Map") {
			result.push_back(PrintMap(request_map, requesthandler).AsMap());
		}
	}
	json::Print(json::Document{ result }, std::cout);
}

const json::Node JsonReader::PrintBus(const json::Dict& request_map, RequestHandler& requesthandler) const {
	json::Dict result;
	const std::string& route_number = request_map.at("name").AsString();
	result["request_id"] = request_map.at("id").AsInt();
	if (!requesthandler.IsBusNumber(route_number)) {
		result["error_message"] = json::Node{ static_cast<std::string>("not found") };
	}
	else {
		result["curvature"] = requesthandler.GetBusStat(route_number).courv;
		result["route_length"] = requesthandler.GetBusStat(route_number).route_length;
		result["stop_count"] = static_cast<int>(requesthandler.GetBusStat(route_number).stops);
		result["unique_stop_count"] = static_cast<int>(requesthandler.GetBusStat(route_number).uniqe_stops);
	}

	return json::Node{ result };
}

const json::Node JsonReader::PrintStop(const json::Dict& request_map, RequestHandler& requesthandler) const {
	json::Dict result;
	const std::string& stop_name = request_map.at("name").AsString();
	result["request_id"] = request_map.at("id").AsInt();
	if (!requesthandler.IsStopName(stop_name)) {
		result["error_message"] = json::Node{ static_cast<std::string>("not found") };
	}
	else {
		json::Array buses;
		for (auto& bus : requesthandler.GetBusesByStop(stop_name)) {
			buses.push_back(bus);
		}
		result["buses"] = buses;
	}
	return json::Node{ result };
}

const json::Node JsonReader::PrintMap(const json::Dict& request_map, RequestHandler& requesthandler) const {
	json::Dict result;
	result["request_id"] = request_map.at("id").AsInt();
	std::ostringstream strm;
	svg::Document map = requesthandler.RenderMap();
	map.Render(strm);
	result["map"] = strm.str();

	return json::Node{ result };
}

