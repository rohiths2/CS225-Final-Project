# CS 225 Final Project: Finding Best Connection Path Between Airports

## Table of Contents:
- Group Members
- Documentation
- Description
- Features
- Installing the Program
- Running the Program
- Output
- Testing the Program
- File Structure

## Group Members: 

- Michael Garbus (mgarbus2 at illinois dot edu)
- Rohith Sanjay (rohiths2 at illinois dot edu)
- Calvin Deering (deering3 at illinois dot edu)
- Justin Bai (justinb8 at illinois dot edu)

## Documentation:
Project Proposal: linked here: https://github.com/rohiths2/CS225-Final-Project/blob/main/CS225%20Project%20Proposal.pdf

Team Contract: linked here: https://github.com/rohiths2/CS225-Final-Project/blob/main/Group%20contract%20cs225.pdf

Written Report (contains Results information): linked here: https://github.com/rohiths2/CS225-Final-Project/blob/main/results.md

Report Video: https://mediaspace.illinois.edu/media/t/1_15le8f5h

## Description:
This application allows for multiple functions using airports from the OpenFlights airport/route database. These include finding the details of an airport, listing all other airports that have flights connecting to a certain airport, performing a Breadth-First Traversal (BFS) between two airports, finding the shortest connection path between two airports (minimizing both the number of stopovers and the length between one airport to the next) through Dijkstra's algorithm, and finding the likelihood of a person encountering a certain location as a layover between two airports (Betweenness Centrality using Brandes's algorithm). 

