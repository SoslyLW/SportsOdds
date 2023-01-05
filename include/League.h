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

const int trials = 150000;

class League
{
    public:
        League();

        int readTeams(string filename);
        int readTeams(string filename, vector<Team> v);
        int readSchedule(string filename);
        int readSchedule(string filename, vector<Team> v);

        void loadFromSchedule();
        void loadFromSchedule(vector<Team> &v, vector<Game> &s);
        void pointsAndPercentCalcs();
        void pointsAndPercentCalcs(vector<Team> &v);
        float getWinPct(Team t);
        void refreshTeams();
        void refreshTeams(vector<Team> &v, vector<Game> &s);
        void calcLeagueAverages();

        void predict();
        void predict(vector<Game> v);

        void printTeams();
        void printTeams(vector<Team> &v);
        void printPredictSchedule();
        void printPredictSchedule(vector<Game> &v);

        void printTeamSchedule(string teamName);

        void outputStrengthOfScheduleToFile(string filename);

        void simulate();
        void simulateWithPlayoffs();

        void playoffCut();
        void playoffCut(vector<Team> &v);

        void standingsSort();
        void sortPoints();
        void sortGamesPlayed();
        void sortROW();
        void sortDifferential();
        void sortPyth();
        void standingsSort(vector<Team> &v);
        void sortPoints(vector<Team> &v);
        void sortGamesPlayed(vector<Team> &v);
        void sortROW(vector<Team> &v);
        void sortDifferential(vector<Team> &v);
        void sortPyth(vector<Team> &v);


    //private:
        vector<Team> teams;
        vector<Team> simmedTeams;
        vector<Game> sched;
        vector<Game> simmedSched;

        float avgGF;
};

#endif // LEAGUE_H
