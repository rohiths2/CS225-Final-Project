# Written Report

## Results: Data Parsing
Our data parsing (from a text file to a 2D vector) works correctly as expected. First, our program populates a 1D vector with each element representing a row of the data file. From the first test case in tests.cpp, the number of rows in the Airports/Routes data sets are equal to the sizes of the Airports/Routes Rows vectors, respectively, proving that this component works. 

Next, our program splits each row into airport details (name, city, country, IATA, latitude, longitude, etc.) based on commas (since it's a CSV file). So the 1D Vector (of rows) is converted into a 2D vector (of details). After running the second test case in tests.cpp, each row of the Airport 2D vector has size 15, which is equal to one more than the number of commas in a row on the original Airports text file, indicating that the CSV splitting worked correctly. Similarly, each row of our Routes 2D vector has size 10, which is one more than the number of commas in a row on the original Routes text file, indicating that the CSV splitting worked correctly. 

The size values are correct for multiple rows (not just the first one), proving that our program is able to handle special data parsing cases (e.g. where a comma may be inside the name of the airport, so we only split if the comma is right next to quotation marks or numbers).

## Results: Making the Graph
### Graph Initialization
After calling the Graph's constructor, from the 3rd test case in tests.cpp, the number of rows in the graph's adjacency list is equal to the number of rows in the Airports Details vector, meaning that the graph is initialized correctly. Also, after populating the adjacency list map (calling populateConnectionsIATA() function), the number of connections for many major airports (ORD, ATL, LAX, etc.) have sizes of over 100, meaning that the program is correctly adding all the connected airports to the map. 

### Adjacency List Content
#### Example Output
The following terminal output shows the connections from "SBH" (an IATA code for an airport). So if SBH is a key in the adjacency list, the value would be a vector with the following airports:

`Connections from SBH:`

`PTP (Pointe-à-Pitre Le Raizet)`

`ANU (V.C. Bird International Airport)`

`SFG (L'Espérance Airport)`

`SXM (Princess Juliana International Airport)`

`SJU (Luis Munoz Marin International Airport)`

`STT (Cyril E. King Airport)`

`SAB (Juancho E. Yrausquin Airport)`

The adjacency list at SBH includes multiple airports, and there are no repeated values. In addition, when going through the actual routes.dat text file, the routes which use the SBH airport include PTP, ANU, SFG, SXM, SJU, STT, and SAB--which is exactly the same as the output from the adjacency list. This proves that the graph creation was successful. 

## Results: Makefile/Deliverables
- The program executes without compilation errors. 
- The commands "make", "./main", and "./test" work as intended (running the main function and testing the code, respectively), so our makefile is configured correctly.
- Correct output from the terminal is written to an output.txt file in the build folder (e.g. when BFS is run, the same BFS output on the terminal is present in the output.txt file. The output.txt file is up-to-date every time a new function runs)

### Example Output
The program is run to display details about the IATA string "LAX"

The terminal output:

`Details for LAX:`

`Name: Los Angeles International Airport --- City: Los Angeles --- Country United States --- Latitude 33.9425 --- Longitude -118.408`

The content of the "output.txt" file:

`Finding details of an airport, given an IATA string`

`Details for LAX:`

`Name: Los Angeles International Airport --- City: Los Angeles --- Country United States --- Latitude 33.9425 --- Longitude -118.408`

Both the terminal output and the output.txt file includes the same details, proving that writing to the output file works successfully as intended.

## Results: Breadth-First Search/Traversal
### Example Output

## Results: Shortest Path Algorithm (Dijkstra's)
### Example Output

## Results: Betweenness Centrality Algorithm
### Example Output
