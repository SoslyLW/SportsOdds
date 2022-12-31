#ifndef SIMULATIONDB_H
#define SIMULATIONDB_H

#include "League.h"

//const int numTrials = 10;

//auto randomNumber = bind(dis, generator);

class SimulationDB: League
{
    public:
        SimulationDB();
        void simulateOne(League league);
        void simulate(League league);

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

        void sortOdds(vector<Team> &v);

        void printSimulationResults();

        //float randomNumber();
        float prediction;
        float predictionChange;

    private:
        vector<League> seasons;
        vector<Team> teamResults;
};

#endif // SIMULATIONDB_H
