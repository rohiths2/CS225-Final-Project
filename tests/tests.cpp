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

TEST_CASE("populate_airports_lines_data_in_vector", "[part=1]")
{
    d.populateAirportRows("../lib/airports.dat");
    d.populateRoutesRows("../lib/routes.dat");
    REQUIRE(d.AirportsRows.size() == 7698);
    REQUIRE(d.RoutesRows.size() == 67663);
}

TEST_CASE("airports_2D_vector_population", "[part=1]") {
  d.populateAirportsDetails();
  REQUIRE(d.AirportsDetails.size() == d.AirportsRows.size());
  REQUIRE(d.AirportsDetails[0].size() == 15);
   REQUIRE(d.AirportsDetails[0][d.AirportsDetails[0].size()-1] == "1");
  REQUIRE(d.AirportsDetails[1].size() == d.AirportsDetails[0].size());
  REQUIRE(d.AirportsDetails[2].size() == d.AirportsDetails[1].size());
}

TEST_CASE("routes_2D_vector_population", "[part=1]") {
    d.populateRoutesDetails();
    REQUIRE(d.RoutesDetails.size() == d.RoutesRows.size());
    REQUIRE(d.RoutesDetails[0].size() == 10);
    REQUIRE(d.RoutesDetails[0][d.RoutesDetails[0].size()-1] == "1");
      REQUIRE(d.RoutesDetails[1].size() == d.RoutesDetails[0].size());
  REQUIRE(d.RoutesDetails[2].size() == d.RoutesDetails[1].size());
}

TEST_CASE("Graph_Populate_Airport", "[part=1]") {
  d.populateAirportRows("../lib/airports.dat");
  d.populateRoutesRows("../lib/routes.dat");
  d.populateAirportsDetails();
  d.populateRoutesDetails();
  d.checkMissingInfo();
  Graph g = Graph(d);
  REQUIRE(g.getAirports().size() == d.AirportsDetails.size());
  REQUIRE(g.getAirports()[0].name_ == "Goroka Airport");
}


