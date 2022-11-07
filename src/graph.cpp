#include "graph.h"


Graph::Airport::Airport(std::vector<std::string> airport) {
    name_ = airport[1];
    city_ = airport[2];
    country_ = airport[3];
    IATA_ = airport[4];
    ICAO_ = airport[5];
    latitude_ = std::stof(airport[6]);
    longitude_ = std::stof(airport[7]);
    usable_ = (airport[14]);
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

Graph::Airport Graph::getAirportFromIATA_(std::string iata) {
    for (auto airport : getAirports()) {
        if (iata == airport.IATA_) {
            return airport;
        }
    }
    Airport a;
    return a;
}

void Graph::populateConnectionsIATA(DataParser d) {
    for (auto airport : getAirports()) {
        std::pair<std::string, std::vector<std::string>> pair;
        pair.first = airport.IATA_;
        connectionsIATA_.insert(pair);
    }
    int i = 0;
    for (auto route : d.RoutesDetails) {
        std::cout << i << std::endl;
        if (connectionsIATA_.find(route[2]) != connectionsIATA_.end()) {
            connectionsIATA_.find(route[2])->second.push_back(route[4]);
        }
        ++i;
    }
    // for (auto route : d.RoutesDetails) {
    //     Airport currentAirport = getAirportFromIATA_(route[2]);
    //     Airport destination = getAirportFromIATA_(route[4]);
    //     if (connections_.count(currentAirport) > 0) {
    //        connections_.find(currentAirport)->second.push_back(destination);
    //     } else {
    //         std::pair<Airport, std::vector<Airport>> pair;
    //         pair.first = currentAirport;
    //         pair.second.push_back(destination);
    //         connections_.insert(pair);
    //     }
    // }
}
