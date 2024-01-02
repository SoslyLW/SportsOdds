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
#include "TimeFunctions.h"

using namespace std;

class League
{
    public:
        League();
        // League(League &l);

        //Getters and Setters
        int getNumGames() { return numGames; };
        tm getStartDate() { return startDate; };
        tm getEndDate() { return endDate; };
        tm getCurrentDate() { return currentDate; };

        int readTeams(string filename);
        int readSchedule(string filename);
        int readScheduleToDate(string filename, string endDate);

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

        int numGames;
        tm startDate;
        tm endDate;
        tm currentDate;
};

#endif // LEAGUE_H
