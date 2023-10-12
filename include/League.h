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
#include <time.h>

#include "Team.h"
#include "Game.h"

using namespace std;

class League
{
    public:
        League();

        int readTeams(string filename);
        int readSchedule(string filename);

        void loadFromSchedule();
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

        //Variables not in private to aid with speed of simulation
        vector<Team> teams;
        vector<Game> sched;
};

#endif // LEAGUE_H
