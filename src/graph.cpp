#include "graph.h"
#include <queue>
#include <stack>
#include <cmath>

//Returns whether a vector v contains a certain string str
bool vectContains(std::vector<std::string> v, std::string str) {
    for (auto s : v) {
        if (s == str) {
            return true;
        }
    }
    return false;
}

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
Graph::Graph(const DataParser& data): data_(data) {
    for (size_t i = 0; i < data_.AirportsDetails.size(); i++) {
        Airport toAdd = Airport(data_.AirportsDetails[i]);
        airports_.push_back(toAdd);
        mapIATA[toAdd.IATA_] = toAdd;
        if (toAdd.IATA_.size() != 3 && toAdd.IATA_ != "\\N") {
            /*If this error gets throw there is probably a comma in the name of the airport, or something else that has caused
                the misalignment of the data*/
            throw(std::runtime_error("Value should not be possible: IATA is " + toAdd.IATA_));
        }
    }
}

//Populates the adjacency list map (map from strings (airport IATAs) to vectors of strings (connected airports))
void Graph::populateConnectionsIATA() {
    for (auto airport : getAirports()) {
        std::pair<std::string, std::vector<std::string>> pair;
        pair.first = airport.IATA_;
        connectionsIATA_.insert(pair);
    }
    for (auto route : data_.RoutesDetails) {
        if (connectionsIATA_.find(route[2]) != connectionsIATA_.end() && route[2] != "\\N" && route[4] != "\\N") {
            if (!vectContains(connectionsIATA_.find(route[2])->second, route[4])) {
                connectionsIATA_.find(route[2])->second.push_back(route[4]);
            }
        }
    }
    connectionsIATA_.erase("\\N");
}

