# CS 225 Final Project: Finding Best Connection Path Between Airports

## Table of Contents:
Group Members
Documentation
Description
Installing the Program
Features
Running the Program
Testing the Program
File Structure

## Group Members: 

- Michael Garbus (mgarbus2 at illinois dot edu)
- Rohith Sanjay (rohiths2 at illinois dot edu)
- Calvin Deering (deering3 at illinois dot edu)
- Justin Bai (justinb8 at illinois dot edu)

## Documentation:
Project Proposal: linked here: https://github.com/rohiths2/CS225-Final-Project/blob/main/CS225%20Project%20Proposal.pdf
Team Contract: linked here: https://github.com/rohiths2/CS225-Final-Project/blob/main/Group%20contract%20cs225.pdf
Written Report (contains Development and Results information): linked here:

## Description:
This application allows for multiple functions using airports from the OpenFlights airport/route database. These include finding the details of an airport, listing all other airports that have flights connecting to a certain airport, performing a Breadth-First Traversal (BFS) between two airports, finding the shortest connection path between two airports (minimizing both the number of stopovers and the length between one airport to the next) through Dijkstra's algorithm, and finding the likelihood of a person encountering a certain location as a layover between two airports (Betweenness Centrality using Johnson's algorithm). 
 
## Installing the Program:
Clone the repository by clicking the green Code button on the repository's home page, copying the HTTPS link, and pasting it into the code editor (e.g. VSCode). Alternatively, you can download the Git repository as a zip folder and open that folder in VSCode locally with the Docker container running.
Another way to clone the Git repository is by entering the following command in the VSCode terminal:
git clone https://github.com/rohiths2/CS225-Final-Project.git

## Features:
1. Given any airport's IATA code (3-letter abbreviation) as an input, show the details (Name, City, Country, Latitude, and Longitude) for the airport as the output.
2. Given any airport (IATA code as input), return a list of all possible connections (other airports that offer direct flights to a user's inputed airport)
3. Performing a Breadth-First Search between any two airports (given their IATA codes as inputs), or a Breadh-First Traversal given a starting airport IATA code as an input
4. Using Dijkstra's Algorithm, finding the shortest flight path between any two airports (given their IATA codes as inputs), considering the least number of connections and the smallest distance between airports
5. Using Betweenness Centrality (Johnson's Algorithm) to determine which airports a traveler will likely have a layover in, after computing posible shortest paths from an airport. 

## Running the Program:
Make sure the terminal is visible in VSCode and the CS225-Final-Project directory is opened. To run the program the first time, enter the following commands in the terminal (these 3 commands only need to be done the first time the program runs):

mkdir build

cd build

cmake .. 

Then, to run the program, enter these commands in the terminal:

make

./main

You will see the text "Parsing Data". The program is converting database text files into internal data structures. This will take about 3 seconds.

Follow the prompts given in the terminal. To find details of an airport, type 1 and then press enter. To display airports connected to a certain airport, type 2 and press enter. To run a Breadth-First Traversal, type 3 and then press enter. To find the shortest path between 2 airports using Dijkstra's algorithm, type 4 and press enter. To run the Betweenness Centrality algorithm, type 5 and then press enter. 

Enter the necessary Airport IATA codes as described on the terminal, then press enter. The IATA is the 3-letter identifying code for an airport (e.g. for O'Hare airport, it is ORD). IATA codes are NOT case-sensitive, except when instructed to type in either uppercase or lowercase. 

After entering the necessary information, the output of the program should be shown immediately (or take around 30 seconds for Dijkstra's algorithm to run).

The output of the program will be shown in the terminal. If this is a long list (e.g. BFS traversal or connection list, which displays one airport per line), you may need to scroll up on the terminal to view the full list.

To run the program again, type in anything and press enter. To stop running the program, press CTRL+C. 

## Testing the Program:
This application includes Catch Test Cases in the /tests/tests.cpp folder. To run all small-size and medium-size tests (tests for correct data parsing, BFS output, Dijkstra's output, and Betweenness Centrality output with airports from a single country), enter the following commands in the terminal:

make test

./test [part=1]

All of the domestic (small/medium) tests (part=1) should finish running in less than a minute.

To run ALL tests (including the large test cases involving airports from multiple countries at a time), enter the following commands in the terminal:

make test

./test

Large/international test cases may take up to one minute for each large/international test case.

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
- the Shortest Path feature (Dijkstra's algorithm implementation and sub-functions, used to find Intersection in airport sets and Distance between airports)
- Betweenness Centralty feature function
  
tests folder:

  tests.cpp file:
  - contains Catch Test Cases which proves that each algorithm works correctly
  - go to the Testing section of the readme for instructions on how to run the tests
  
entry folder:
  
  main.cpp file:
  - Contains the user_control() function which starts the command-line/terminal-based user interface (taking in commands through standard input std::cin, and returning the program's results using standard output std::cout)
  - Contains the main() function: the executable that runs the program in the terminal


