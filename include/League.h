#ifndef LEAGUE_H
#define LEAGUE_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <utility>
#include <algorithm>
#include <functional>
#include <cmath>
#include <random>
#include <cstdlib>
#include <ctime>

#include "Team.h"
#include "Game.h"

using namespace std;

class League
{
    public:
        League();

        int readTeams(string filename);
        int readSchedule(string filename);

        void resetStandings();
        void resetStandingsToLoadState();
        void GrabInitialTeamConfiguration();
        void loadFromSchedule(bool firstTime = false);
        void pointsAndPercentCalcs();
        float getWinPct(Team t);
        void refreshTeams();

        void predict();

        void printTeams();
        void printTeamsWithOdds(int numSims);
        void printPredictSchedule();

        void printTeamSchedule(string teamName);

        void outputStrengthOfScheduleToFile(string filename);

        void standingsSort();
        void sortPoints();
        void sortGamesPlayed();
        void sortROW();
        void sortDifferential();
        void sortPyth();

        map<string, int> TSNToIndex;
        map<string, int> TSNtoOriginalIndex;
        map<string, int> nameToIndex;

        //Variables not in private to aid with speed of simulation
        vector<Team> teams;
        vector<Team> originalTeamConfiguration;
        vector<Game> sched;
};

#endif // LEAGUE_H