## Features:
1. Given any airport's IATA code (3-letter abbreviation) as an input, show the details (Name, City, Country, Latitude, and Longitude) for the airport as the output.
2. Given any airport (IATA code as input), return a list of all possible connections (other airports that offer direct flights to a user's inputed airport)
3. Performing a Breadth-First Search between any two airports (given their IATA codes as inputs), or a Breadh-First Traversal given a starting airport IATA code as an input
4. Using Dijkstra's Algorithm, finding the shortest flight path between any two airports (given their IATA codes as inputs), considering the least number of connections and the smallest distance between airports
5. Using Betweenness Centrality (Brandes's Algorithm) to determine which airports a traveler will likely have a layover in, after computing posible shortest paths from an airport. ***The output of this Betweenness Centrality function is found in /tests/btw.txt. Open the document and find (CTRL+F) for IATA codes with large values (greater than zero)***
 
## Installing the Program:
There are 3 ways you can install the program. Choose ONE of the 3 possible ways below. Make sure the Docker container is running.

1. Clone the Git repository is by entering the following command in the VSCode terminal:

`git clone https://github.com/rohiths2/CS225-Final-Project.git`

1. OR, you can clone the repository by clicking the green Code button on the repository's home page, copying the HTTPS link, and pasting it into the code editor (e.g. VSCode).

2. OR, you can download the Git repository as a zip folder and open that folder in VSCode locally with the Docker container running.

***After installing the program, go to the "Running the Program" section***


## Running the Program:
Make sure the terminal is visible in VSCode and the CS225-Final-Project directory is opened.

Running this program works just like the MPs and Labs in this class. Steps to run the program are listed below.

### CMake Setup: ONLY for the FIRST TIME opening the program

To run the program the first time (these steps only need to be done the first time the program runs):

Enter the following commands in the terminal 

`mkdir build`

`cd build`

`cmake .. `

### Compiling (building) the program

To build the program, enter the following command in the terminal:

`make`

"make" should be run the first time, and also every other time the code is edited/modified. This compiles the program

### Executing the program

Then, to run the program, enter this command in the terminal:

`./main`

"./main" should be run every time (not just the first time) in order to start a new instance of the program

### Using the program (follow the terminal instructions)

Type the relative path for the Airports data file name. **OR just type "default" (no quotes) to use the default airports.dat file from OpenFlights (this is recommended)**. Then press enter.

Then, type the relative path for the Routes data file name. **OR just type "default (no quotes) to use the default routes.dat file from OpenFlights (this is recommended)**. Then press enter.

(NOTE: If you're using a different file than the included airports.dat/routes.dat, then make sure the file is located somewhere in the CS225-Final-Project folder. Example file path: `"../lib/airports.dat"`)

***Follow the prompts given in the terminal.*** Type either 1, 2, 3, 4, or 5 depending on what you want the program to do ((1) find details about an airport, (2) display connected airports, (3) run BFS, (4) run Dijkstra's, or (5) run Betweenness Centrality)

The terminal will ask to enter IATA codes for airports. These are the 3-letter identification codes. So for O'Hare, it is "ORD". For Champaign, it is "CMI". It doesn't matter if you enter it uppercase/lowercase unless specified otherwise in the terminal. 

Wait for the desired algorithm to run (should finish within just a few seconds). 

***Note: If you ran the Betweenness Centrality, see the Betweenness Centrality output description in the Output section below.***

## Output

Output is displayed in the terminal AND in the output.txt file (CS225-Final-Project/build/output.txt):

- ***Terminal Output:*** The output of the program will be shown in the terminal. If this is a long list (e.g. BFS traversal or connection list, which displays one airport per line), you may need to scroll up on the terminal to view the full list.

- ***Text File Output:*** The output of the algorithms is ALSO found in the "output.txt" file in the build folder (/build/output.txt). Scroll down to find the most recent output. You will see the text "Opened Output File" indicating the start of each program run. To clear the output file, just navigate to output.txt, and use backspace or delete to remove text. 

To run the program again, type in anything and press enter. To stop running the program, press CTRL+C. 

### NOTE: Betweenness Centrality Output
Output for the Betweenness Centrality function is found in /tests/btw.txt (NOT in output.txt) due to the large size of the Betweenness Centrality output list.
Open this file, do Find (Ctrl+F) for a layover destination IATA code (e.g. "LAX"). If this value is greater than zero, it is very likely to encounter a layover at this airport. 

## Testing the Program:
This application includes Catch Test Cases in the /tests/tests.cpp folder. To run all small-size and medium-size tests (tests for correct data parsing, BFS output, Dijkstra's output, and Betweenness Centrality output with airports from a single country), enter the following commands in the terminal:

`make test`

`./test [part=1]`

All of the domestic (small/medium) tests (part=1) should finish running in less than a minute.

To run ALL tests (including the large test cases involving airports from multiple countries at a time), enter the following commands in the terminal:

`make test`

`./test`

## File Structure:
src folder:

data_parser.h, data_parser.cpp: 

- contains functions required to read the .dat database files in a CSV format
- contains functions that split the a string by commas, converting a CSV file into 2D vectors
- contains a function for checking for invalid information (either missing/blank cells, or non-numbers in number attributes), replacing them appropriately.

graph.h, graph.cpp: 

- contains the Airport class with the following parts:
- variables representing an airport's details (name, city, country, etc.)
- ==operators to determine whether two Airport objects are the same
- function that returns an Airport type (with all details) from a string (IATA code) as the input

contains the Graph class with the following parts:
- the adjacency list (map<string, vector<string>>) representing each airport's connections (direct destinations)
- a BFS function to run a Breadth-First Traversal or Breadth-First Search between two airports
- the Shortest Path feature (Dijkstra's algorithm implementation and sub-functions, used to find the priority airport within a heap and Distance between airports)
- Betweenness Centralty feature function

build folder:
- output.txt: contains the terminal's output of the last run of the program
  
tests folder:

  tests.cpp file:
  - contains Catch Test Cases which proves that each algorithm works correctly
  - go to the Testing section of the readme for instructions on how to run the tests
  
  btw.txt:
  - contains full Betweenness Centrality output. Shows the airport's IATA code, with a float (a large number, above 0, means that it is fairly likely to encounter a layover to the corresponding airport)
  
entry folder:
  
  main.cpp file:
  - Contains the user_control() function which starts the command-line/terminal-based user interface (taking in commands through standard input std::cin, and returning the program's results using standard output std::cout)
  - Contains the main() function: the executable that runs the program in the terminal


