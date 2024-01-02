#include "../include/TimeFunctions.h"

TimeFunctions::TimeFunctions() {
    //ctor
}

tm TimeFunctions::makeDate(int day, int month, int year) {
    tm ttm = {0};
    ttm.tm_mday= day;
    ttm.tm_mon= month-1;
    ttm.tm_year= year-1900;
    return ttm;
}

tm TimeFunctions::makeDate(std::string dateString) {
    int year, month, day;

    sscanf(dateString.c_str(), "%d-%d-%d", &year, &month, &day);

    // tm ttm = makeDate(day, month, year);
    tm ttm = {0};
    ttm.tm_mday= day;
    ttm.tm_mon= month-1;
    ttm.tm_year= year-1900;
    return ttm;
}

// Converts a tm struct into a date string with the form "YYYY-MM-DD"
std::string TimeFunctions::tmToString(tm dateStruct) {
    std::stringstream ss;
    std::string s = "";
    ss << (dateStruct.tm_year + 1900);
    ss << "-";
    if (dateStruct.tm_mon < 9) {
        ss << "0";
    }
    ss << dateStruct.tm_mon + 1;
    ss << "-";
    if (dateStruct.tm_mday < 10) {
        ss << "0";
    }
    ss << dateStruct.tm_mday;

    ss >> s;

    return s;
}