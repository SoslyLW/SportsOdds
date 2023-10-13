#ifndef SIMULATIONDB_H
#define SIMULATIONDB_H

#include "League.h"

class SimulationDB {
    public:
        SimulationDB();
        SimulationDB(int numSims);
        void simulateOne(League league);
        void simulate(League league);

        void standingsSort(vector<Team> &v);
        void sortPoints(vector<Team> &v);
        void sortGamesPlayed(vector<Team> &v);
        void sortROW(vector<Team> &v);
        void sortDifferential(vector<Team> &v);
        void sortPyth(vector<Team> &v);

        void sortOdds(vector<Team> &v);

        void printSimulationResults();

        void printSimulationResultsToFile(string filename);

        void setTrials(int numSims) { trials = numSims; };

    private:
        vector<League> seasons;
        vector<Team> teamResults;
        vector<Team> printingTeamResults;
        int trials;
};

#endif // SIMULATIONDB_H
