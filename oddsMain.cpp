/***
    Issues: cold simulation, SOW and OTW not separated

    Thoughts: could turn the schedule and teams vectors into classes

***/

#include "Team.h"
#include "Game.h"
#include "League.h"

int main() {
    League wjc;

    //cout << 0;

    wjc.readTeams("howjc19teams.txt");
    wjc.readSchedule("howjc19schedule2.txt");

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

    wjc.printTeams();

    return 0;
}
