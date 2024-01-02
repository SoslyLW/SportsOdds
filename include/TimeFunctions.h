#ifndef TIMEFUNCTIONS_H
#define TIMEFUNCTIONS_H

#include <iostream>
#include <ctime>
#include <string>
#include <sstream>

class TimeFunctions {
    public:
        TimeFunctions();

        static tm makeDate(int day, int month, int year);
        static tm makeDate(std::string dateString);
        static std::string tmToString(tm dateStruct);
};

#endif // TIMEFUNCTIONS_H