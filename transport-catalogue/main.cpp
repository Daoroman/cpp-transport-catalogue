#include "stat_reader.h"
#include "input_reader.h"

using namespace transport_catalog;

int main() {
    TransportCatalogue catalogue;
    std::ostream& output = std::cout;
    ReadData(std::cin, catalogue);
    ProcessRequests(output, catalogue);
}
