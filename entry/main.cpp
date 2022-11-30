#include <iostream>
#include "../src/data_parser.cpp"
#include "../src/graph.cpp"


//The below function represents the terminal command-line user interface 
//This allows the user to run the program with custom inputs, and is called continuously in main until the program is terminated
void user_control() {
  //Prepares AirportsDetails vector, RoutesDetails vector, and Graph immediately
  std::cout << "Parsing data..." << std::endl;
  DataParser d;
  d.populateAirportRows("../lib/airports.dat");
  d.populateRoutesRows("../lib/routes.dat");
  d.populateAirportsDetails();
  d.populateRoutesDetails();
  d.checkMissingInfo();
  Graph g = Graph(d);

  //Allows user to choose desired task (details, connections, BFS, Dijkstra's, or Betweeness Centrality algorithms)
  char option; //character representing the user's selected option 1, 2, 3, or 4
  std::cout << "Please type the number corresponding to what you would like to do, then press enter:" << std::endl;
  std::cout << "1 = Find details about an airport" << std::endl; 
  std::cout << "2 = Display connecting airports from a certain airport" << std::endl;
  std::cout << "3 = Run a Breadth-First Traversal (BFS) between two airports" << std::endl;
  std::cout << "4 = Find the Shortest Connection Path between two airports (Dijkstra's algorithm)" << std::endl;
  std::cin >> option;

  //Finding details of an airport, given an IATA string
  if (option == '1') {
    std::string iata;
    std::cout << "Type the 3-letter airport code (IATA), then press enter" << std::endl;
    std::cin >> iata;
    for (size_t i = 0; i < iata.size(); ++i) {
      iata[i] = toupper(iata[i]);
    }

    char detail_type; //character representing the user's selected detail type 1, 2, 3, or 4
    std::cout << "Type the number corresponding to which detail to find:" << std::endl;
    std::cout << "1 = Airport Name, 2 = Location (city/country), 3 = Location (latitude/longitude), 4 = All Details" << std::endl;
    std::cin >> detail_type;
    std::cout << "Details for " << iata << ":" << std::endl;

    Graph::Airport a = g.getAirportFromIATA_(iata);
    if (detail_type == '1') { //prints only the name to standard output
        std::cout << a.name_ << std::endl;
    } else if (detail_type == '2') { //prints only the location (in words, or city/country) to standard output
        std::cout << "City: " << a.city_ << "; Country: " << a.country_ << std::endl;
    } else if (detail_type == '3') { //prints only the numerical location (latitude/longitude) to standard output
        std::cout << "Latitude: " << a.latitude_ << "; Longitude: " << a.longitude_ << std::endl;
    } else if (detail_type == '4') { //prints all of the above details to standard output
        std::cout << "Name: " << a.name_ << " --- City: " << a.city_ << " --- Country " << a.country_ << " --- Latitude " << a.latitude_ << " --- Longitude " << a.longitude_ << std::endl;
    } else {
      std::cout << "Invalid input" << std::endl;
    }

    std::cout << std::endl;

  //List the connected airports to a certain airport written into standard input
  } else if (option == '2') {
    std::string iata;
    std::cout << "Type the 3-letter airport code (IATA), then press enter" << std::endl;
    std::cin >> iata;
    for (size_t i = 0; i < iata.size(); ++i) {
      iata[i] = toupper(iata[i]);
    }

    Graph::Airport a = g.getAirportFromIATA_(iata);
    std::string country = a.country_;
    g.populateConnectionsIATA_country(d, country);
    std::cout << "Connections from " << iata << ":" << std::endl;
    std::vector<std::string> vect = g.getConnectionsIATA().find(iata)->second;
    for (auto str : vect) {
        std::cout << str << " (" << g.getAirportFromIATA_(str).name_ << ")" << std::endl;
    }
    std::cout << std::endl;

  //Performs a BFS Traversal from a Starting and Ending airport
  } else if (option == '3') {
    std::string country;
    g.populateConnectionsIATA(d);

    std::string source;
    std::string dest;
    std::cout << "Type the source airport's 3-letter (IATA) code: " << std::endl;
    std::cin >> source;
    for (size_t i = 0; i < source.size(); ++i) {
      source[i] = toupper(source[i]);
    }
    std::cout << "Type the destination airport's 3-letter (IATA) code:" << std::endl;
    std::cin >> dest;
    for (size_t i = 0; i < dest.size(); ++i) {
      dest[i] = toupper(dest[i]);
    }

    std::cout << std::endl;
    char complete;
    std::cout << "Would you like to show only complete airports or all airports in the traversal?" << std::endl;
    std::cout << "1 = show only complete airports (those with no missing info), 2 = show all airports (even those with missing details)" << std::endl;
    std::cin >> complete;
    if (complete == '1') {
        std::cout << "BFS Traversal from " << source << " to " << dest << std::endl;
        g.BFS(source, dest, true);
    } else if (complete == '2') {
        std::cout << "BFS Traversal from " << source << " to " << dest << std::endl;
        g.BFS(source, dest, false);
    } else {
        std::cout << "Invalid input" << std::endl;
    }
    std::cout << std::endl;

  //Performs a Dijkstra's algorithm and lists the shortest path from a source and destination airport
  //Allows the user to only involve a certain country in this algorithm to make calculations faster
  } else if (option == '4') {
    std::string country;
    std::cout << "Are the two airports located in the same country? If so, type the country, and if not, type no" << std::endl;
    std::cout << "(capitalize the first letter of each word when typing the country)" << std::endl;
    std::cout << "Typing US (all uppercase) also means United States" << std::endl;
    std::cin >> country;
    if (country == "US") {
      country = "United States";
    }
    if (country == "no") {
        g.populateConnectionsIATA(d);
    } else {
        g.populateConnectionsIATA_country(d, country);
    }

    std::string source;
    std::string dest;
    std::cout << "Type the 3-letter IATA code for the source airport:" << std::endl;
    std::cin >> source;
    for (size_t i = 0; i < source.size(); ++i) {
      source[i] = toupper(source[i]);
    }
    
    std::cout << "Type the 3-letter IATA code for the destination airport:" << std::endl;
    std::cin >> dest;
    for (size_t i = 0; i < dest.size(); ++i) {
      dest[i] = toupper(dest[i]);
    }
    std::vector<std::string> shortest_path = g.shortestPathIATA(source, dest);
    std::cout << std::endl;
    std::cout << "Shortest Path from " << source << " to " << dest << ": " << std::endl;
    for (auto s : shortest_path) {
      std::cout << s << " (" << g.getAirportFromIATA_(s).name_ << ")" << std::endl;
    }

    std::cout << std::endl;

  } else {
    std::cout << "Invalid input option" << std::endl;
  }

  char z;
  std::cout << "Your output result is shown above" << std::endl;
  std::cout << "Type in anything and press enter to run the program again. Press CTRL+C to stop." << std::endl;
  std::cin >> z;
}





int main() {


while(true) {
  user_control();
}
return 1;
}
