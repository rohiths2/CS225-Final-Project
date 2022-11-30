# CS 225 Final Project: Finding Best Connection Path Between Airports

## Members: 

- Michael Garbus (mgarbus2 at illinois dot edu)

- Rohith Sanjay (rohiths2 at illinois dot edu)

- Calvin Deering (deering3 at illinois dot edu)

- Justin Bai (justinb8 at illinois dot edu)

## Description:
This application allows for multiple functions using airports from the OpenFlights airport/route database. These include finding the details of an airport, listing all other airports that have flights connecting to a certain airport, performing a Breadth-First Traversal (BFS) between two airports, finding the shortest connection path between two airports (minimizing both the number of stopovers and the length between one airport to the next) through Dijkstra's algorithm, and finding the likelihood of a person encountering a certain location as a layover between two airports (Betweenness Centrality using Johnson's algorithm). 

## Installing the Program:
Clone the repository by clicking the green Code button on the repository's home page, copying the HTTPS link, and pasting it into the code editor (e.g. VSCode). Alternatively, you can download the Git repository as a zip folder and open that folder in VSCode locally with the Docker container running.

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
make
./test [part=1]

All of the domestic (small/medium) tests (part=1) should finish running in less than a minute.

To run ALL tests (including the large test cases involving airports from multiple countries at a time), enter the following commands in the terminal:
make
./test

Large/international test cases may take up to one minute for each large/international test case.

