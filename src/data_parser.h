#pragma once
#include <string>
#include <vector>
#include <fstream>

class DataParser {
public:
    //Default Constructor
    DataParser() {
    }

    //Vector of strings: each string represents each whole row of the airports.dat text file
    std::vector<std::string> AirportsRows;

    //2D vector: each cell represents each detail of the airport, separated by quotes/numbers and commas
    std::vector<std::vector<std::string>> AirportsDetails;

    //Vector of strings: each string represents each whole row of the routes.dat text file
    std::vector<std::string> RoutesRows;

    //2D vector: each cell represents each detail of the route, separated by quotes/numbers and commas
    std::vector<std::vector<std::string>> RoutesDetails;

    //function that takes in the file and pushes each row to the AirportsRows vector
    //Input: string of the filename
    //Output: none, but directly changes the AirportsRows vector
    void populateAirportRows(const std::string& filename);

    //Splits each string in AirportsRows by comma and puts each string-part into AirportsDetails 2d vector
    //can ONLY be done AFTER calling populateAirportsRows()
    //Input: none, but uses the AirportsRows vector as a reference
    //Output: none, but directly changes the AirportsDetails vector
    void populateAirportsDetails();

    //Splits each string in RoutesRows by comma and puts each string-part into RoutesDetails 2d vector
    //can ONLY be done AFTER calling populateRoutesRows()
    //Input: none, but uses the RoutesRows vector as a reference
    //Output: none, but directly changes the RoutesDetails vector
    void populateRoutesDetails();

    //function that takes in the file and pushes each row to the RoutesRows vector
    //Input: string of the filename
    //Output: none, but directly changes the RoutesRows vector
    void populateRoutesRows(const std::string& filename);

    //Used for data cleaning.
    //Checks if there are any blank values in AirportsDetails: if so, marks them as null and marks the row as "unusable". 
    //Also checks if latitude and longitude are actually numbers using the isNumber function on the cpp file
    //Input: none, but uses the AirportsDetails vector as a reference and modifies it
    //Output: none, but modifies the AirportsDetails vector directly
    void checkMissingInfo();
};