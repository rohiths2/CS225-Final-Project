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
         void populateConnectionsIATA(DataParser d);
         void populateConnectionsIntIndexes(DataParser d);

         Airport getAirportFromIATA_(std::string iata);

    std::map<std::string, std::vector<std::string>> getConnectionsIATA() { return connectionsIATA_; }

    std::map<std::string, std::vector<int>> getConnectionsIntIndexes() { return connectionsIntIndexes_; }

    std::map<std::string, std::vector<std::string>> connectionsIATA_;
    
    private:

    

    std::map<std::string, std::vector<int>> connectionsIntIndexes_;

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