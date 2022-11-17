#include "../src/data_parser.h"
#include <iostream>
#include <algorithm>

bool isNumber(std::string& s) {
    return s.find_first_not_of("-1234567890");
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




std::string TrimRight(const std::string & str) {
    std::string tmp = str;
    return tmp.erase(tmp.find_last_not_of('"') + 1);
}

std::string TrimLeft(const std::string & str) {
    std::string tmp = str;
    return tmp.erase(0, tmp.find_first_not_of('"'));
}

std::string Trim(const std::string & str) {
    std::string tmp = str;
    return TrimLeft(TrimRight(tmp));
}

int SplitString(const std::string & str1, char sep, std::vector<std::string> &fields) {
    std::string str = str1;
    std::string::size_type pos;
    while((pos=str.find(sep)) != std::string::npos) {
        fields.push_back(str.substr(0,pos));
        str.erase(0,pos+1);  
    }
    fields.push_back(str);
    return fields.size();
}


void DataParser::populateAirportsDetails() {
    for (auto row : AirportsRows) {
        std::vector<std::string> splitRow;
        SplitString(row, ',', splitRow);
        std::vector<std::string> trimmedRow;
        for (size_t i = 0; i < splitRow.size(); ++i) {
            trimmedRow.push_back(Trim(splitRow[i]));
        }
        trimmedRow.push_back("1"); //The usability indicator: 0 if errors, 1 if no errors
        AirportsDetails.push_back(trimmedRow);
    }
}

void DataParser::populateRoutesDetails() {
   for (auto row : RoutesRows) {
        std::vector<std::string> splitRow;
        SplitString(row, ',', splitRow);
        std::vector<std::string> trimmedRow;
        for (size_t i = 0; i < splitRow.size(); ++i) {
            trimmedRow.push_back(Trim(splitRow[i]));
        }
        trimmedRow.push_back("1"); //The usability indicator: 0 if errors, 1 if no errors
        RoutesDetails.push_back(trimmedRow);
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
            if (isNumber(AirportsDetails[i][7]) && isNumber(AirportsDetails[i][8])) {
                AirportsDetails[i][6] = AirportsDetails[i][7];
                AirportsDetails[i][7] = AirportsDetails[i][8];
            } else {
                AirportsDetails[i][6] = '0';
                AirportsDetails[i][AirportsDetails[i].size()-1] = "0";
            }
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