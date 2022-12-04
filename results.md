# Written Report

## Results: Data Parsing
Our data parsing (from a text file to a 2D vector) works correctly as expected. First, our program populates a 1D vector with each element representing a row of the data file. From the first test case in tests.cpp, the number of rows in the Airports/Routes data sets are equal to the sizes of the Airports/Routes Rows vectors, respectively, proving that this component works. 

Next, our program splits each row into airport details (name, city, country, IATA, latitude, longitude, etc.) based on commas (since it's a CSV file). So the 1D Vector (of rows) is converted into a 2D vector (of details). After running the second test case in tests.cpp, each row of the Airport 2D vector has size 15, which is equal to one more than the number of commas in a row on the original Airports text file, indicating that the CSV splitting worked correctly. Similarly, each row of our Routes 2D vector has size 10, which is one more than the number of commas in a row on the original Routes text file, indicating that the CSV splitting worked correctly. 

The size values are correct for multiple rows (not just the first one), proving that our program is able to handle special data parsing cases (e.g. where a comma may be inside the name of the airport, so we only split if the comma is right next to quotation marks or numbers).

## Results: Making the Graph

## Results: Makefile

## Results: Breadth-First Search/Traversal
### Example Output

## Results: Shortest Path Algorithm (Dijkstra's)
### Example Output

## Results: Betweenness Centrality Algorithm
### Example Output
