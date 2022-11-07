#include "graph.h"


Graph::Airport::Airport(std::vector<std::string> airport) {
    name_ = airport[1];
    city_ = airport[2];
    country_ = airport[3];
    IATA_ = airport[4];
    ICAO_ = airport[5];
    latitude_ = std::stof(airport[6]);
    longitude_ = std::stof(airport[7]);
}


Graph::Graph(DataParser data): data_(data) {
    for (size_t i = 0; i < data_.AirportsDetails.size(); i++) {
        Airport toAdd = Airport(data_.AirportsDetails[i]);
        airports_.push_back(toAdd);
    }
}

std::vector<Graph::Airport> Graph::getAirports() {
    return airports_;
}

void Graph::populateConnections(DataParser d) {
    for (auto airport : getAirports()) {
        for (auto route : d.RoutesDetails) {
            if (route[2] == airport.IATA_) {

            }
        }
    }
}
