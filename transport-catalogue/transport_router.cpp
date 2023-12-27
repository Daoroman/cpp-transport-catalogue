#include "transport_router.h"

namespace transport_catalog {

	const graph::DirectedWeightedGraph<double>& TransportRouter::BuildGraph(const TransportCatalogue& catalogue) {
		auto stops = catalogue.SortAllStops();
		auto buses = catalogue.SortAllBuses();
		graph::VertexId id = 0;
		graph::DirectedWeightedGraph<double> new_graph(stops.size()*2);
		for (const auto& stop : stops) {
			name_to_vortex_[stop.second->name] = id;
			new_graph.AddEdge({ stop.second->name, 0, id, ++id, static_cast<double> (wait_time_) });
			++id;
		}
		
		for (const auto& bus : buses) {
			auto& stops = bus.second->stops;
			for (size_t i = 0; i < stops.size(); ++i) {
				for (size_t j = i + 1; j < stops.size(); ++j) {
					auto from = stops[i];
					auto to = stops[j];
					size_t dist = 0;
					size_t dist_reverse = 0;
					for (size_t k = i + 1; k <= j; ++k) {
						dist += catalogue.ComputeFactDistance({ stops[k - 1], stops[k] });
						dist_reverse += catalogue.ComputeFactDistance({ stops[k] , stops[k - 1] });
					}
					new_graph.AddEdge({bus.second->name, j -i,  name_to_vortex_.at(from->name)+1 , name_to_vortex_.at(to->name),
						static_cast<double>(dist) / (bus_velocity_ * (100.0 / 6.0)) });
					if (!bus.second->circle) {
						new_graph.AddEdge({ bus.second->name, j - i, name_to_vortex_.at(to->name) +1, name_to_vortex_.at(from->name),
						static_cast<double>(dist_reverse) / (bus_velocity_ * (100.0 / 6.0)) });
					}
				}
			}
		}
		graph_ = std::move(new_graph);
		router_ = std::make_unique<graph::Router<double>>(graph_);
		
		return graph_;
	}

	const std::optional<graph::Router<double>::RouteInfo> TransportRouter::FindRoute(const std::string_view stop_from, const std::string_view stop_to) const {
		return router_->BuildRoute(name_to_vortex_.at(std::string(stop_from)), name_to_vortex_.at(std::string(stop_to)));
	}

	const graph::DirectedWeightedGraph<double>& TransportRouter::GetGraph() const {
		return graph_;
	}

}