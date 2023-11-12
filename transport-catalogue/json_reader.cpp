#include "json_reader.h"


const json::Node& JsonReader::GetBaseRequests() const {
	if (!data_.GetRoot().AsMap().count("base_requests")) {
		return null_;
	}

	return data_.GetRoot().AsMap().at("base_requests");
}

const json::Node& JsonReader::GetStatRequests() const {
	if (!data_.GetRoot().AsMap().count("stat_requests")) {
		return null_;
	}
	return data_.GetRoot().AsMap().at("stat_requests");
}

const json::Node& JsonReader::GetRenderSettings() const {
	if (!data_.GetRoot().AsMap().count("render_settings")) {
		return null_;
	}
	return data_.GetRoot().AsMap().at("render_settings");
}


void JsonReader::FillCatalogue(transport_catalog::TransportCatalogue& catalogue) {
	std::vector<transport_catalog::InputDataBus> input_buses;
	std::vector<transport_catalog::Stop> all_stops;
	std::vector<transport_catalog::Bus> all_buses;

	json::Array result = GetBaseRequests().AsArray();
	
	for (auto& stops : result) {
		auto& requests_map = stops.AsMap();
		auto& type = requests_map.at("type").AsString();
		if (type == "Stop") {
			transport_catalog::Stop stop;
			stop.name = requests_map.at("name").AsString();
			geo::Coordinates coor = { requests_map.at("latitude").AsDouble(), requests_map.at("longitude").AsDouble() };
			stop.coordinates = coor;
			all_stops.push_back(stop);
		}
		if (type == "Bus") {
			transport_catalog::InputDataBus bus;
			bus.name = requests_map.at("name").AsString();
			bus.circle = requests_map.at("is_roundtrip").AsBool();
			for (auto& stop : requests_map.at("stops").AsArray()) {
				bus.stops.push_back(stop.AsString());
			}
			input_buses.push_back(std::move(bus));
		}
	}

		for (const auto& stop : all_stops) {
			catalogue.AddStop(stop);
		}

		for (const auto bus : input_buses) {
			transport_catalog::Bus new_bus;
			new_bus.name = std::move(bus.name);
			new_bus.circle = bus.circle;
			for (const auto stop : bus.stops) {
				new_bus.stops.push_back(std::move(catalogue.FindStop(stop)));
			}
			all_buses.push_back(new_bus);
		}

		for (const auto& bus : all_buses) {
			catalogue.AddBus(bus);
		}

		FillDistance(catalogue);

}

void JsonReader::FillDistance(transport_catalog::TransportCatalogue& catalogue) {
	json::Array result = GetBaseRequests().AsArray();
	for (auto& stops : result) {
		auto& requests_map = stops.AsMap();
		auto& type = requests_map.at("type").AsString();
		if (type == "Stop") {
			for (auto& distance : requests_map.at("road_distances").AsMap()) {
				auto stop_1 = catalogue.FindStop(requests_map.at("name").AsString());
				auto stop_2 = catalogue.FindStop(distance.first);
				catalogue.AddDistance({ stop_1, stop_2 }, distance.second.AsInt());
			}
		}
	}

}

renderer::MapRenderer JsonReader::FillRenderSettings(const json::Dict& request_map) const {
	
	renderer::RenderSettings render_settings;

	render_settings.width = request_map.at("width").AsDouble();
	render_settings.height = request_map.at("height").AsDouble();
	render_settings.padding = request_map.at("padding").AsDouble();
	render_settings.stop_radius = request_map.at("stop_radius").AsDouble();
	render_settings.line_width = request_map.at("line_width").AsDouble();
	render_settings.bus_label_font_size = request_map.at("bus_label_font_size").AsInt();
	const json::Array& bus_label_offset = request_map.at("bus_label_offset").AsArray();
	render_settings.bus_label_offset = { bus_label_offset[0].AsDouble(), bus_label_offset[1].AsDouble() };
	render_settings.stop_label_font_size = request_map.at("stop_label_font_size").AsInt();
	const json::Array& stop_label_offset = request_map.at("stop_label_offset").AsArray();
	render_settings.stop_label_offset = { stop_label_offset[0].AsDouble(), stop_label_offset[1].AsDouble() };
	render_settings.underlayer_width = request_map.at("underlayer_width").AsDouble();

	if (request_map.at("underlayer_color").IsString()) {
		render_settings.underlayer_color = request_map.at("underlayer_color").AsString();
	}
	else if (request_map.at("underlayer_color").IsArray()) {
		auto& color = request_map.at("underlayer_color").AsArray();
		if (color.size() == 3) {
			render_settings.underlayer_color = svg::Rgb(color[0].AsInt(), color[1].AsInt(), color[2].AsInt());
		}
		else if (color.size() == 4) {
			render_settings.underlayer_color = svg::Rgba(color[0].AsInt(), color[1].AsInt(), color[2].AsInt(), color[3].AsDouble());
		}
	}


	auto& color_palette = request_map.at("color_palette").AsArray();
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
		}
	}
	return render_settings;
}
	 
