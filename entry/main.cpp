#include <iostream>
#include "../src/data_parser.cpp"
#include "../src/graph.cpp"

int main() {
 DataParser d;
d.populateAirportRows("../lib/airports.dat");
d.populateRoutesRows("../lib/routes.dat");
d.populateAirportsDetails();
d.populateRoutesDetails();
d.checkMissingInfo();
std::cout << "Airports Details for First 20 Airports" << std::endl;
for (int i = 0; i < 20; ++i) {
  std::cout << "Name: " << d.AirportsDetails[i][1] << "       --- City: " << d.AirportsDetails[i][2] << "        --- Code: " << d.AirportsDetails[i][4] << std::endl;
}
std::cout << std::endl;
std::cout << "Routes Details for First 20 Routes" << std::endl;
for (int i = 0; i < 20; ++i) {
  std::cout << "Source: " << d.RoutesDetails[i][2] << "       --- Dest: " << d.RoutesDetails[i][4] << std::endl;
}
std::cout << std::endl;
Graph g = Graph(d);
std::cout << "Details for ORD" << std::endl;
Graph::Airport ord = g.getAirportFromIATA_("ORD");
std::cout << "Name: " << ord.name_ << " --- City: " << ord.city_ << " --- Country " << ord.country_ << " --- Latitude " << ord.latitude_ << " --- Longitude " << ord.longitude_ << std::endl;
std::cout << std::endl;
std::cout << "Details for LAX" << std::endl;
Graph::Airport lax = g.getAirportFromIATA_("LAX");
std::cout << "Name: " << lax.name_ << " --- City: " << lax.city_ << " --- Country " << lax.country_ << " --- Latitude " << lax.latitude_ << " --- Longitude " << lax.longitude_ << std::endl;

std::cout << std::endl;
std::cout << "Adjacency List" << std::endl;
g.populateConnectionsIATA_country(d, "United States");
std::cout << "Connections From MDW" << std::endl;
std::vector<std::string> v = g.getConnectionsIATA().find("MDW")->second;
for (auto a : v) {
  std::cout << a << ", ";
}
std::cout << std::endl;
std::cout << std::endl;
std::cout << "Connections From AUS" << std::endl;
std::vector<std::string> w = g.getConnectionsIATA().find("AUS")->second;
for (auto a : w) {
  std::cout << a << ", ";
}
std::cout << std::endl;
std::cout << std::endl;
//std::cout << "BFS Traversal from SAB to SBH" << std::endl;
g.populateConnectionsIATA(d);
//g.BFS("SAB", "SBH", false);
std::cout << std::endl;
std::cout << std::endl;
//std::cout << "BFS Traversal from ORD to LAX" << std::endl;
g.populateConnectionsIATA(d);
//g.BFS("ORD", "LAX", false);

g.populateConnectionsIATA_country(d, "China");


    std::string str1 = "SHE";
    std::string str2 = "DOY";
  auto a = g.shortestPathIATA(str1, str2);
  for (auto b : a) {
    std::cout << b << std::endl;
  }
 return 1;
}
