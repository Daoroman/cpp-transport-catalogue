#pragma once

#include "json.h"
#include "transport_catalogue.h"
#include "map_renderer.h"

#include <sstream>


    class RequestHandler {
    public:

        RequestHandler(transport_catalog::TransportCatalogue& catalogue, const renderer::MapRenderer& renderer)
            : catalogue_(catalogue), renderer_(renderer)
        {

        }

        transport_catalog::BusInfo GetBusStat(const std::string_view bus_number) const;
        const std::set<std::string> GetBusesByStop(std::string_view stop_name) const;
        bool IsBusNumber(const std::string_view bus_number) const;
        bool IsStopName(const std::string_view stop_name) const;

        svg::Document RenderMap() const;

    private:

        transport_catalog::TransportCatalogue& catalogue_;
        const renderer::MapRenderer& renderer_;
    };

