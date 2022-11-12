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
                std::string usable_;

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

        std::vector<Airport> getAirports();
        void populateConnections(DataParser d);
         void populateConnectionsIATA(DataParser d);
         void populateConnectionsIntIndexes(DataParser d);

         Airport getAirportFromIATA_(std::string iata);

    std::map<std::string, std::vector<std::string>> getConnectionsIATA() { return connectionsIATA_; }

    std::map<Airport, std::vector<Airport>> getConnections() { return connections_; }

    std::map<std::string, std::vector<std::string>> connectionsIATA_;

    void BFS(std::string origin);

    //Uses Dijkstra's algorithm to find the shortest path, returns a vector where first value is start and last value is destination
    std::vector<const Airport*> shortestPath(Airport& start, Airport& destination) const;
    
    private:

    //returns a map, each Airport maps to a pair. The first value is the Airports parent, the second value is the distance from the start airport
    std::map<const Airport*, std::pair<const Airport*, float>> Dijk(const Airport& start) const;

    //Helper for Dijk, finds, removes and returns the Airport with the lowest distance in the inputed vector of Airport pointers
    const Graph::Airport* RemoveSmallest(std::map<const Graph::Airport*, std::pair<const Graph::Airport*, float>>& map, std::vector<const Graph::Airport*> airports) const;

    //Helper for Dijk, returns the intersection of the two sets as a vector of Airports
    std::vector<const Airport*> AirportIntersection(const std::vector<Airport>& connections, const std::vector<Airport*> airports) const;

    //Find an approximate distance between two airports
    float Distance(const Airport& place1, const Airport& place2) const;

    std::vector<std::string> BFS_visited;
    std::vector<std::string> BFS_output;

    std::map<Airport, std::vector<Airport>> connections_;
    
    /**
     * Stores only data relevant to CS225 Final Project
    **/
    std::vector<Airport> airports_;
    
    /**
     * Stores the original data of airports 
     * that we will use for the graph algorithm
    **/
    DataParser data_;
};