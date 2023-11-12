#pragma once
#include "json.h"
#include "json_reader.h"
#include "transport_catalogue.h"
#include "map_renderer.h"


class RequestHandler {
public:
    
    RequestHandler(transport_catalog::TransportCatalogue& catalogue, const renderer::MapRenderer& renderer)
        : catalogue_(catalogue), renderer_(renderer)
    {
 
    }

    void RequestOut(const json::Node& requests) const;
  

    const json::Node PrintStop(const json::Dict& request_map) const; 
    const json::Node PrintBus(const json::Dict& request_map) const;
    const json::Node PrintMap(const json::Dict& request_map) const;

    svg::Document RenderMap() const;


   

private:

    transport_catalog::TransportCatalogue& catalogue_;
    const renderer::MapRenderer& renderer_;
};
