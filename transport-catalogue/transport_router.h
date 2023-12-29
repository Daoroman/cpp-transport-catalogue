#pragma once

#include "router.h"
#include "transport_catalogue.h"

#include <memory>

namespace transport_catalog {

	class TransportRouter {
	public:
		TransportRouter() = default;
		
		TransportRouter(const RouteData& settings, const TransportCatalogue& catalogue) 
		{
			settings_ = settings;
			BuildGraph(catalogue);
		}

		const std::optional<graph::Router<double>::RouteInfo> FindRoute(const std::string_view stop_from, const std::string_view stop_to) const;
		
		// не совсем понял как не использовать этот метод, я использую данные из графа в функции
	  //const json::Node JsonReader::PrintRouting
		const graph::DirectedWeightedGraph<double>& GetGraph() const;

	private:
		RouteData settings_;
		std::map<std::string, graph::VertexId> name_to_vortex_;
		graph::DirectedWeightedGraph<double> graph_;
		std::unique_ptr<graph::Router<double>> router_;

		const graph::DirectedWeightedGraph<double>& BuildGraph(const TransportCatalogue& catalogue);
		graph::DirectedWeightedGraph<double>  BuildWaitEdges(const TransportCatalogue& catalogue);
		graph::DirectedWeightedGraph<double>  BuildMoveEdges(const TransportCatalogue& catalogue);
		
	};
	
	
}