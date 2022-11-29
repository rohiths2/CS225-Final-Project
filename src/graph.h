#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include "data_parser.h"

class Graph {
    public:
        Graph(DataParser data);

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

    //Populates an adjacency list, with keys being Airport object types, and values being their neighbors
    void populateConnections(DataParser d);

    //Populates an adjacency list, with keys being airport IATA codes, and values being their neighbors
    void populateConnectionsIATA(DataParser d);
    void populateConnectionsIATA_country(DataParser d, std::string country_);

    //Given an IATA code, search for the airport with the matching IATA in the airports vector, and return the Airport object
    Airport& getAirportFromIATA_(std::string iata) {
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

    //Getter for AirportIntersectionIATA function: used for testing
    std::vector< std::string> GetAirInt(std::vector<std::string>& connections,  std::vector< std::string> airports) {
        return AirportIntersectionIATA(connections, airports);
    }

    //Getter for remove smallest: used for testing
      const std::string getRemoveSmallest(std::map< std::string, std::pair< std::string, float>>& map, std::vector< std::string> airports) {
        return RemoveSmallestIATA(map, airports);
      }


    //Uses Dijkstra's algorithm to find the shortest path, returns a vector where first value is start and last value is destination
    std::vector< std::string> shortestPathIATA(std::string& start, std::string& destination);
    
    private:

    //returns a map, each Airport maps to a pair. The first value is the Airports parent, the second value is the distance from the start airport
    std::map< std::string, std::pair< std::string, float>> DijkIATA( std::string& start);

    //Helper for Dijk, finds, removes and returns the Airport with the lowest distance in the inputed vector of Airport pointers
    const std::string RemoveSmallestIATA(std::map< std::string, std::pair< std::string, float>>& map, std::vector< std::string>& airports);
    
    //Helper for Dijk, returns the intersection of the two sets as a vector of Airports
    std::vector< std::string> AirportIntersectionIATA(std::vector<std::string>& connections,  std::vector< std::string> airports);
    
    //Find an approximate distance between two airports
    float DistanceIATA( std::string& place1,  std::string& place2);

    // Betweeness centrality, returns a vector of IATA strings and their Centralities. MAY BE RESOURCE HEAVY ONCE IMPL.
    std::vector<std::pair<std::string, float>> BetweenessCentrality();

    std::vector<std::string> BFS_visited;
    std::vector<std::string> BFS_output;

    //Private adjacency list of Airport types, NOT IATA strings
    std::map<Airport, std::vector<Airport>> connections_;
    
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
    DataParser data_;
};