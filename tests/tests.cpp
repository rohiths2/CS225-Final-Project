#include <catch2/catch_test_macros.hpp>
#include "../src/data_parser.h"
#include "../src/graph.h"
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <time.h>
#include <utility>


using namespace std;
DataParser d;

TEST_CASE("populate_airports_lines_data_in_vector")
{
    d.populateAirportRows("../lib/airports.dat");
    d.populateRoutesRows("../lib/routes.dat");
    REQUIRE(d.AirportsRows.size() == 7698);
    REQUIRE(d.RoutesRows.size() == 67663);
}

TEST_CASE("airports_2D_vector_population") {
  d.populateAirportsDetails();
  REQUIRE(d.AirportsDetails.size() == d.AirportsRows.size());
  REQUIRE(d.AirportsDetails[0].size() == 15);
   REQUIRE(d.AirportsDetails[0][d.AirportsDetails[0].size()-1] == "1");
  REQUIRE(d.AirportsDetails[1].size() == d.AirportsDetails[0].size());
  REQUIRE(d.AirportsDetails[2].size() == d.AirportsDetails[1].size());
}

TEST_CASE("routes_2D_vector_population") {
    d.populateRoutesDetails();
    REQUIRE(d.RoutesDetails.size() == d.RoutesRows.size());
    REQUIRE(d.RoutesDetails[0].size() == 10);
    REQUIRE(d.RoutesDetails[0][d.RoutesDetails[0].size()-1] == "1");
      REQUIRE(d.RoutesDetails[1].size() == d.RoutesDetails[0].size());
  REQUIRE(d.RoutesDetails[2].size() == d.RoutesDetails[1].size());
}

TEST_CASE("Graph_Populate_Airport") {
  d.populateAirportRows("../lib/airports.dat");
  d.populateRoutesRows("../lib/routes.dat");
  d.populateAirportsDetails();
  d.populateRoutesDetails();
  d.checkMissingInfo();
  Graph g = Graph(d);
  REQUIRE(g.getAirports().size() == d.AirportsDetails.size());
  REQUIRE(g.getAirports()[0].name_ == "Goroka Airport");
}


TEST_CASE("Airport_IATA_Conversions") {
  Graph g = Graph(d);
  Graph::Airport a = g.getAirports()[0];
  std::string a_iata = g.getAirports()[0].IATA_;
  Graph::Airport a_test = g.getAirportFromIATA_(a_iata);
  REQUIRE(a.name_ == a_test.name_);
  Graph::Airport b = g.getAirports()[1];
  std::string b_iata = g.getAirports()[1].IATA_;
  Graph::Airport b_test = g.getAirportFromIATA_(b_iata);
  REQUIRE(b.name_ == b_test.name_);
    Graph::Airport c = g.getAirports()[2];
  std::string c_iata = g.getAirports()[2].IATA_;
  Graph::Airport c_test = g.getAirportFromIATA_(c_iata);
  REQUIRE(c.name_ == c_test.name_);
    Graph::Airport d = g.getAirports()[3];
  std::string d_iata = g.getAirports()[3].IATA_;
  Graph::Airport d_test = g.getAirportFromIATA_(d_iata);
  REQUIRE(d.name_ == d_test.name_);
    Graph::Airport e = g.getAirports()[4];
  std::string e_iata = g.getAirports()[4].IATA_;
  Graph::Airport e_test = g.getAirportFromIATA_(e_iata);
  REQUIRE(e.name_ == e_test.name_);
}