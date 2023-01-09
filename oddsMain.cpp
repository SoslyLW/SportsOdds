/***
    Issues: cold simulation, SOW and OTW not separated

***/

// Include classes
#include "Team.h"
#include "Game.h"
#include "League.h"
#include "SimulationDB.h"

int main() {
    // Create Variables
    League nhl;
    char verbose;

    // Load league data from csv files (Data provided by Sports Reference https://www.hockey-reference.com/leagues/NHL_2023_games.html)
    nhl.readTeams("nhlteams.csv");
    nhl.readSchedule("nhlschedule.csv");

    // Calculate odds based on the games already played in the schedule
    nhl.loadFromSchedule();
    nhl.pointsAndPercentCalcs();
    nhl.refreshTeams();

    // Create predictions for remaining games in schedule
    nhl.predict();


    // Ask user for input
    cout << "Verbose? [y/n]: ";
    cin >> verbose;


    // Print info to screen if necessary
    if (verbose == 'y' || verbose == 'Y') {
        nhl.printPredictSchedule();
        nhl.printTeams();
    }


    // Create object for class that runs the Monte Carlo simulation
    SimulationDB MonteCarlo;

    // Run Monte Carlo Simulation
    MonteCarlo.simulate(nhl);

    // Print Results
    MonteCarlo.printSimulationResults();
    MonteCarlo.printSimulationResultsToFile("results.txt");

    return 0;
}
