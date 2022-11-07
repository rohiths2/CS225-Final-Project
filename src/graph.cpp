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
    for (auto route : d.RoutesDetails) {
        if (connectionsIATA_.find(route[2]) != connectionsIATA_.end() && route[2] != "\\N" && route[4] != "\\N") {
            connectionsIATA_.find(route[2])->second.push_back(route[4]);
        }
    }
    connectionsIATA_.erase("\\N");
}

void Graph::populateConnectionsIntIndexes(DataParser d) {
    for (auto airport : getAirports()) {
        std::pair<std::string, std::vector<int>> pair;
        pair.first = airport.IATA_;
        connectionsIntIndexes_.insert(pair);
    }
    for (size_t i = 0; i < d.RoutesDetails.size(); ++i) {
        if (connectionsIntIndexes_.find(d.RoutesDetails[i][2]) != connectionsIntIndexes_.end() && d.RoutesDetails[i][2] != "\\N") {
            connectionsIntIndexes_.find(d.RoutesDetails[i][2])->second.push_back(i);
        }
    }
    connectionsIntIndexes_.erase("\\N");
}
