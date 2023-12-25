/***
    Issues: cold simulation, SOW and OTW not separated

***/

// Include classes
#include "include/Team.h"
#include "include/Game.h"
#include "include/League.h"
#include "include/SimulationDB.h"
#include "include/SimulationTester.h"

int main() {
    // Create Variables
    League nhl;
    char verbose;
    int numSimulations = 0;

    // Load league data from csv files (Data provided by Sports Reference https://www.hockey-reference.com/leagues/NHL_2023_games.html)
    nhl.readTeams("nhlteams.csv");
    nhl.readSchedule("nhlschedule.csv");

    // Calculate odds based on the games already played in the schedule
    nhl.loadFromSchedule(true);
    nhl.pointsAndPercentCalcs();
    nhl.refreshTeams();

    // Create predictions for remaining games in schedule
    nhl.predict();

    // Ask user for input
    cout << "Verbose? [y/n]: ";
    cin >> verbose;

    if (verbose != 'T') {
        cout << "Number of trials? ";
        cin >> numSimulations;

        if (numSimulations == 0) {
            //Set a default number of simulations
            numSimulations = 1000;
        }
    }

    // Create object for class that runs the Monte Carlo simulation
    SimulationDB MonteCarlo(numSimulations);

    // Print info to screen if necessary
    if (verbose == 'y' || verbose == 'Y') {
        nhl.printPredictSchedule();
        nhl.printTeams();
    } else if (verbose == 'T') {
        //Run speed tests in future
        SimulationTester Tester(MonteCarlo, nhl);
        Tester.runTestSuite();

        return 0;
    }

    // Run Monte Carlo Simulation
    MonteCarlo.simulate(nhl);

    // Print Results
    MonteCarlo.printSimulationResults();
    MonteCarlo.printSimulationResultsToFile("results.txt");

    return 0;
}
