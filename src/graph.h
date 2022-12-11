#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include "data_parser.h"

class Graph {
    public:
        Graph(const DataParser& data);

        class Airport {

            public:
                Airport() {
                    name_ = "";
                    city_ = "";
                    country_ = "";
                    IATA_ = "";
                    ICAO_ = "";
                    latitude_ = 0;
                    longitude_ = 0;
                }
                Airport(std::vector<std::string> airport);
                std::string name_;
                std::string city_;
                std::string country_ ;
                std::string IATA_;
                std::string ICAO_;
                float latitude_;
                float longitude_;
                std::string string_latitude_;
                std::string string_longitude_;
                std::string usable_;

            //Returns if two airports are equal
            bool operator==(const Airport &other) const {
                    if (this->name_ == other.name_ && this->IATA_ == other.IATA_ && this->city_ == other.city_ && this->country_ == other.country_) {
                        return true;
                    }
                    return false;
            }

            //Returns if two airports are NOT equal (Airport objects can be determined not equal using "less than" operations)
            bool operator<(const Airport &other) const {
                    if (this->name_ == other.name_ && this->IATA_ == other.IATA_ && this->city_ == other.city_ && this->country_ == other.country_) {
                        return false;
                    }
                    return true;
            }
            
            private:    
                
        };

    //Getter for airports vector
    const std::vector<Graph::Airport>& getAirports() const {return airports_;}

    //Populates an adjacency list, with keys being airport IATA codes, and values being their neighbors
    //Input: DataParser object
    //Output: None, but modifies the Connections map
    void populateConnectionsIATA();
    
    //Only populates connections from a certain country to make domestic traversals faster
    //Input: DataParser object, String representing country to get connections from
    //Output: None, but modifies the Connections map
    void populateConnectionsIATA_country(std::string country_);

    //Given an IATA code, search for the airport with the matching IATA in the airports vector, and return the Airport object
    //Input: String containing Airport IATA code
    //Output: Airport object with that IATA code
    Airport& getAirportFromIATA_(std::string iata){
        if (mapIATA.find(iata) == mapIATA.end()){
            Airport a;
            mapIATA.insert(std::make_pair(iata, a));
        }
        return mapIATA.at(iata);
    }

    //Getter for connections (adjacency list with IATA codes)
    const std::map<std::string, std::vector<std::string>>& getConnectionsIATA() const { return connectionsIATA_; }

    //Getter for connections (adjacency list with Airport types)
    const std::map<Airport, std::vector<Airport>>& getConnections() const { return connections_; }

    //Adjacency list, mapping airport codes to neighboring airport codes
    std::map<std::string, std::vector<std::string>> connectionsIATA_;

    //Traverses all airports from origin (airport code) to ending (airport code) using BFS algorithm. 
    //set only_complete_airports to true to skip airports with incomplete info
    void BFS(std::string origin, std::string ending, bool only_complete_airports);

    std::vector<std::pair<std::string, std::string>> btwBFS(std::string origin, std::string ending, bool only_complete_airports);
    //Getter for BFS function: used for testing
    std::vector<std::string> getBFSoutput() {
        return BFS_output;
    } 


    //Getter for distance function: used for testing
    float getDistanceIATA(std::string& place1, std::string& place2) {
        return DistanceIATA(place1, place2);
    }
    // float getDistance(Airport& place1, Airport& place2) {
    //     return Distance(place1, place2);
    // }


    //Uses Dijkstra's algorithm to find the shortest path, returns a vector where first value is start and last value is destination
    //Input: IATA Strings of Starting and Ending airports
    //Output: Vector of IATA strings containing the shortest path from the start to destination airport in the correct order
    std::vector<std::string> shortestPathIATA(std::string& start, std::string& destination);

     // Betweeness centrality, returns a vector of IATA strings and their Centralities. MAY BE RESOURCE HEAVY ONCE IMPL.
    std::vector<std::pair<std::string, float>> BetweenessCentrality();

    // Generates a map of IATAs and their BetweenessCentralities originating from a certain input. 
    // May be resource intensive.
   std::map<std::string, float> BetweenessCentrality(std::string input);


    float getCentralityOf(std::string airportIATA);

    auto testHeap() {return DijkHeap();}

    // std::pair<std::string, float> BetweenessCentrality(std::string origin, bool only_complete_airports);
    private:

