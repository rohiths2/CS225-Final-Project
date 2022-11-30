#include "graph.h"
#include <queue>
#include <stack>
#include <cmath>

//Creates an Airport object type from a vector of airport's details
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

//Makes a graph from the data parser's Airport Details vector by populating the Adjacency List map
Graph::Graph(DataParser data): data_(data) {
    for (size_t i = 0; i < data_.AirportsDetails.size(); i++) {
        Airport toAdd = Airport(data_.AirportsDetails[i]);
        airports_.push_back(toAdd);
        mapIATA[toAdd.IATA_] = toAdd; 
    }
}

//Populates the adjacency list map (map from strings (airport IATAs) to vectors of strings (connected airports))
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

//Populates the adjacency list but only with connections/airports from a certain country, to make Dijkstra's faster
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


//Returns whether a vector v contains a certain string str
bool vectContains(std::vector<std::string> v, std::string str) {
    for (auto s : v) {
        if (s == str) {
            return true;
        }
    }
    return false;
}


//Performs a Breadth-First Traversal from the starting airport, stopping at the ending airport
//Setting the "Only Complete Airports" argument will not print airports that have missing/blank details, if they exist
void Graph::BFS(std::string origin, std::string ending, bool only_complete_airports) {
    if (connectionsIATA_.find(origin) == connectionsIATA_.end()) {
        std::cout << origin + " is not a valid airport code in the countries selected." << std::endl;
    }
    if (connectionsIATA_.find(ending) == connectionsIATA_.end()) {
        std::cout << ending + " is not a valid airport code in the countries selected." << std::endl;
    }
    BFS_output.clear();
    BFS_visited.clear();
    std::queue<std::string> q;
    BFS_visited.push_back(origin);
    q.push(origin);
    while (!q.empty()) {
        std::string current = q.front();
        Airport ap = getAirportFromIATA_(current); 
       if (only_complete_airports == false || ap.usable_ == "1") {
            std::cout << current << " --- Name: " << ap.name_ << " --- City: " << ap.city_ << "--- Location: (" << ap.latitude_ << ", " << ap.longitude_ << ")" << std::endl; 
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


//Main Dijkstra's algorithm logic using IATA strings (the "start" argument is the origin airport's IATA)
std::map< std::string, std::pair< std::string, float>> Graph::DijkIATA( std::string& start) {
    if (connectionsIATA_.find(start) == connectionsIATA_.end()) {
        std::cout << start + " is not a valid airport code in the countries selected." << std::endl;
    }
    std::map< std::string, std::pair< std::string, float>> map;
    std::vector< std::string> airports;
    int i = 0;
    for (auto airport_iter = connectionsIATA_.begin(); airport_iter !=  connectionsIATA_.end(); airport_iter++) {
        //iterate through keys of connections_ map, populating shortest route map with worstcase data;
         std::string current_airport = (airport_iter->first);
        map[current_airport] = std::pair<std::string, float>("", INFINITY);
        airports.push_back(current_airport);
        ++i;
    }
    map[start] = std::pair< std::string, float>("", 0);
    while(!airports.empty()) {
          std::string closest_airport = RemoveSmallestIATA(map, airports);
        for (auto airport : AirportIntersectionIATA(connectionsIATA_.find(closest_airport)->second, airports)) {
            float possible_distance = map[closest_airport].second + DistanceIATA(closest_airport, airport);
            if (possible_distance < map[airport].second) {
                map[airport].first = closest_airport;
                map[airport].second = possible_distance;
            }
        }
    }
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

//Used during Dijkstra's algorithm
//Checks if there is a common airport shared between the Connections and Airports vector arguments
std::vector< std::string> Graph::AirportIntersectionIATA(std::vector<std::string>& connections, std::vector< std::string> airports) {
    std::vector< std::string> in_both;

    for (std::string airport1 : connections) {
        for ( std::string airport2 : airports) {
            if (airport1 == airport2) {
                if (!vectContains(in_both, airport2)) {
                    in_both.push_back(airport2);
                }
            }
        }
    }
    return in_both;
}

//Finds the airport with the smallest distance value, removes it from the airports vector, and returns that airport's string IATA
const std::string Graph::RemoveSmallestIATA(std::map< std::string, std::pair< std::string, float>>& map, std::vector< std::string>& airports) {
    size_t smallest_idx = 0;
    float smallest_value = 9999999;
    for (size_t i = 0; i < airports.size(); ++i) {
        if (map.find(airports[i]) != map.end()) {
            if (map.find(airports[i])->second.second < smallest_value) {
                smallest_value = map.find(airports[i])->second.second;
                smallest_idx = i;
            }
        }
    }
     std::string toreturn = airports[smallest_idx];
    airports.erase(airports.begin()+smallest_idx, airports.begin()+smallest_idx+1);
    return toreturn;
}

//Public function to call Dijkstra's algorithm multiple times to find which shortest path ends with the destination airport
std::vector< std::string> Graph::shortestPathIATA(std::string& start, std::string& destination) {
    std::vector< std::string> shortest_path = {start};
    std::map< std::string, std::pair< std::string, float>> shortest_map = DijkIATA(start);
     std::string current = destination;
    while (current != start) {
        shortest_path.insert(shortest_path.begin() + 1, current);
        current = shortest_map[current].first; //Sets current to parent
    }
    return shortest_path;
}

//Calculates the distance between two airports using latitude and longitude
float Graph::DistanceIATA( std::string& place1,  std::string& place2) {
    //Used the Haversine formula to calculate distance in miles
    Airport airport1 = getAirportFromIATA_(place1); //Convert IATA string to Airport object and use latitude/longitude
    Airport airport2 = getAirportFromIATA_(place2);
    float place1_long = (airport1.longitude_ * 3.1415) / 180;
    float place1_lat = (airport1.latitude_ * 3.1415) / 180;
    float place2_long = (airport2.longitude_ * 3.1415) / 180;
    float place2_lat = (airport2.latitude_ * 3.1415) / 180;
    float distance = 3963 * acos((sin(place1_lat) * sin(place2_lat)) + cos(place1_lat) * cos(place2_lat) * cos(place2_long - place1_long));
    return distance;
}