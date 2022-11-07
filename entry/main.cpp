#include <iostream>
#include "../src/data_parser.cpp"
#include "../src/graph.cpp"

int main() {
 DataParser d;
 d.populateAirportRows("../lib/airports.dat");
  d.populateRoutesRows("../lib/routes.dat");
 std::cout << d.AirportsRows.size() << std::endl;
 std::cout << d.RoutesRows.size() << std::endl;
 d.populateAirportsDetails();
  std::cout << d.AirportsDetails.size() << std::endl;
  std::cout << d.AirportsDetails[0].size() << std::endl;
  std::cout << d.AirportsDetails[16].size() << std::endl;
  std::cout << d.AirportsDetails[729].size() << std::endl;
 d.populateRoutesDetails();
  std::cout << d.RoutesDetails.size() << std::endl;
  std::cout << d.RoutesDetails[0].size() << std::endl;
  std::cout << d.RoutesDetails[16].size() << std::endl;
  std::cout << d.RoutesDetails[729].size() << std::endl;
  d.checkMissingInfo();
  Graph g = Graph(d);
    g.populateConnectionsIATA(d);
    for (auto pair : g.getConnectionsIATA()) {
        if (pair.first == "ORD" || pair.first == "CLT") {
            std::cout << g.getAirportFromIATA_(pair.first).city_ << " : {";
            for (auto part : pair.second) {
                std::cout << g.getAirportFromIATA_(part).city_ << ",";
            }
            std::cout << "}" << std::endl;
        }
    }
 return 1;
}
