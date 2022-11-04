#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include "data_parser.h"

class Graph {
    public:
        Graph(const std::string& airportFile, const std::string& routeFile);

    private:

        class Airport {

            public:
                Airport();
                Airport(std::vector<std::string> airport);

            private:
                std::string name_;
                std::string city_;
                std::string IATA_;
                std::string ICAO_;
                float longitude_;
                float latitude_;
                
        };


    
    std::map<Airport, std::vector<Airport>> connections_;
    
    std::vector<Airport> airports_;
    /**
     * Stores the data that we will use for the graph algorithm
    **/
    DataParser data_;
};