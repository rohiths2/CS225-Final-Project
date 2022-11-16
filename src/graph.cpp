#include "graph.h"
#include <queue>
#include <cmath>

Graph::Airport::Airport(std::vector<std::string> airport) {
    name_ = airport[1];
    city_ = airport[2];
    country_ = airport[3];
    IATA_ = airport[4];
    ICAO_ = airport[5];
    string_latitude_ = airport[6];
    string_longitude_ = airport[7];
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
    int i = 0;
    std::cout << "goeshere" << std::endl;
    for (auto connection : getConnectionsIATA()) {
        std::cout << i << "/" << getConnectionsIATA().size() << std::endl;
        std::pair<Airport, std::vector<Airport>> pair;
        pair.first = getAirportFromIATA_(connection.first);
        for (auto part : connection.second) {
            pair.second.push_back(getAirportFromIATA_(part));
        }
        ++i;
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


void Graph::BFS(std::string origin, std::string ending, bool only_complete_airports) {
    BFS_output.clear();
    BFS_visited.clear();
    std::queue<std::string> q;
    BFS_visited.push_back(origin);
    q.push(origin);
    while (!q.empty()) {
        std::string current = q.front();
        Airport ap = getAirportFromIATA_(current);
       if (only_complete_airports == false || ap.usable_ == "1") {
            std::cout << current << " --- " << ap.name_ << " --- " << ap.city_ << "---" << ap.latitude_ << "---" << ap.longitude_ << std::endl;
            BFS_output.push_back(current);
       }
        if (current == ending) {
            return;
        }
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


std::map<const Graph::Airport*, std::pair<const Graph::Airport*, float>> Graph::Dijk( Airport& start) {
    std::map<const Graph::Airport*, std::pair<const Graph::Airport*, float>> map;
    std::vector<const Graph::Airport*> airports;
    int i = 0;
    std::cout << connections_.size() << std::endl;
    for (auto airport_iter = connections_.begin(); airport_iter !=  connections_.end(); airport_iter++) {
        std::cout << i << "/" << connections_.size() << std::endl;
        //iterate through keys of connections_ map, populating shortest route map with worstcase data;
        const Airport* current_airport = &(airport_iter->first);
        map[current_airport] = std::pair<Airport*, float>(NULL, INFINITY);
        airports.push_back(current_airport);
        ++i;
    }
    map[&start] = std::pair< Graph::Airport*, float>(NULL, 0);
    while(!airports.empty()) {
        std::cout << airports.size() << std::endl;
         const Airport* closest_airport = RemoveSmallest(map, airports);
        for (auto airport : AirportIntersection(connections_.find(*closest_airport)->second, airports)) {
            float possible_distance = map[closest_airport].second + Distance(*closest_airport, *airport);
            if (possible_distance < map[airport].second) {
                map[airport].first = closest_airport;
                map[airport].second = possible_distance;
            }
        }
    }
    return map;
}

std::vector<const Graph::Airport*> Graph::AirportIntersection(std::vector<Graph::Airport>& connections, std::vector<const Graph::Airport*> airports) {
    std::vector<const Graph::Airport*> in_both;
    for (Graph::Airport airport1 : connections) {
        for (const Graph::Airport* airport2 : airports) {
            if (airport1 == *airport2) {
                in_both.push_back(airport2);
                break;
            }
        }
    }
    return in_both;
}

const Graph::Airport* Graph::RemoveSmallest(std::map<const Graph::Airport*, std::pair<const Graph::Airport*, float>>& map, std::vector<const Graph::Airport*> airports) {
    auto smallest_itr = airports.begin();
    for (auto iter = airports.begin() + 1; iter != airports.end(); iter++) {
        if (map.at(*iter).second < map.at(*smallest_itr).second) {
            smallest_itr = iter;
        }
    }
    airports.erase(smallest_itr);
    return *smallest_itr;
}

std::vector<const Graph::Airport*> Graph::shortestPath(Airport& start, Airport& destination) {
    std::vector<const Airport*> shortest_path = {&start};
    std::map<const Graph::Airport*, std::pair<const Graph::Airport*, float>> shortest_map = Dijk(start);
    const Airport* current = &destination;
    while (current != &start) {
        shortest_path.insert(shortest_path.begin() + 1, current);
        current = shortest_map[current].first; //Sets current to parent
    }
    return shortest_path;
}

float Graph::Distance(const Airport& place1, const Airport& place2) {
    //Used the Haversine formula to calculate distance in miles
    float place1_long = (place1.longitude_ * 3.1415) / 180;
    float place1_lat = (place1.latitude_ * 3.1415) / 180;
    float place2_long = (place2.longitude_ * 3.1415) / 180;
    float place2_lat = (place2.latitude_ * 3.1415) / 180;
    float distance = 3963 * acos((sin(place1_lat) * sin(place2_lat)) + cos(place1_lat) * cos(place2_lat) * cos(place2_long - place1_long));
    std::cout << distance << std::endl;
    return distance;
}


//IATA alternative functions



std::map<const std::string*, std::pair<const std::string*, float>> Graph::DijkIATA( std::string& start) {
    std::map<const std::string*, std::pair<const std::string*, float>> map;
    std::vector<const std::string*> airports;
    int i = 0;
    for (auto airport_iter = connectionsIATA_.begin(); airport_iter !=  connectionsIATA_.end(); airport_iter++) {
        std::cout << i << "/" << connections_.size() << std::endl;
        //iterate through keys of connections_ map, populating shortest route map with worstcase data;
        const std::string* current_airport = &(airport_iter->first);
        map[current_airport] = std::pair<std::string*, float>(NULL, INFINITY);
        airports.push_back(current_airport);
        ++i;
    }
    map[&start] = std::pair< std::string*, float>(NULL, 0);
    while(!airports.empty()) {
         const std::string* closest_airport = RemoveSmallestIATA(map, airports);
        for (auto airport : AirportIntersectionIATA(connectionsIATA_.find(*closest_airport)->second, airports)) {
            float possible_distance = map[closest_airport].second + DistanceIATA(*closest_airport, *airport);
            if (possible_distance < map[airport].second) {
                map[airport].first = closest_airport;
                map[airport].second = possible_distance;
            }
        }
    }
    return map;
}

std::vector<const std::string*> Graph::AirportIntersectionIATA(std::vector<std::string>& connections, std::vector<const std::string*> airports) {
    std::vector<const std::string*> in_both;
    for (std::string airport1 : connections) {
        for (const std::string* airport2 : airports) {
            if (airport1 == *airport2) {
                in_both.push_back(airport2);
                break;
            }
        }
    }
    return in_both;
}

const std::string* Graph::RemoveSmallestIATA(std::map<const std::string*, std::pair<const std::string*, float>>& map, std::vector<const std::string*> airports) {
    auto smallest_itr = airports.begin();
    for (auto iter = airports.begin() + 1; iter != airports.end(); iter++) {
        if (map.at(*iter).second < map.at(*smallest_itr).second) {
            smallest_itr = iter;
        }
    }
    airports.erase(smallest_itr);
    return *smallest_itr;
}

std::vector<const std::string*> Graph::shortestPathIATA(std::string& start, std::string& destination) {
    std::vector<const std::string*> shortest_path = {&start};
    std::map<const std::string*, std::pair<const std::string*, float>> shortest_map = DijkIATA(start);
    const std::string* current = &destination;
    while (current != &start) {
        shortest_path.insert(shortest_path.begin() + 1, current);
        current = shortest_map[current].first; //Sets current to parent
    }
    return shortest_path;
}

float Graph::DistanceIATA(const std::string& place1, const std::string& place2) {
    //Used the Haversine formula to calculate distance in miles
    Airport airport1 = getAirportFromIATA_(place1);
    Airport airport2 = getAirportFromIATA_(place2);
    float place1_long = (airport1.longitude_ * 3.1415) / 180;
    float place1_lat = (airport1.latitude_ * 3.1415) / 180;
    float place2_long = (airport2.longitude_ * 3.1415) / 180;
    float place2_lat = (airport2.latitude_ * 3.1415) / 180;
    float distance = 3963 * acos((sin(place1_lat) * sin(place2_lat)) + cos(place1_lat) * cos(place2_lat) * cos(place2_long - place1_long));
    std::cout << distance << std::endl;
    return distance;
}