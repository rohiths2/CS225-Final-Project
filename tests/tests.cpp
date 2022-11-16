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
  REQUIRE(a==a_test);
  Graph::Airport b = g.getAirports()[1];
  std::string b_iata = g.getAirports()[1].IATA_;
  Graph::Airport b_test = g.getAirportFromIATA_(b_iata);
  REQUIRE(b==b_test);
    Graph::Airport c = g.getAirports()[2];
  std::string c_iata = g.getAirports()[2].IATA_;
  Graph::Airport c_test = g.getAirportFromIATA_(c_iata);
  REQUIRE(c==c_test);
    Graph::Airport d = g.getAirports()[3];
  std::string d_iata = g.getAirports()[3].IATA_;
  Graph::Airport d_test = g.getAirportFromIATA_(d_iata);
  REQUIRE(d==d_test);
    Graph::Airport e = g.getAirports()[4];
  std::string e_iata = g.getAirports()[4].IATA_;
  Graph::Airport e_test = g.getAirportFromIATA_(e_iata);
  REQUIRE(e==e_test);
}

TEST_CASE("Graph_Populate_Connections_IATA_adjacency_list") {
  Graph g = Graph(d);
  g.populateConnectionsIATA(d);
  REQUIRE(g.connectionsIATA_.find("ORD")->second.size() > 100);
  REQUIRE(g.connectionsIATA_.find("ATL")->second.size() > 100);
  REQUIRE(g.connectionsIATA_.find("LAX")->second.size() > 100);
}

TEST_CASE("BFS Small") {
  Graph g = Graph(d);
  g.populateConnectionsIATA(d);
  g.BFS("SAB", "SBH", false);
  REQUIRE(g.getBFSoutput().size() > 20);
  REQUIRE(g.getBFSoutput()[0] == "SAB");
  REQUIRE(g.getBFSoutput()[g.getBFSoutput().size()-1] == "SBH");
  std::vector<std::string> visited;
  bool only_visits_once = true;
  for (auto a : g.getBFSoutput()) {
    for (auto b : visited) {
      if (a==b) {
        only_visits_once = false;
      }
    }
    visited.push_back(a);
  }
  REQUIRE(only_visits_once == true);
}


TEST_CASE("BFS Large") {
  Graph g = Graph(d);
  g.populateConnectionsIATA(d);
  g.BFS("ORD", "LAX", false);
  REQUIRE(g.getBFSoutput().size() > 50);
  REQUIRE(g.getBFSoutput()[0] == "ORD");
  REQUIRE(g.getBFSoutput()[g.getBFSoutput().size()-1] == "LAX");
  std::vector<std::string> visited;
  bool only_visits_once = true;
  for (auto a : g.getBFSoutput()) {
    for (auto b : visited) {
      if (a==b) {
        only_visits_once = false;
      }
    }
    visited.push_back(a);
  }
  REQUIRE(only_visits_once == true);
}

TEST_CASE("Test distance function") {
  
}