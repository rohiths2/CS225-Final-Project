#include <iostream>
#include "../src/data_parser.cpp"

int main() {
 DataParser d;
 d.populateAirportRows("../build/airports.dat");
 std::cout << d.AirportsRows.size() << std::endl;
 return 1;
}
