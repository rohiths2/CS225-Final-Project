#include "../src/data_parser.h"
#include <iostream>

void DataParser::populateAirportRows(std::string filename) {
    std::ifstream wordsFile(filename);
    std::string line;
    if (wordsFile.is_open()) {
        /* Reads a line from `wordsFile` into `word` until the file ends. */
        while (std::getline(wordsFile, line)) {
            std::cout << line << std::endl;
            AirportsRows.push_back(line);
        }
    }
}

void DataParser::populateRoutesRows(std::string filename) {
    std::ifstream wordsFile(filename);
    std::string line;
    if (wordsFile.is_open()) {
        /* Reads a line from `wordsFile` into `word` until the file ends. */
        while (std::getline(wordsFile, line)) {
            RoutesRows.push_back(line);
        }
    }
}