#include "../src/data_parser.h"
#include <iostream>
#include <algorithm>

//checks if all characters of a string are either digits or the - sign
//helps determine if latitude and longitude values are correct, and if a string can be split with a comma next to a number

bool isNumber(std::string& s) {
    return s.find_first_not_of("-1234567890");
}


//function that takes in the file and pushes each row to the AirportsRows vector
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


//function that takes in the file and pushes each row to the RoutesRows vector
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


//The following functions are similar to ones from MP Schedule
//Trim functions remove any quotation marks from a string (as the dat file contains quotes which should not be in the vectors)
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

//SplitString looks for each instance of "sep" character (commas, if there are quotes/numbers around them) and splits it
//Puts each "part" of the string into the fields vector
int SplitString(const std::string & str1, char sep, std::vector<std::string> &fields) {
    /*This will only count the delimiter if there are not quotes on the left or right side
        ex: {"exam,ple"} will not seperate, but {exam,ple}, {"exam",ple}, {"exam","ple"} will seperate*/
    std::string str = str1;
    size_t index = 0;
    //std::string::size_type pos;
    bool encounter_quote = false;
    while(index < str.size()) {
        if (str[index] == sep && encounter_quote == false) {
            fields.push_back(str.substr(0, index));
            str.erase(0, index+1);
            index = 0;
        } else {
            if (str[index] == '\"') {
                encounter_quote = !encounter_quote;
            }
            index++;
        }
    }
    fields.push_back(str);
    return fields.size();
}

//Splits each string in AirportsRows by comma and puts each string-part into AirportsDetails 2d vector
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

//Splits each string in RoutesRows by comma and puts each string-part into RoutesDetails 2d vector
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


// Used for data cleaning.
// Checks if there are any blank values in AirportsRows: if so, marks them as null and marks the row as "unusable". 
// Also checks if latitude and longitude are actually numbers.
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