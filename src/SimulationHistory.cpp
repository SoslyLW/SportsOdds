#include "../include/SimulationHistory.h"

SimulationHistory::SimulationHistory(League _l, SimulationDB _db) {
    baseLeague = _l;
    db = _db;
    trials = defaultTrials;
    lastDayTrials = defaultTrials * 100;
}

SimulationHistory::SimulationHistory(League _l) {
    baseLeague = _l;
    trials = defaultTrials;
    lastDayTrials = defaultTrials * 100;
}

void SimulationHistory::SimulateDayByDay() {
    tm startDate = baseLeague.getStartDate();
    tm endDate = baseLeague.getEndDate();
    time_t end=mktime(&(baseLeague.currentDate));

    //Loop to go through every day in the season
    for(tm date = startDate; end>=mktime(&date); ++date.tm_mday) {
        cout << "Simulating " << TimeFunctions::tmToString(date) << endl;
        if (SimulateDay(TimeFunctions::tmToString(date)) == 404) {
            cout << "Error reading file";
            return;
        }

        cout << endl;
    }

    printToJSON("testOutput.json");
}

int SimulationHistory::SimulateDay(string endDate) {
    //Make a copy of the league
    // League league(baseLeague);
    League league = baseLeague;

    //Read schedule up to date
    if (league.readScheduleToDate("nhlschedule2024.csv", endDate) == 404) {
        return 404;
    }

    //Calculate weights up to that date
    league.loadFromSchedule(false);
    league.pointsAndPercentCalcs();
    league.refreshTeams();

    //Run simulations
    league.predict();

    SimulationDB MonteCarlo(league, trials);

    if (endDate == TimeFunctions::tmToString(league.getCurrentDate())) {
        MonteCarlo.setTrials(lastDayTrials);
    }

    // Run Monte Carlo Simulation
    MonteCarlo.simulate(league);

    // Sort teams for consistency
    MonteCarlo.sortName(MonteCarlo.getTeamResults());

    string fn = "tmp/testing" + endDate;
    MonteCarlo.printSimulationResultsToFile(fn);

    // Print Results
    TeamAtDate tad;
    tad.date = endDate;
    tad.teams = vector<Team>(MonteCarlo.getTeamResults());

    history.push_back(tad);

    return 0;
}

void SimulationHistory::printToJSON(string filename) {
    ofstream f(filename);
    tm startDate = baseLeague.getStartDate();
    tm endDate = baseLeague.getEndDate();
    string currentDate = TimeFunctions::tmToString(baseLeague.currentDate);
    time_t end=mktime(&(baseLeague.currentDate));
    int numTeams = history[0].teams.size();

    cout << history.size() << endl;

    cout <<"Looping through history struct" << endl;

    for (int k = 0; k < history.size(); k++) {
        cout << k << ": " << history[k].date << endl;
    }
    // return;
    cout <<"Done looping through history struct" << endl;

    

    f << "{\n\t\"graph\": [";
    // cout << "{\n\t\"graph\": [";

    //Set up header of data
    f << "[\"Date\", ";
    // cout << "[\"Date\", ";

    for (int i = 0; i < numTeams; i++) {
        // cout << endl << history[0].teams.size();
        f << "\"" << history[0].teams[i].getName() << "\"";
        // cout << "\"" << history[0].teams[i].getName() << "\"";
        if (i != numTeams - 1) {
            f << ", ";
            // cout << ", ";
        }
    }

    f << "],\n";
    // cout << "],\n";


    //Loop to go through every day in the season
    // int i = 0;
    // for(tm date = startDate; end>=mktime(&date); ++date.tm_mday) {
    for (int i = 0; i < history.size(); i++) {
        // string currentDay = history[i].date;
        cout << i << ": " << history[0].date << endl;
        

        // cout << endl << currentDay << "=" << history[i].date << ": " << history[i].teams[0].getTSN();
        f << "\t\t\t\t\t\t[";
        // cout << "[";

        for (int j = 0; j < numTeams; j++) {
            if (i != history.size() - 1) {
                f << (history[i].teams[j].getPlayoffAppearances() / (float)defaultTrials * 100.0);
            } else {
                f << (history[i].teams[j].getPlayoffAppearances() / (float)lastDayTrials * 100.0);
            }
            // cout << (int) (history[i].teams[j].getPlayoffAppearances() / (float) defaultTrials * 100.0);
            if (j != numTeams - 1) {
                f << ", ";
                // cout <<", ";
            }
        }

        f << "]";
        // cout << "]";
        if (history[i].date != currentDate) {
            f << ",\n";
            // cout << ",\n";
        }
        
        // i++;
    }

    f << "\n\t]\n}";
    //cout << "\n\t]\n}";

    f.close();
}