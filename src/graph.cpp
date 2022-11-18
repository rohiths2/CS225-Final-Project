#include "graph.h"
#include <queue>
#include <stack>
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


void Graph::populateConnectionsIATA_country(DataParser d, std::string country_) {
    connectionsIATA_.clear();
    for (auto airport : getAirports()) {
        if (airport.country_ == country_) {
            std::pair<std::string, std::vector<std::string>> pair;
            pair.first = airport.IATA_;
            connectionsIATA_.insert(pair);
        }
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
    for (auto connection : getConnectionsIATA()) {
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


// std::map<const Graph::Airport*, std::pair<const Graph::Airport*, float>> Graph::Dijk( Airport& start) {
//     std::map<const Graph::Airport*, std::pair<const Graph::Airport*, float>> map;
//     std::vector<const Graph::Airport*> airports;
//     int i = 0;
//     for (auto airport_iter = connections_.begin(); airport_iter !=  connections_.end(); airport_iter++) {
//         //iterate through keys of connections_ map, populating shortest route map with worstcase data;
//         const Airport* current_airport = &(airport_iter->first);
//         map[current_airport] = std::pair<Airport*, float>(NULL, INFINITY);
//         airports.push_back(current_airport);
//         ++i;
//     }
//     map[&start] = std::pair< Graph::Airport*, float>(NULL, 0);
//     while(!airports.empty()) {
//          const Airport* closest_airport = RemoveSmallest(map, airports);
//         for (auto airport : AirportIntersection(connections_.find(*closest_airport)->second, airports)) {
//             float possible_distance = map[closest_airport].second + Distance(*closest_airport, *airport);
//             if (possible_distance < map[airport].second) {
//                 map[airport].first = closest_airport;
//                 map[airport].second = possible_distance;
//             }
//         }
//     }
//     return map;
// }

// std::vector<const Graph::Airport*> Graph::AirportIntersection(std::vector<Graph::Airport>& connections, std::vector<const Graph::Airport*> airports) {
//     std::vector<const Graph::Airport*> in_both;
//     for (Graph::Airport airport1 : connections) {
//         for (const Graph::Airport* airport2 : airports) {
//             if (airport1 == *airport2) {
//                 in_both.push_back(airport2);
//                 break;
//             }
//         }
//     }
//     return in_both;
// }

// const Graph::Airport* Graph::RemoveSmallest(std::map<const Graph::Airport*, std::pair<const Graph::Airport*, float>>& map, std::vector<const Graph::Airport*> airports) {
//     auto smallest_itr = airports.begin();
//     for (auto iter = airports.begin() + 1; iter != airports.end(); iter++) {
//         if (map.at(*iter).second < map.at(*smallest_itr).second) {
//             smallest_itr = iter;
//         }
//     }
//     airports.erase(smallest_itr);
//     return *smallest_itr;
// }

// std::vector<const Graph::Airport*> Graph::shortestPath(Airport& start, Airport& destination) {
//     std::vector<const Airport*> shortest_path = {&start};
//     std::map<const Graph::Airport*, std::pair<const Graph::Airport*, float>> shortest_map = Dijk(start);
//     const Airport* current = &destination;
//     while (current != &start) {
//         shortest_path.insert(shortest_path.begin() + 1, current);
//         current = shortest_map[current].first; //Sets current to parent
//     }
//     return shortest_path;
// }

// float Graph::Distance(const Airport& place1, const Airport& place2) {
//     //Used the Haversine formula to calculate distance in miles
//     float place1_long = (place1.longitude_ * 3.1415) / 180;
//     float place1_lat = (place1.latitude_ * 3.1415) / 180;
//     float place2_long = (place2.longitude_ * 3.1415) / 180;
//     float place2_lat = (place2.latitude_ * 3.1415) / 180;
//     float distance = 3963 * acos((sin(place1_lat) * sin(place2_lat)) + cos(place1_lat) * cos(place2_lat) * cos(place2_long - place1_long));
//     return distance;
// }


//IATA alternative functions



std::map<const std::string*, std::pair<const std::string*, float>> Graph::DijkIATA( std::string& start) {
    std::map<const std::string*, std::pair<const std::string*, float>> map;
    std::vector<const std::string*> airports;
    int i = 0;
    std::cout << "here2" << std::endl;
    for (auto airport_iter = connectionsIATA_.begin(); airport_iter !=  connectionsIATA_.end(); airport_iter++) {
        //iterate through keys of connections_ map, populating shortest route map with worstcase data;
        const std::string* current_airport = &(airport_iter->first);
        map[current_airport] = std::pair<std::string*, float>(NULL, INFINITY);
        airports.push_back(current_airport);
        ++i;
    }
    std::cout << "here2" << std::endl;
    map[&start] = std::pair< std::string*, float>(NULL, 0);
    while(!airports.empty()) {
        std::cout << airports.size() << std::endl;
         const std::string* closest_airport = RemoveSmallestIATA(map, airports);
        for (auto airport : AirportIntersectionIATA(connectionsIATA_.find(*closest_airport)->second, airports)) {
            float possible_distance = map[closest_airport].second + DistanceIATA(*closest_airport, *airport);
            if (possible_distance < map[airport].second) {
                map[airport].first = closest_airport;
                map[airport].second = possible_distance;
            }
        }
    }
    std::cout << "here2" << std::endl;
    return map;
}

// Draft Code for Betweenness centrality
// would love advice on this 
std::vector<std::pair<std::string, float>> Graph::BetweenessCentrality(){

    /***
     * @TODO: compare 2008 paper vs 2001 paper effectivity
     * Discuss limitations
     * Impl.
    */
    // https://pdodds.w3.uvm.edu/research/papers/others/2001/brandes2001a.pdf BRANDES' ORIGINAL PAPER
    // https://www.cl.cam.ac.uk/teaching/1718/MLRD/handbook/brandes.html BRANDES' 2008 DISCUSSION (UPDATED 2008 IS BEHIND PAYWALL)

    //Tried to keep return type similar to code above for Djikstra's --
    // returns a map, each Airport maps to a pair. 
    //The first value is the Airports parent, the second value is the distance from the start airport
    std::vector<float> sigma; //https://en.wikipedia.org/wiki/Betweenness_centrality Each sigma is a shortest path. 
                              // this should be of N size.    

    // Algorithim:
    // Essentially, we perform a BFS again. This can be improved by unifying both the BFS and this code together, maybe adding a switch? 
    // regardless, we perform a BFS, and then put the sigma values into a queue.
    // then, we can go through the sigma values and then get the BtwCnt. 

    // We can use Brandes' with this approach: https://pdodds.w3.uvm.edu/research/papers/others/2001/brandes2001a.pdf
    // depending on time, both algos can be very time consuming. I haven't tried it out, but I 
    // recommend a timeout (probably at 3000ms), or a stop limit.
    size_t stop_size = -1; 

    // to make things easier, assumes only complete airports.
    std::vector<std::pair<std::string, float>> betweeness_centrality; // this is what will end up returning the centralities.  
    std::vector<std::string> predecessors; // nodes that precede w in shortest path
    std::queue<std::string> q;
    std::stack<std::string> s;
    while (!q.empty()){
        std::string last_visited = q.back();
        q.pop();
        s.push(last_visited); // psuedocode time  
        //For each neighbor (w) of v,  
        // if dist(w) == -1  (distance not found yet)
            //pushback w to q
            //distance(w) = distance(v) + 1 
        // if dist(w) == distance(v) + 1, 
            // sigma[w] = sigma[w] + sigma[v];
            // predecessors.push_back(v);
    }
    // Commented out for sake of clarity.
    // std::vector<float> delta; // init to be of size V 
    // while (!s.empty()){
    //     std::string current = s.top();
    //     s.pop();
    //     for (vertex in adjacency_list(current)){
    //         delta(vertex) = delta(vertex) * (
    //             (sigma(vertex)/sigma(current)) * (1 + delta(current))); // This is where Brandes' Algo comes in.
    //         if (w not in s){
    //             Btw_c[w] = Btw_c[w] + delta(w); 
    //         }
    //     }
    // }
    std::vector<std::pair<std::string, float>> v;
    return v;
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

const std::string* Graph::RemoveSmallestIATA(std::map<const std::string*, std::pair<const std::string*, float>>& map, std::vector<const std::string*>& airports) {
    size_t smallest_idx = 0;
    float smallest_value = 99999;
    for (size_t i = 0; i < airports.size(); ++i) {
        if (map.find(airports[i]) != map.end()) {
            if (map.find(airports[i])->second.second < smallest_value) {
                smallest_value = map.find(airports[i])->second.second;
                smallest_idx = i;
            }
        }
    }
    const std::string* toreturn = airports[smallest_idx];
    airports.erase(airports.begin()+smallest_idx, airports.begin()+smallest_idx+1);
    return toreturn;
}

std::vector<const std::string*> Graph::shortestPathIATA(std::string& start, std::string& destination) {
    std::vector<const std::string*> shortest_path = {&start};
    std::cout << "here" << std::endl;
    std::map<const std::string*, std::pair<const std::string*, float>> shortest_map = DijkIATA(start);
    const std::string* current = &destination;
        std::cout << "here" << std::endl;
    while (current != &start) {
        shortest_path.insert(shortest_path.begin() + 1, current);
        current = shortest_map[current].first; //Sets current to parent
    }
        std::cout << "here" << std::endl;
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
    return distance;
}