#include <catch2/catch_test_macros.hpp>
#include "../src/data_parser.h"
#include "../src/graph.h"
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <time.h>
#include <utility>

bool vectContains(std::string s, std::vector<std::string> v) {
  for (auto a : v) {
    if (a == s) {
      return true;
    }
  }
  return false;
}
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
  Graph g = Graph(d);
  g.populateConnectionsIATA(d);
  std::string ord = "ORD"; 
  std::string mdw = "MDW"; 
  std::string lax = "LAX"; 
  std::string ewr = "EWR"; 
  std::string lhr = "LHR"; 
  std::string bom = "BOM";
  REQUIRE(int(g.getDistanceIATA(ord, mdw)) == 15);
  REQUIRE(int(g.getDistanceIATA(ord, lax)) == 1742);
  REQUIRE(int(g.getDistanceIATA(ord, lax)) == int(g.getDistanceIATA(lax, ord)));
  REQUIRE(int(g.getDistanceIATA(ord, ewr)) == int(g.getDistanceIATA(ewr, ord)));
  REQUIRE((int(g.getDistanceIATA(ord, ewr)) >= 716 && int(g.getDistanceIATA(ord, ewr)) <= 721));
  REQUIRE((int(g.getDistanceIATA(lax, ewr)) >= 2448 && int(g.getDistanceIATA(lax, ewr)) <= 2453));
  REQUIRE((int(g.getDistanceIATA(ord, lhr)) >= 3940 && int(g.getDistanceIATA(ord, lhr)) <= 3950));
  REQUIRE((int(g.getDistanceIATA(lax, lhr)) >= 5440 && int(g.getDistanceIATA(lax, lhr)) <= 5450));
  REQUIRE((int(g.getDistanceIATA(lhr, bom)) >= 4480 && int(g.getDistanceIATA(lhr, bom)) <= 4490));
}

TEST_CASE("Airport Intersection 1") {
  Graph g = Graph(d);
  g.populateConnectionsIATA(d);
  std::vector<std::string> connections = {"a", "c", "e", "g"};
  std::vector<std::string> connections2 = {"b", "c", "g", "z"};
  std::vector<std::string> answer = {"c", "g"};
  std::vector< std::string> AirInt = g.GetAirInt(connections, connections2);
  int i = 0;
  for (auto s : AirInt) {
    REQUIRE(s == answer[i]);
    ++i;
  }
}

TEST_CASE("Airport Intersection 2") {
  Graph g = Graph(d);
  g.populateConnectionsIATA(d);
  std::vector<std::string> connections = {"a", "c", "e", "g"};
  std::vector<std::string> connections2 = {"b", "v", "u", "o"};
  std::vector< std::string> airports;
  std::vector< std::string> AirInt = g.GetAirInt(connections, connections2);
  REQUIRE(AirInt.size() == 0);
}

TEST_CASE("Airport Intersection 3") {
  Graph g = Graph(d);
  g.populateConnectionsIATA(d);
  std::vector<std::string> connections = g.getConnectionsIATA().at("SBH");
  std::cout << std::endl;
  std::vector<std::string> connections2 = {"SXM", "SAB", "ORD"};
  std::vector<std::string> answer = {"SXM", "SAB"};
  std::vector< std::string> AirInt = g.GetAirInt(connections, connections2);
  int i = 0;
  for (auto s : AirInt) {
    REQUIRE(s == answer[i]);
    ++i;
  }
}

TEST_CASE("Test Remove Smallest") {
  Graph g = Graph(d);
  std::string a = "a";
  std::string b = "b";
  std::string c = "c";
  std::vector< std::string> airports;
  airports.push_back(a);
  airports.push_back(b);
  airports.push_back(c);
  std::map< std::string, std::pair< std::string, float>> map;
  map.insert(std::pair< std::string, std::pair< std::string, float>>(a, std::pair< std::string, float>(a, 0)));
  map.insert(std::pair< std::string, std::pair< std::string, float>>(b, std::pair< std::string, float>(b, 1)));
  map.insert(std::pair< std::string, std::pair< std::string, float>>(c, std::pair< std::string, float>(c, 2)));
  REQUIRE((g.getRemoveSmallest(map, airports)) == "a");
}