    //Used by the dijkstra function for quick access to highest priority elements
    class DijkHeap {
        public:
            //Creates an empty heap with the root at index 1
            DijkHeap() {
                IATAVector.push_back(std::pair<std::string, float>({"", 0}));
            }
            //Adds a iata and distance pair to the heap and determines their priority
            void add(std::string iata, float distance) {
                IATAVector.push_back(std::pair<std::string, float>({iata, distance}));
                IATAtoIndex[iata] = IATAVector.size() - 1;
                heapifyUp(IATAVector.size() - 1);
            }
            //Changes the distance associated with a iata and updates its priority
            void updateElem(std::string iata, float new_distance) {
                size_t index = IATAtoIndex[iata];
                IATAVector[index].second = new_distance;
                heapifyDown(index);
                heapifyUp(index);
            }
            //Returns that highest priority element and removes it from the heap
            std::string pop() {
                IATAtoIndex.erase(IATAVector[1].first);
                if (IATAVector.size() < 2) {
                    return "";
                }
                std::string temp = IATAVector[1].first;
                IATAVector[1] = IATAVector[IATAVector.size() - 1];
                IATAVector.pop_back();
                heapifyDown(1);
                return temp;
            }
            //Returns true if the heap is empty, except for the element in index 0
            bool is_empty() {
                return IATAVector.size() < 2;
            }
        private:
            size_t leftChild(size_t current_ind) {return current_ind * 2;}
            size_t rightChild(size_t current_ind) {return current_ind * 2 + 1;}
            size_t parent(size_t current_ind) {return current_ind / 2;}
            bool hasChild(size_t current_ind) {return current_ind * 2 < IATAVector.size();}

            std::vector<std::pair<std::string, float>> IATAVector;
            std::map<std::string, size_t> IATAtoIndex;
            
            void heapifyUp(size_t current_ind) {
                if (current_ind == 1) {
                    return;
                }
                size_t parent_ind = parent(current_ind);
                if (IATAVector[current_ind].second < IATAVector[parent_ind].second) {
                    std::swap(IATAVector[current_ind], IATAVector[parent_ind]);
                    IATAtoIndex[IATAVector[parent_ind].first] = parent_ind;
                    IATAtoIndex[IATAVector[current_ind].first] = current_ind;
                    heapifyUp(parent_ind);
                }

            }
            void heapifyDown(size_t current_ind) {
                if (hasChild(current_ind)) {
                    size_t next_child = 0;
                    if (current_ind * 2 + 1 >= IATAVector.size()) {
                        next_child = leftChild(current_ind);
                    } else {
                        next_child = IATAVector[leftChild(current_ind)].second < IATAVector[rightChild(current_ind)].second ? leftChild(current_ind) : rightChild(current_ind);
                    }
                    if (IATAVector[next_child].second < IATAVector[current_ind].second) {
                        std::swap(IATAVector[current_ind], IATAVector[next_child]);
                        IATAtoIndex[IATAVector[current_ind].first] = current_ind;
                        IATAtoIndex[IATAVector[next_child].first] = next_child;
                        heapifyDown(next_child);
                    }
                }
            }
    };

    //returns a map, each Airport maps to a pair. The first value is the Airports parent, the second value is the distance from the start airport
    //Input: IATA string of starting airport
    //Output: Map of Strings to Pair<String, Float>, as described above
    std::map<std::string, std::pair< std::string, float>> DijkIATA( std::string& start);


    //Find an approximate distance between two airports
    //Input: 2 IATA strings representing two airports
    //Output: Float representing the distance between the first and second IATA argument
    float DistanceIATA(std::string& place1,  std::string& place2);
    

    std::vector<std::string> BFS_visited;
    std::vector<std::string> BFS_output;

    //Private adjacency list of Airport types, NOT IATA strings
    std::map<Airport, std::vector<Airport>> connections_;
    
    // Assigns central_ the centrality of each node
    void Centrality(std::vector<std::vector<std::string>> paths);

    // Stores centrality data on a given traversal
    std::map<std::string, float> central_;
    
    /**
     * Stores only data relevant to CS225 Final Project
    **/
   //Stores Airport Object types from each row of the data-parsed airports details vector
    std::vector<Airport> airports_;
    
    //Map for quick access of Airports from IATA strings
    std::map<std::string, Airport> mapIATA;
    
    /**
     * Stores the original data of airports 
     * that we will use for the graph algorithm
    **/
    const DataParser& data_;
};