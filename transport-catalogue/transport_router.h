#pragma once

#include "router.h"
#include "transport_catalogue.h"

#include <memory>

namespace transport_catalog {

	class TransportRouter {
	public:
		TransportRouter() = default;
		TransportRouter (int  wait_time, double bus_velocity ) 
			:wait_time_(wait_time), bus_velocity_(bus_velocity)
		{
		}
		TransportRouter(const TransportRouter& settings, const TransportCatalogue& catalogue) 
		{
			wait_time_ = settings.wait_time_;
			bus_velocity_ = settings.bus_velocity_;
			BuildGraph(catalogue);
		}

		const graph::DirectedWeightedGraph<double>& BuildGraph(const TransportCatalogue& catalogue);
		const std::optional<graph::Router<double>::RouteInfo> FindRoute(const std::string_view stop_from, const std::string_view stop_to) const;
     	const graph::DirectedWeightedGraph<double>& GetGraph() const;

	private:
		int  wait_time_ = 0;
		double bus_velocity_ = 0.0;
		std::map<std::string, graph::VertexId> name_to_vortex_;
		graph::DirectedWeightedGraph<double> graph_;
		std::unique_ptr<graph::Router<double>> router_;
	};
	
	
}