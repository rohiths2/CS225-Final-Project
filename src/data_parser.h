#pragma once
#include <string>
#include <vector>
#include <fstream>

class DataParser {
public:
    DataParser() {
        
    }
    std::vector<std::string> AirportsRows;
    std::vector<std::vector<std::string>> AirportsDetails;
    std::vector<std::string> RoutesRows;
    std::vector<std::vector<std::string>> RoutesDetails;
    void populateAirportRows(const std::string& filename);
    void populateAirportsDetails();
    void populateRoutesDetails();
    void populateRoutesRows(const std::string& filename);
    void checkMissingInfo();
};