TEST_CASE("Airport_IATA_Conversions", "[part=1]") {
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

TEST_CASE("Graph_Populate_Connections_IATA_adjacency_list", "[part=1]") {
  Graph g = Graph(d);
  g.populateConnectionsIATA();
  REQUIRE(g.connectionsIATA_.find("ORD")->second.size() > 100);
  REQUIRE(g.connectionsIATA_.find("ATL")->second.size() > 100);
  REQUIRE(g.connectionsIATA_.find("LAX")->second.size() > 100);
}

TEST_CASE("BFS Small", "[part=1]") {
  Graph g = Graph(d);
  g.populateConnectionsIATA();
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


TEST_CASE("BFS Large", "[part=1]") {
  Graph g = Graph(d);
  g.populateConnectionsIATA();
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

TEST_CASE("Test distance function", "[part=1]") {
  Graph g = Graph(d);
  g.populateConnectionsIATA();
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

TEST_CASE("Dijk ordered heap test", "[part=1]") {
  Graph g = Graph(d);
  auto heap = g.testHeap();
  std::vector<std::string> alphabet = {"a", "b", "c", "d", "e", "f"};
  std::vector<float> distances = {1, 2, 3, 4, 5, 6};
  for (size_t index = 0; index < alphabet.size(); index++) {
    heap.add(alphabet[index], distances[index]);
  }
  for (size_t index = 0; index < alphabet.size(); index++) {
    REQUIRE(alphabet[index] == heap.pop());
  }
}

TEST_CASE("Dijk unordered heap test", "[part=1]") {
  Graph g = Graph(d);
  auto heap = g.testHeap();
  std::vector<std::string> alphabet = {"a", "b", "c", "d", "e", "f"};
  std::vector<float> distances = {5, 1, 8, 3, 7, 2};
  for (size_t index = 0; index < alphabet.size(); index++) {
    heap.add(alphabet[index], distances[index]);
  }
  REQUIRE(heap.pop() == "b");
  REQUIRE(heap.pop() == "f");
  REQUIRE(heap.pop() == "d");
  REQUIRE(heap.pop() == "a");
  REQUIRE(heap.pop() == "e");
  REQUIRE(heap.pop() == "c");
}

TEST_CASE("Dijk heap test, update element", "[part=1]") {
  Graph g = Graph(d);
  auto heap = g.testHeap();
  std::vector<std::string> alphabet = {"a", "b", "c", "d", "e", "f"};
  std::vector<float> distances = {5, 1, 8, 3, 7, 2};
  for (size_t index = 0; index < alphabet.size(); index++) {
    heap.add(alphabet[index], distances[index]);
  }
  for (size_t index = 0; index < alphabet.size(); index++) {
    heap.updateElem(alphabet[index], float(index));
  }
  for (size_t index = 0; index < alphabet.size(); index++) {
    REQUIRE(alphabet[index] == heap.pop());
  }
}

TEST_CASE("Test Dijkstra's Algorithm Small", "[part=1]") {
  Graph g = Graph(d);
  g.populateConnectionsIATA_country("China");
    std::string str1 = "SHE";
    std::string str2 = "DOY";
  auto a = g.shortestPathIATA(str1, str2);
  REQUIRE((a[0] == "SHE" && a[a.size()-1] == "DOY"));
  REQUIRE(a[1] == "PEK");
}

TEST_CASE("Test Dijkstra's Algorithm Medium/Domestic", "[part=1]") {
  Graph g = Graph(d);
  g.populateConnectionsIATA_country("United States");
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

TEST_CASE("Test Dijkstra's Algorithm Medium/Domestic (Direct and Connecting Routes)", "[part=1]") {
  Graph g = Graph(d);
  g.populateConnectionsIATA_country("United States");
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
 
}


TEST_CASE("Test Dijkstra's Algorithm Large/International", "[part=2]") {
   std::cout << "This test case is an international test case and may take a long time. If no failures are shown above, then all the domestic test cases are passed." << std::endl;
  Graph g = Graph(d);
  g.populateConnectionsIATA();
  std::string str1 = "LAX";
  std::string str2 = "LHR";
  auto a = g.shortestPathIATA(str1, str2);
  REQUIRE(a.size() == 2);
  REQUIRE((a[0] == "LAX" && a[1] == "LHR"));
}

TEST_CASE("Test Dijkstra's Algorithm Large/International 2", "[part=2]") {
  std::cout << "This test case is an international test case and may take a long time. If no failures are shown above, then all the domestic test cases are passed." << std::endl;
  Graph g = Graph(d);
  g.populateConnectionsIATA();
  std::string str1 = "EWR";
  std::string str2 = "SYD";
  auto b = g.shortestPathIATA(str1, str2);
  REQUIRE(b.size() == 3);
  REQUIRE(b[0] == "EWR");
  REQUIRE((b[1] == "SFO" || b[1] == "LAX"));
  REQUIRE(b[2] == "SYD");
}

TEST_CASE("Test Betweeness Centrality Algorithm for Small US Airport Origins", "[part=1]") {
  Graph g = Graph(d);
  g.populateConnectionsIATA();
  auto vect = g.BetweenessCentrality("HVN");
  REQUIRE(vect["ORD"] > vect["ONT"]);
  REQUIRE(vect["PEK"] > vect["PIT"]);
}

TEST_CASE("Test Betweeness Centrality Algorithm for International US Airport Origins", "[part=2]") {
  Graph g = Graph(d);
  g.populateConnectionsIATA();
  auto vect = g.BetweenessCentrality("ORD");
  REQUIRE(vect["LAX"] > vect["ONT"]);
  REQUIRE(vect["PEK"] > vect["CMX"]);
}

TEST_CASE("Test Betweeness Centrality Algorithm for Small Non-US Airport Origins", "[part=1]") {
  Graph g = Graph(d);
  g.populateConnectionsIATA();
  auto vect = g.BetweenessCentrality("BWK");
  REQUIRE(vect["ZAG"] > vect["ORD"]);
  REQUIRE(vect["ZRH"] > vect["ORD"]);
  REQUIRE(vect["ZRH"] > vect["PPT"]);
}

TEST_CASE("Test Betweeness Centrality Algorithm for International Non-US Airport Origins", "[part=2]") {
  Graph g = Graph(d);
  g.populateConnectionsIATA();
  auto vect = g.BetweenessCentrality("PEK");
  REQUIRE(vect["ORD"] > vect["CNY"]);
}

TEST_CASE("Betweenness Centrality 1", "[part=1]") {
  Graph g = Graph(d);
  g.populateConnectionsIATA();
  std::string dest = "ORD";
  auto vector = g.BetweenessCentrality(dest);
  float lax = 0;
  for (auto a : vector) {
    if (a.first == "LAX") {
      lax = a.second;
    }
  }
  REQUIRE(lax > 1);
}

TEST_CASE("Betweenness Centrality 2", "[part=1]") {
  Graph g = Graph(d);
  g.populateConnectionsIATA();
  std::string dest = "ATL";
  auto vector = g.BetweenessCentrality(dest);
  float ewr = 0;
  for (auto a : vector) {
    if (a.first == "EWR") {
      ewr = a.second;
    }
  }
  REQUIRE(ewr > 1);
  float ord = 0;
  for (auto a : vector) {
    if (a.first == "ORD") {
      ord = a.second;
    }
  }
  REQUIRE(ord > 1);
}

TEST_CASE("Betweenness Centrality International", "[part=2]") {
  Graph g = Graph(d);
  g.populateConnectionsIATA();
  std::string dest = "LHR";
  auto vector = g.BetweenessCentrality(dest);
  float ewr = 0;
  for (auto a : vector) {
    if (a.first == "JFK") {
      ewr = a.second;
    }
  }
  REQUIRE(ewr > 1);
  float ord = 0;
  for (auto a : vector) {
    if (a.first == "LAX") {
      ord = a.second;
    }
  }
  REQUIRE(ord > 1);
}
