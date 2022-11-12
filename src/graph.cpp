#include "graph.h"
#include <queue>
#include <cmath>

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


std::map<const Graph::Airport*, std::pair<const Graph::Airport*, float>> Graph::Dijk(const Airport& start) const {
    std::map<const Graph::Airport*, std::pair<const Graph::Airport*, float>> map;
    std::vector<const Graph::Airport*> airports;
    for (auto airport_iter = connections_.begin(); airport_iter !=  connections_.end(); airport_iter++) {
        //iterate through keys of connections_ map, populating shortest route map with worstcase data;
        const Airport* current_airport = &(airport_iter->first);
        map[current_airport] = std::pair<Airport*, float>(NULL, INFINITY);
        airports.push_back(current_airport);
    }
    map[&start] = std::pair<const Graph::Airport*, float>(NULL, 0);
    while(!airports.empty()) {
        const Airport* closest_airport = RemoveSmallest(map, airports);
        for (Airport* airport : AirportIntersection(connections_[*closest_airport], airports)) {
            float possible_distance = map[closest_airport].second + Distance(*closest_airport, *airport);
            if (possible_distance < map[airport].second) {
                map[airport].first = closest_airport;
                map[airport].second = possible_distance;
            }
        }
    }
    return map;
}

std::vector<const Airport*> AirportIntersection(const std::vector<Airport>& connections, const std::vector<Airport*> airports) const {
    std::vector<const Airport*> in_both;
    for (Airport airport1 : connections) {
        for (Airport* airport2 : airports) {
            if (airport1 == *airport2) {
                in_both.push_back(airport2);
                break;
            }
        }
    }
    return in_both;
}

const Graph::Airport* Graph::RemoveSmallest(std::map<const Graph::Airport*, std::pair<const Graph::Airport*, float>>& map, std::vector<const Graph::Airport*> airports) const {
    std::vector<const Graph::Airport*>::iterator smallest_itr = airports.begin();
    for (auto iter = airports.begin() + 1; iter != airports.end(); iter++) {
        if (map.at(*iter).second < map.at(*smallest_itr).second) {
            smallest_itr = iter;
        }
    }
    airports.erase(smallest_itr);
    return *smallest_itr;
}

std::vector<const Graph::Airport*> Graph::shortestPath(Airport& start, Airport& destination) const {
    std::vector<const Airport*> shortest_path = {&start};
    std::map<const Graph::Airport*, std::pair<const Graph::Airport*, float>> shortest_map = Dijk(start);
    const Airport* current = &destination;
    while (current != &start) {
        shortest_path.insert(shortest_path.begin() + 1, current);
        current = shortest_map[current].first; //Sets current to parent
    }
    return shortest_path;
}

float Graph::Distance(const Airport& place1, const Airport& place2) const {
    //Used the Haversine formula to calculate distance in miles
    float place1_long = (place1.longitude_ * 3.1415) / 180;
    float place1_lat = (place1.latitude_ * 3.1415) / 180;
    float place2_long = (place2.longitude_ * 3.1415) / 180;
    float place2_lat = (place2.latitude_ * 3.1415) / 180;
    float distance = 3963 * acos((sin(place1_lat) * sin(place2_lat)) + cos(place1_lat) * cos(place2_lat) * cos(place2_long - place1_long));
    return distance;
}