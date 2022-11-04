#include "graph.h"


Graph::Airport::Airport(std::vector<std::string> airport) {
    // name_ = airport[1];
    // city_ = airport[2];
    // IATA_ = airport[3];
    // ICAO_ = airport[4];
    // latitude_ = airport[5];
    // longitude_ = airport[6];

}


Graph::Graph(const std::string& airportFile, const std::string& routeFile) {
    data_.populateAirportRows(airportFile);
    data_.populateRoutesRows(routeFile);
    for (size_t i = 0; i < data_.AirportsRows.size(); i++) {
        // Airport toAdd = Airport(data_.AirportsRows[i]);
        // airports_.push_back();
    }
}
