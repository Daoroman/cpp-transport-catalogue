#pragma once

#include "json.h"
#include "transport_catalogue.h"
#include "domain.h"
#include "map_renderer.h"

#include <iostream>

class JsonReader {
public: 
	JsonReader(std::istream& input)
		:data_(json::Load(input))
	{
	}
	const json::Node& GetBaseRequests() const;
    const json::Node& GetStatRequests() const;
	const json::Node& GetRenderSettings() const;

	void FillCatalogue(transport_catalog::TransportCatalogue& catalogue);
	 void FillDistance(transport_catalog::TransportCatalogue& catalogue);
	renderer::MapRenderer FillRenderSettings(const json::Dict& request_map) const;

private:
	json::Document data_;
	json::Node null_ = nullptr;

};