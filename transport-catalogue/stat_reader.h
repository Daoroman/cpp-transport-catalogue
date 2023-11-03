#pragma once
#include "transport_catalogue.h"

#include <iostream>

namespace transport_catalog {

	void ProcessRequests(std::ostream& out, TransportCatalogue& catalog);

}
