#include "request_handler.h"

/*
 * Здесь можно было бы разместить код обработчика запросов к базе, содержащего логику, которую не
 * хотелось бы помещать ни в transport_catalogue, ни в json reader.
 *
 * Если вы затрудняетесь выбрать, что можно было бы поместить в этот файл,
 * можете оставить его пустым.
 */
 

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
