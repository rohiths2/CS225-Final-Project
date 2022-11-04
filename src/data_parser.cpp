#include "../src/data_parser.h"
#include <iostream>

void DataParser::populateAirportRows(const std::string& filename) {
    std::ifstream wordsFile(filename);
    std::string line;
    if (wordsFile.is_open()) {
        /* Reads a line from `wordsFile` into `word` until the file ends. */
        while (std::getline(wordsFile, line)) {
            AirportsRows.push_back(line);
        }
    }
}



void DataParser::populateRoutesRows(const std::string& filename) {
    std::ifstream wordsFile(filename);
    std::string line;
    if (wordsFile.is_open()) {
        /* Reads a line from `wordsFile` into `word` until the file ends. */
        while (std::getline(wordsFile, line)) {
            RoutesRows.push_back(line);
        }
    }
}


std::vector<std::string> Split(const std::string& str, char delimiter) {
  size_t last = 0;
  std::vector<std::string> substrs;
  for (size_t i = 0; i != str.length(); ++i) {
    if (str.at(i) == delimiter || str.at(i) == '\r') {
      std::string substr = str.substr(last, i - last);
      last = i + 1;
      substrs.push_back(substr);
    }
  }

  if (last != str.size()) {
    std::string substr = str.substr(last, str.length() - last);
    substrs.push_back(substr);
  }

  return substrs;
}


void DataParser::populateAirportsDetails() {
    for (auto row : AirportsRows) {
        std::vector<std::string> splitRow = Split(row, ',');
        AirportsDetails.push_back(splitRow);
    }
}

void DataParser::populateRoutesDetails() {
    for (auto row : RoutesRows) {
        std::vector<std::string> splitRow = Split(row, ',');
        RoutesDetails.push_back(splitRow);
    }
}