#include "request_handler.h"
 

transport_catalog::BusInfo RequestHandler::GetBusStat(const std::string_view bus_number) const {
	return catalogue_.GetBusinfo(bus_number);

}
const std::set<std::string> RequestHandler::GetBusesByStop(std::string_view stop_name) const {
	return catalogue_.FindStop(stop_name)->buses;
}

bool RequestHandler::IsBusNumber(const std::string_view bus_number) const {
	return catalogue_.FindBus(bus_number);
}

bool RequestHandler::IsStopName(const std::string_view stop_name) const {
	return catalogue_.FindStop(stop_name);
}

svg::Document RequestHandler::RenderMap() const {
	return renderer_.GetSVG(catalogue_.SortAllBuses());
}

 const std::optional<graph::Router<double>::RouteInfo> RequestHandler::GetOptimalRoute(const std::string_view stop_from, const std::string_view stop_to) const {
	return router_.FindRoute(stop_from, stop_to);
}


const std::vector< graph::Edge<double>>& RequestHandler::GetRouteInformation() const {
	return router_.GetInformation();
}