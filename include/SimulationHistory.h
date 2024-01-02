#ifndef SIMULATIONHISTORY_H
#define SIMULATIONHISTORY_H

#include "SimulationDB.h"

using namespace std;

struct TeamAtDate
{
    string date;
    vector<Team> teams;
    int mostTrials; //Keep track of the deepest simulation so it won't be overwritten
};

const int defaultTrials = 100;

class SimulationHistory {
    public:
        SimulationHistory(League _l, SimulationDB _db);
        SimulationHistory(League _l);

        void setTrials(int numSims) { trials = numSims; };

        void SimulateDayByDay();
        int SimulateDay(string endDate);

        void printToJSON(string filename);

    private:
        League baseLeague;
        SimulationDB db;
        int trials;
        int lastDayTrials;
        vector<TeamAtDate> history;
};

#endif // SIMULATIONHISTORY_H
