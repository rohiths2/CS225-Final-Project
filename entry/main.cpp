#include <iostream>
#include "../src/data_parser.cpp"

int main() {
 DataParser d;
 d.populateAirportRows("../lib/airports.dat");
  d.populateRoutesRows("../lib/routes.dat");
 std::cout << d.AirportsRows.size() << std::endl;
 std::cout << d.RoutesRows.size() << std::endl;
 return 1;
}