TEST_CASE("Test Remove Smallest 2") {
  Graph g = Graph(d);
  std::string a = "a";
  std::string b = "b";
  std::string c = "c";
  std::vector< std::string> airports;
  airports.push_back(a);
  airports.push_back(b);
  airports.push_back(c);
  std::map< std::string, std::pair< std::string, float>> map;
  map.insert(std::pair< std::string, std::pair< std::string, float>>(a, std::pair< std::string, float>(a, 6)));
  map.insert(std::pair< std::string, std::pair< std::string, float>>(b, std::pair< std::string, float>(b, 3)));
  map.insert(std::pair< std::string, std::pair< std::string, float>>(c, std::pair< std::string, float>(c, 3)));
  REQUIRE((g.getRemoveSmallest(map, airports)) == "b");
}

TEST_CASE("Test Dijkstra's Algorithm Small") {
  Graph g = Graph(d);
  g.populateConnectionsIATA_country(d, "China");
    std::string str1 = "SHE";
    std::string str2 = "DOY";
  auto a = g.shortestPathIATA(str1, str2);
  REQUIRE((a[0] == "SHE" && a[a.size()-1] == "DOY"));
  REQUIRE(a[1] == "PEK");
}

TEST_CASE("Test Dijkstra's Algorithm Medium/Domestic") {
  Graph g = Graph(d);
  g.populateConnectionsIATA_country(d, "United States");
    std::string str1 = "PVD";
    std::string str2 = "GDV";
    std::string msp = "MSP";
    std::string dtw = "DTW";
  auto a = g.shortestPathIATA(str1, str2);
  REQUIRE((a[0] == "PVD" && a[a.size()-1] == "GDV"));
  std::vector<std::string> vect;
  for (auto element : a) {
    vect.push_back(element);
  }
  REQUIRE(vectContains(msp, vect));
  REQUIRE(vectContains(dtw, vect));
}

TEST_CASE("Test Dijkstra's Algorithm Medium/Domestic (Direct and Connecting Routes)") {
  Graph g = Graph(d);
  g.populateConnectionsIATA_country(d, "United States");
  std::string ord = "ORD";
  std::string clt = "CLT";
  auto b = g.shortestPathIATA(ord, clt);
  REQUIRE((b[0] == "ORD" && b[1] == "CLT"));
  std::string lax = "LAX";
  std::string fll = "FLL";
  auto c = g.shortestPathIATA(lax, fll);
  REQUIRE((c[0] == "LAX" && c[1] == "FLL"));
  std::string sna = "SNA";
  auto d = g.shortestPathIATA(sna, fll);
  REQUIRE(d[0] == "SNA");
  REQUIRE(d[1] == "PHX");
  REQUIRE(d[2] == "FLL");
  std::cout << "The next test case is an international test case and may take a long time. If no failures are shown above, then all the domestic test cases are passed." << std::endl;
}


// TEST_CASE("Test Dijkstra's Algorithm Large/International") {
//   Graph g = Graph(d);
//   g.populateConnectionsIATA(d);
//     std::string str1 = "LAX";
//     std::string str2 = "LHR";
//   auto a = g.shortestPathIATA(str1, str2);
//   REQUIRE(a.size() == 2);
//   REQUIRE((a[0] == "LAX" && a[1] == "LHR"));
//   str1 = "EWR";
//   str2 = "SYD";
//   auto b = g.shortestPathIATA(str1, str2);
//   REQUIRE(b.size() == 3);
//   REQUIRE(b[0] == "EWR");
//   REQUIRE((b[1] == "SFO" || b[1] == "LAX"));
//   REQUIRE(b[2] == "SYD");
// }