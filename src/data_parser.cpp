#include "../src/data_parser.h"
#include <iostream>
#include <algorithm>

bool isNumber(std::string s) {
    for (char c : s) {
        if (!isdigit(c) && c != '-') {
            return false;
        }
    }
    return true;
}

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
            substr.erase(std::remove(substr.begin(), substr.end(), '"'), substr.end());
            substrs.push_back(substr);
        }
  }

  if (last != str.size()) {
    std::string substr = str.substr(last, str.length() - last);
    substrs.push_back(substr);
    substr.erase(std::remove(substr.begin(), substr.end(), '"'), substr.end());
  }

  return substrs;
}


void DataParser::populateAirportsDetails() {
    for (auto row : AirportsRows) {
        std::vector<std::string> splitRow = Split(row, ',');
        splitRow.push_back("1"); //The usability indicator: 0 if errors, 1 if no errors
        AirportsDetails.push_back(splitRow);
    }
}

void DataParser::populateRoutesDetails() {
    for (auto row : RoutesRows) {
        std::vector<std::string> splitRow = Split(row, ',');
        splitRow.push_back("1"); //The usability indicator: 0 if errors, 1 if no errors
        RoutesDetails.push_back(splitRow);
    }
}


// The U.S. x coordinates are betwen -126 and -66, and y coordinates are between 24 and 50


void DataParser::checkMissingInfo() { //iterate through Routes Details
    for (size_t i = 0; i < AirportsDetails.size(); ++i) {
        for (auto element : AirportsDetails[i]) {
            if (element == "") {
                element = "NULL";
                AirportsDetails[i][AirportsDetails[i].size()-1] = "0"; //makes this row vector unusable
            }
        }
        //the following two if blocks replace any invalid 6 and 7 with 0
        if (!isNumber(AirportsDetails[i][6])) {
            AirportsDetails[i][6] = '0';
            AirportsDetails[i][AirportsDetails[i].size()-1] = "0";
        }
        if (!isNumber(AirportsDetails[i][7])) {
            AirportsDetails[i][7] = '0';
            AirportsDetails[i][AirportsDetails[i].size()-1] = "0";
        }
    }
    for (size_t i = 0; i < RoutesDetails.size(); ++i) { //iterate through Airports Details
        for (auto element : RoutesDetails[i]) {
            if (element == "") {
                element = "NULL";
                RoutesDetails[i][RoutesDetails[i].size()-1] = "0"; //makes this row vector unusable
            }
        }
    }
}