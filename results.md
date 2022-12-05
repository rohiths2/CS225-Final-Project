# Written Report

## Answer to Leading Question:
Our leading question asked how we can determine the most efficient way to travel between two airports. From our program's successful results, we determined that **populating a map through Dijkstra's algorithm and iterating through that map can help determine the path with the least number of stop-overs, least distance (and thus least flying time), and best probability of remaining inside a certain country--all of which contribute to the efficiency of a route.** Additionally, a Betweenness-Centrality algorithm (using Dijkstra's) can help solve this problem even in special cases (such as with complex graphs). We learned that these algorithms make it possible to calculate the best route between two airports both domestically and internationally (larger routes), exceeding our original expectation. 

## Results: Data Parsing
Our data parsing (from a text file to a 2D vector) works correctly as expected. First, our program populates a 1D vector with each element representing a row of the data file. From the first test case in tests.cpp, the number of rows in the Airports/Routes data sets are equal to the sizes of the Airports/Routes Rows vectors, respectively, **proving that this component works**. 

Next, our program splits each row into airport details (name, city, country, IATA, latitude, longitude, etc.) based on commas (since it's a CSV file). So the 1D Vector (of rows) is converted into a 2D vector (of details). After running the second test case in tests.cpp, each row of the Airport 2D vector has size 15, which is equal to one more than the number of commas in a row on the original Airports text file, indicating that the CSV splitting worked correctly. Similarly, each row of our Routes 2D vector has size 10, which is one more than the number of commas in a row on the original Routes text file, **indicating that the CSV splitting worked correctly**. 

The size values are correct for multiple rows (not just the first one), **proving that our program is able to handle special data parsing cases** (e.g. where a comma may be inside the name of the airport, so we only split if the comma is right next to quotation marks or numbers).

## Results: Making the Graph
### Graph Initialization
After calling the Graph's constructor, from the 3rd test case in tests.cpp, the number of rows in the graph's adjacency list is equal to the number of rows in the Airports Details vector, meaning that the graph is initialized correctly. Also, after populating the adjacency list map (calling populateConnectionsIATA() function), the number of connections for many major airports (ORD, ATL, LAX, etc.) have sizes of over 100, meaning that **the program is correctly adding all the connected airports to the map.**

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

The adjacency list at SBH includes multiple airports, and there are no repeated values. In addition, when going through the actual routes.dat text file, the routes which use the SBH airport include PTP, ANU, SFG, SXM, SJU, STT, and SAB--which is exactly the same as the output from the adjacency list. **This proves that the graph creation was successful. **

## Results: Makefile/Deliverables
- The program **executes without compilation errors. **
- The **commands "make", "./main", and "./test" work as intended** (running the main function and testing the code, respectively), so our makefile is configured correctly.
- **Correct output from the terminal is written to an output.txt file** in the build folder (e.g. when BFS is run, the same BFS output on the terminal is present in the output.txt file. The output.txt file is up-to-date every time a new function runs)

### Example Output
The program is run to display details about the IATA string "LAX"

The terminal output:

`Details for LAX:`

`Name: Los Angeles International Airport --- City: Los Angeles --- Country United States --- Latitude 33.9425 --- Longitude -118.408`

The content of the "output.txt" file:

`Finding details of an airport, given an IATA string`

`Details for LAX:`

`Name: Los Angeles International Airport --- City: Los Angeles --- Country United States --- Latitude 33.9425 --- Longitude -118.408`

Both the terminal output and the output.txt file includes the same details, **proving that writing to the output file works successfully as intended.**

## Results: Breadth-First Search/Traversal
### Example Output (BFS from SAB to SAH - small): content of "output.txt" file
![alt text](https://github.com/rohiths2/CS225-Final-Project/blob/main/Images%20of%20Examples/sample%20BFS%20results%20(SAB-SBH).JPG)

### Conclusion (BFS from SAB to SAH - small)
In the original routes.dat text file, there are more than 20 lines which include "SAB" and "SBH" (source and destination airports)--meaning there should be more than 20 airports included in the BFS search. From the above BFS output, there is a large number (more than 20) of airports included in the result, which means that the program is using all the connected airports (or "neighbors") both directly and indirectly related to either SAB or SBH. Also, there are no repeated airports (each airport is only displayed once), which is an important aspect of Breadth-First Traversals and Searches. The BFS starts with the correct airport ("SAB") and ends with the intended stopping point ("SBH"), working as expected. These conditions are tested in the 7th test case in the tests.cpp file. All these conditions **prove that our program is computing the BFS successfully and correctly. **

### Conclusion (BFS from ORD to LAX - larger)
For a larger-scale example, ORD and LAX are IATA codes of two of the U.S.'s largest airports, and running a BFS between them gives an output of multiple airports (more than 50 total) that represent the connections from ORD and connections from LAX together. The BFS starts at "ORD" and ends at "LAX". There are no repeated values (each airport is only visited once). Therefore, from these same conditions discussed previously, our **BFS feature works on a larger example as well. **

## Results: Shortest Path Algorithm (Dijkstra's)
### Example Output (Dijkstra's algorithm from SHE to DOY - small): content of "output.txt" file
`Dijkstra's algorithm `

`Shortest Path from SHE to DOY: `

`SHE (Taoxian Airport) `

`PEK (Beijing Capital International Airport) `

`DOY (Dongying Shengli Airport) `

### Conclusion (Dijkstra's algorithm from SHE to DOY - small)
From the above output, our Shortest Path feature from two small airports (both within the same country, China) returns the starting airport's IATA string correctly ("SHE") and the ending airport's IATA string correctly ("DOY"). Also, from the original Routes.dat file, there is a route including PEK from both SHE and from DOY, meaning PEK is a common neighbor of both airports and is therefore an appropriate layover location. Also, many online flight booking sites (e.g. Google Flights) suggest routes (from SHE to DOY) with layovers at PEK--demonstrating two ways that prove our program is finding the correct layover location. These assertions are tested in the 15th test case ("Dijkstra's Algorithm Small") in tests.cppAlso, our result only includes one layover (PEK), even though there are routes with two or more layovers possible, which **proves our program is correctly calculating the most efficient route between the two airports. **

### Example Output (Dijkstra's algorithm from PVD to GDV - medium): content of "output.txt" file

`PVD (Theodore Francis Green State Airport) `

`DTW (Detroit Metropolitan Wayne County Airport) `

`MSP (Minneapolis-St Paul International/Wold-Chamberlain Airport) `

`BIL (Billings Logan International Airport) `

`GDV (Dawson Community Airport) `

### Conclusion (Dijkstra's algorithm from PVD to DTW - medium)
In this case, we are finding the shortest path between PVD and GDV: IATA codes for smaller airports both in the United States. This route requires more layovers, so this case tests if our program can handle more complex routes with more calculations (resulting in more layovers). Our program's result includes the correct first airport (PVD) and correct last airport (GDV), along with 3 airports in the middle (DTW, MSP, and BIL), which proves that our program was able to compute a larger Dijkstra's call completely without crashing or aborting. These assertions are tested in the 16th test case in tests.cpp ("Dijkstra's Algorithm Medium). Also, the airports used for layovers (DTW/MSP) are larger airports (making sense for a stopover) and are located in the same country/region, **further proving that our algorithm works** for this example.

### Conclusion (Other medium-sized Dijkstra's algorithm cases)
In this case, we are testing whether commonly direct routes (from two major airports) do not include layovers after calculating the shortest path. Testing our algorithm from "ORD" to "CLT" results in output of only those two airports, and testing the algorithm from "LAX" to "FLL" results in output of only those two airports, **proving that our function returns the most efficient path** (does not include layovers if direct routes are possible). These assertions are tested and passed in the 17th test case.

### Conclusion (Dijkstra's Algorithm Large/International Cases)
We are testing our algorithm on a large-scale example, from the United States's east coast to Australia ("EWR" to "SYD"). Logically, the best place to stop would be on the west coast of the United States, most likely California (because California has relatively larger airports, such as "LAX" or "SFO"). Our program's output returns a result with one stop at "SFO" (San Francisco airport, which is located in California). Therefore, our **program is correctly able to find the shortest path even between two different countries.**

### Conclusion (Dijkstra's Algorithm Sub-Functions)
Our Dijkstra's implementation includes sub-functions that calculate the distance between two airports, find the intersection (common airports) between two vectors, and remove the smallest airport from a vector. Test cases 9 through 14 on tests.cpp test each of these sub-functions. These tests pass, **proving that our program is able to calculate the shortest path correctly** through Dijkstra's algorithm (as expected) and not through another way. 


## Results: Betweenness Centrality Algorithm
### Example Output
