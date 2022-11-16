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
    g.BFS("ORD", "LAX", true);
    std::cout << std::endl;
  //   auto ohare = g.getAirportFromIATA_("ORD");
  //   auto midway = g.getAirportFromIATA_("MDW");
  // auto a = g.shortestPath(ohare, midway);
  // for (auto b : a) {
  //   std::cout << b->name_ << std::endl;
  // }
 return 1;
}
