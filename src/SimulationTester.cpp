#include "SimulationTester.h"


SimulationTester::SimulationTester(SimulationDB sim, League league)
{
    //ctor
    simulationToTest = sim;
    leagueToTest = league;
    char *cString = (char*) malloc(18 * sizeof(char));
    const time_t startTime = time(NULL);
    tm *timeStruct = gmtime(&startTime);

    strftime(cString, 20, "%Y-%m-%d_%H%M%S", timeStruct);
    testName = "Test_";
    testName += cString;

    free(cString);
}

void SimulationTester::runTestSuite()
{
    int trialArray[] = {100, 500, 1000, 10000};
    int n = sizeof(trialArray) / sizeof(trialArray[0]);
    int resultsArray[n][2];
    printf("%s| Running %d tests\n", testName.c_str(), n);

    for (int i = 0; i < n; i++) {
        auto start = high_resolution_clock::now();
        simulationToTest.setTrials(trialArray[i]);

        simulationToTest.simulate(leagueToTest);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);

        resultsArray[i][0] = trialArray[i];
        resultsArray[i][1] = duration.count();
    }

    string filename = testName;
    filename += ".txt";

    printTestResultsToFile(filename, resultsArray, n);
}

void SimulationTester::printTestResultsToFile(string filename, int resultsMatrix[][2], int numTests) {
    ofstream f(filename);
    printf("\nfilename: %s", filename.c_str());

    for (int i = 0; i < numTests; i++) {
        f << resultsMatrix[i][0] << " Trials = " << resultsMatrix[i][1] / 1000.0 << " seconds" << endl;
    }

    f.close();
}
