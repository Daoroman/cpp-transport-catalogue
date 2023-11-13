#include "json_reader.h"
#include "request_handler.h"

int main() {
    transport_catalog::TransportCatalogue catalogue;
    JsonReader json_doc(std::cin);
    
    
    json_doc.FillRequests();
    json_doc.FillCatalogue(catalogue);

    const auto& stat_requests = json_doc.GetStatRequests();
    const auto& render_settings = json_doc.GetRenderSettings().AsMap();
    const auto& renderer = json_doc.FillRenderSettings(render_settings);

    RequestHandler  requesthandler(catalogue, renderer);
    json_doc.RequestOut(stat_requests, requesthandler);
}