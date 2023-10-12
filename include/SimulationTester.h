#ifndef SIMULATIONTESTER_H
#define SIMULATIONTESTER_H

#include "SimulationDB.h"
#include <chrono>

using namespace std::chrono;

class SimulationTester
{
    public:
        SimulationTester(SimulationDB sim, League league);

        void runTestSuite();
        void printTestResultsToFile(string filename, int resultsMatrix[][2], int numTests);

        string getTestName() { return testName; };

    private:
        SimulationDB simulationToTest;
        League leagueToTest;
        string testName;
};

#endif // SIMULATIONTESTER_H
