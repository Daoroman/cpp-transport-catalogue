#pragma once
#include "json.h"
#include "transport_catalogue.h"
#include "domain.h"
#include "map_renderer.h"
#include "request_handler.h"

#include <iostream>

struct RequestsType
{
	json::Node base_requests_{};
	json::Node stat_requests_{};
	json::Node render_settings_{};
};

class JsonReader {
public:
	JsonReader(std::istream& input)
		:data_(json::Load(input))
	{
	}

	void FillRequests();

   const json::Node& GetBaseRequests() const;
   const json::Node& GetStatRequests() const;
   const json::Node& GetRenderSettings() const;

	void FillCatalogue(transport_catalog::TransportCatalogue& catalogue);
	void FillDistance(transport_catalog::TransportCatalogue& catalogue);
	renderer::MapRenderer FillRenderSettings(const json::Dict& request_map) const;

	void RequestOut(const json::Node& requests, RequestHandler& request) const;



	const json::Node PrintBus(const json::Dict& request_map, RequestHandler& rh) const;
	const json::Node PrintStop(const json::Dict& request_map, RequestHandler& rh) const;
	const json::Node PrintMap(const json::Dict& request_map, RequestHandler& rh) const;

	

private:
	json::Document data_;
	json::Node null_ = nullptr;
	RequestsType requests_;

	transport_catalog::Stop FillStop(json::Node& request) const;
	transport_catalog::InputDataBus FillDataBus(json::Node& request) const;
	void FillBus(std::vector<transport_catalog::InputDataBus> input_buses, transport_catalog::TransportCatalogue& catalogue);
	renderer::RenderSettings FillUnderColor(const json::Node& value) const;
	renderer::RenderSettings FillColor(const json::Node& value) const;

};

