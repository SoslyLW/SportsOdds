/***
    Issues: cold simulation, SOW and OTW not separated

    Thoughts: could turn the schedule and teams vectors into classes

***/

#include "Team.h"
#include "Game.h"
#include "League.h"
#include "SimulationDB.h"

int main() {
    //League nhl;
    League wjc;
    //vector<Team> teamsNHL(32);
    //vector<Game> scheduleNHL;

    League nhl;

    //nhl.readTeams("howjc19teams.txt");
    nhl.readTeams("nhlteams.csv");

    nhl.readSchedule("nhlschedule.csv");
   // nhl.printPredictSchedule();

    nhl.loadFromSchedule();
    nhl.pointsAndPercentCalcs();
    nhl.refreshTeams();
    nhl.predict();

    nhl.printPredictSchedule();

    //nhl.simulateWithPlayoffs();
    //nhl.simulate();

    nhl.printTeams();

    SimulationDB MonteCarlo;

    //MonteCarlo.simulateOne(nhl);
    MonteCarlo.simulate(nhl);

    MonteCarlo.printSimulationResults();


    //nhl.printTeams();



    //cout << 0;
/*
    wjc.readTeams("howjc19teams.txt");
    wjc.readSchedule("howjc19schedule.txt");

    //cout << 1;

    wjc.loadFromSchedule();
    wjc.pointsAndPercentCalcs();
    wjc.refreshTeams();
    wjc.predict();

    //wjc.playoffCut();
    wjc.printTeams();
    wjc.printPredictSchedule();

    //wjc.simulate();
    wjc.simulateWithPlayoffs();

    cout << endl << endl;

    wjc.printTeams();*/


    return 0;
}
