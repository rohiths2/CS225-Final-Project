#pragma once
#include <string>
#include <vector>
#include <fstream>

class DataParser {
public:
    DataParser() {
        
    }
    std::vector<std::string> AirportsRows;
    std::vector<std::string> RoutesRows;
    void populateAirportRows(std::string filename);
    void populateRoutesRows(std::string filename);
};