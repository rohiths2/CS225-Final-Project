#include "graph.h"
#include <queue>

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


void Graph::populateConnections(DataParser d) {
    for (auto connection : getConnectionsIATA()) {
        std::pair<Airport, std::vector<Airport>> pair;
        pair.first = getAirportFromIATA_(connection.first);
        for (auto part : connection.second) {
            pair.second.push_back(getAirportFromIATA_(part));
        }
    }
}


bool vectContains(std::vector<std::string> v, std::string str) {
    for (auto s : v) {
        if (s == str) {
            return true;
        }
    }
    return false;
}


void Graph::BFS(std::string origin) {
    BFS_output.clear();
    BFS_visited.clear();
    std::queue<std::string> q;
    BFS_visited.push_back(origin);
    q.push(origin);
    while (!q.empty()) {
        std::string current = q.front();
        std::cout << getAirportFromIATA_(current).name_ << " " << std::endl;
        BFS_output.push_back(current);
        q.pop();
        if (connectionsIATA_.find(current) != connectionsIATA_.end()) {
            for (auto neighbor : connectionsIATA_.find(current)->second) {
                if (!(vectContains(BFS_visited, neighbor))) {
                    BFS_visited.push_back(neighbor);
                    q.push(neighbor);
                }
            }
        }    
    }
}