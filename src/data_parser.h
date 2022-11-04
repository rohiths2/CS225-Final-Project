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
    void populateAirportRows(const std::string& filename);
    void populateRoutesRows(const std::string& filename);
};