//Populates the adjacency list but only with connections/airports from a certain country, to make Dijkstra's faster
void Graph::populateConnectionsIATA_country(std::string country_) {
    connectionsIATA_.clear();
    for (auto airport : getAirports()) {
        if (airport.country_ == country_) {
            std::pair<std::string, std::vector<std::string>> pair;
            pair.first = airport.IATA_;
            connectionsIATA_.insert(pair);
        }
    }
    for (auto route : data_.RoutesDetails) {
        if (connectionsIATA_.find(route[2]) != connectionsIATA_.end() && route[2] != "\\N" && route[4] != "\\N") {
            if (!vectContains(connectionsIATA_.find(route[2])->second, route[4])) {
                connectionsIATA_.find(route[2])->second.push_back(route[4]);
            }
        }
    }
    connectionsIATA_.erase("\\N");
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
std::map<std::string, std::pair< std::string, float>> Graph::DijkIATA( std::string& start) {
    if (connectionsIATA_.find(start) == connectionsIATA_.end()) {
        std::cout << start + " is not a valid airport code in the countries selected." << std::endl;
        return std::map<std::string, std::pair<std::string, float>>();
    }

    std::map< std::string, std::pair< std::string, float>> map;
    DijkHeap priority;
    for (auto airport_iter = connectionsIATA_.begin(); airport_iter != connectionsIATA_.end(); airport_iter++) {
        priority.add(airport_iter->first, INFINITY);
        map[airport_iter->first] = std::pair<std::string, float>("", INFINITY);
    }
    priority.updateElem(start, 0);
    map[start] = std::pair<std::string, float>({"",0});

    while(!priority.is_empty()) {
        std::string min = priority.pop();
        for (std::string neighbor : connectionsIATA_.at(min)) {
            float new_dist = map[min].second + DistanceIATA(min, neighbor);
            if (new_dist < map[neighbor].second) {
                map[neighbor].first = min;
                map[neighbor].second = new_dist;
                priority.updateElem(neighbor, new_dist);
            }
        }
    }
    return map;
}

std::vector<std::pair<std::string, std::string>> Graph::btwBFS(std::string origin, std::string ending, bool only_complete_airports) {
    std::vector<std::pair<std::string, std::string>> retVec;
    if (connectionsIATA_.find(origin) == connectionsIATA_.end()) {
        std::cout << origin + " is not a valid airport code in the countries selected." << std::endl;
        return retVec;
    }
    if (connectionsIATA_.find(ending) == connectionsIATA_.end()) {
        std::cout << ending + " is not a valid airport code in the countries selected." << std::endl;
        return retVec;
    }
    if (!only_complete_airports){
        std::cout << "Needs complete airports" << std::endl;
        return retVec; // would be better to throw exception.
    }
    std::queue<std::string> q;
    BFS_visited.push_back(origin);
    q.push(origin);
    while (!q.empty()) {
        std::string prev = q.front();
        std::string current = q.front();
        Airport ap = getAirportFromIATA_(current); 
       if (only_complete_airports == false || ap.usable_ == "1") {
            std::cout << current << " --- Name: " << ap.name_ << " --- City: " << ap.city_ << "--- Location: (" << ap.latitude_ << ", " << ap.longitude_ << ")" << std::endl; 
            BFS_output.push_back(current);
       }
        if (current == ending) {
            return retVec;
        }
        q.pop();
        if (connectionsIATA_.find(current) != connectionsIATA_.end()) {
            for (auto neighbor : connectionsIATA_.find(current)->second) {
                if (!(vectContains(BFS_visited, neighbor))) {
                    retVec.push_back(std::make_pair(prev, neighbor));
                    BFS_visited.push_back(neighbor);
                    q.push(neighbor);
                }
            }
        }    
    }
    return retVec;
}

/***
 * @ TODO: Fix their BTW for improper input.
*/


std::map<std::string, float> Graph::BetweenessCentrality(std::string input){
    std::map<std::string, float> between_cents; // implies between cents of input.
    std::map<std::string, float> distances;
    std::map<std::string, float> sigmas;
    std::map<std::string, float> visitations;
    std::map<std::string, std::vector<std::string>> paths;

     if (connectionsIATA_.find(input) == connectionsIATA_.end()) {
        std::cout << input + " is not a valid airport code in the countries selected. Returning empty list." << std::endl;
        return between_cents;
    }

    std::cout << input + " is assumed to be a complete airport." << std::endl;


    std::vector<std::string> IATAVals;


    for (auto it = connectionsIATA_.begin(); it != connectionsIATA_.end(); it++){

        std::string name = it->first;
        IATAVals.push_back(name);
    }
    
    // cant directly use the connectionsIATA_ map due to an "out of range" error. The fix is not too resource costly
    // but its curious nonetheless.
    for (std::string name : IATAVals){
        between_cents[name] = 0.0; 
        distances.insert({name, -1.0});
        visitations.insert({name, 0.0});
        paths.insert({name, std::vector<std::string>()}); 
    }

    std::queue<std::string> q;
    std::stack<std::string> s;
    q.push(input); // input = origin
    if (between_cents.find(input) == between_cents.end()){
        std::cout << "You have included an airport that does not appear to be complete, does not exist, or is an island. Please make sure your airport exists" << std::endl;
        return between_cents;
    } 
    distances[input] = 0; // should be useless. 

    while (!q.empty()) {
        std::string current = q.front();
        s.push(current);
        q.pop();

        // if neighbors exist 
        if (connectionsIATA_.find(current) != connectionsIATA_.end()) {
            // for each neighbor... 
            for (auto neighbor : connectionsIATA_.find(current)->second) {
                if (neighbor == "FKI" || neighbor == "YAT" || neighbor == "SVR" ){ 
                    // Island airports.  Theoretically, it *should* be okay to include these,
                    // but the values are noninteresting, and make compute worse.
                    continue;
                }
                if (distances[neighbor] == -1 ){ // if not visited. Can be changed to infinity, etc.
                    distances[neighbor] = distances[current] + 1;
                    q.push(neighbor);
                }

                if (distances[neighbor] == distances[current] + 1){
                    
                    sigmas[neighbor] = sigmas[neighbor] + 1; 
                    std::vector<std::string> npaths = paths[neighbor];
                    npaths.push_back(current);
                    paths[neighbor] = npaths; // neighbor visited current
                                        
                }
            }
        }
    }
    while (!s.empty()){
        std::string visited = s.top();
        s.pop();
        for (auto i : paths[visited]){
            sigmas[i] = sigmas[i] + (paths[i].size()) * ((1 + sigmas[visited]) );
                                                             // or paths.at(visited), but paths.size should proper
                                                             // / (double)paths.size()
        }
        if (visited != input){
            between_cents[visited] = (between_cents[visited] + sigmas[visited] / (double)paths.size()); // moving div oper.
        }
    }
    return between_cents;
}


//Public function to call Dijkstra's algorithm multiple times to find which shortest path ends with the destination airport
std::vector<std::string> Graph::shortestPathIATA(std::string& start, std::string& destination) {
    std::vector< std::string> shortest_path = {start};
    std::map<std::string, std::pair< std::string, float>> shortest_map = DijkIATA(start);
    if (shortest_map.begin() == shortest_map.end()) {
        //If the map is empty, most likely caused by start and destination not being connectable
        return shortest_path;
    }
    std::string current = destination;
    if (shortest_map[current].first == "") {
        return shortest_path;
    }
    while (current != start) {
        shortest_path.insert(shortest_path.begin() + 1, current);
        current = shortest_map[current].first; //Sets current to parent
    }
    return shortest_path;
}

//Calculates the distance between two airports using latitude and longitude
float Graph::DistanceIATA(std::string& place1, std::string& place2){
    //Used the Haversine formula to calculate distance in miles
    Airport airport1 = getAirportFromIATA_(place1); //Convert IATA string to Airport object and use latitude/longitude
    Airport airport2 = getAirportFromIATA_(place2);
    if (airport1 == Airport() || airport2 == Airport()) {
        return INFINITY;
    }
    float place1_long = (airport1.longitude_ * 3.1415) / 180;
    float place1_lat = (airport1.latitude_ * 3.1415) / 180;
    float place2_long = (airport2.longitude_ * 3.1415) / 180;
    float place2_lat = (airport2.latitude_ * 3.1415) / 180;
    float distance = 3963 * acos((sin(place1_lat) * sin(place2_lat)) + cos(place1_lat) * cos(place2_lat) * cos(place2_long - place1_long));
    return distance;
}