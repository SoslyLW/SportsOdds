#include "../include/SimulationDB.h"

SimulationDB::SimulationDB()
{
    //ctor
    srand(time(NULL));
    trials = 1000;

}

SimulationDB::SimulationDB(int numSims)
{
    //ctor
    srand(time(NULL));
    if (numSims != 0) {
        trials = numSims;
    } else {
        trials = 1000;
    }

}

void SimulationDB::simulateOne(League league) {
    ///Steps:
    // 1 - Simulate all the unplayed games
    // 2 - Calculate the updated standings
    // 3 - Determine who makes the playoffs
    // 4 - Save results

    float prediction;
    for (int i = 0; i < league.sched.size(); i++) {
        prediction = (float) rand() / RAND_MAX;

        if (!league.sched[i].getPlayed()) {
            league.sched[i].setPlayed(true);

            // 22% of games go into overtime so if prediction is within 0.11 then count the game as having gone to overtime
            if (prediction <= league.sched[i].getProb()) {
                // Home team won
                league.sched[i].setHomeScore(1);
                league.sched[i].setAwayScore(0);

                if (prediction > (league.sched[i].getProb() - 0.11)) {
                    league.sched[i].setOT(true);
                }

            } else {
                // Away team won
                league.sched[i].setHomeScore(0);
                league.sched[i].setAwayScore(1);

                if (prediction < (league.sched[i].getProb() + 0.11)) {
                    league.sched[i].setOT(true);
                }
            }
        }
    }

    //Run end of season calculations
    league.loadFromSchedule();
    league.pointsAndPercentCalcs();
    league.refreshTeams();
    league.standingsSort();

    //Determine who makes the playoffs
    int westCounter = 0, eastCounter = 0, atlCounter = 0, metCounter = 0, cenCounter = 0, pacCounter = 0;

    for (int i = 0; i < league.teams.size(); i++) {
        int conf = league.teams[i].getConference();
        int div = league.teams[i].getDivision();
        bool playoffsFull = false;

        //determine which counters to increment
        switch (conf) {
            case 0: eastCounter++;
                    break;
            case 1: westCounter++;
                    break;
        }

        if (westCounter > 8) {
            westCounter--;
            playoffsFull = true;
        }
        if (eastCounter > 8) {
            eastCounter--;
            playoffsFull = true;
        }

        switch (div) {
            case 0: atlCounter++;
                    break;
            case 1: metCounter++;
                    break;
            case 2: cenCounter++;
                    break;
            case 3: pacCounter++;
                    break;
        }

        if (atlCounter > 5) {
            atlCounter--;
            playoffsFull = true;
        }
        if (metCounter > 5) {
            metCounter--;
            playoffsFull = true;
        }
        if (cenCounter > 5) {
            cenCounter--;
            playoffsFull = true;
        }
        if (pacCounter > 5) {
            pacCounter--;
            playoffsFull = true;
        }


        //Find matching team using original index map
        int j = league.TSNtoOriginalIndex[league.teams[i].getTSN()];

        //Add a playoff apperance if playoffs have not yet been filled
        if (!playoffsFull) {
            teamResults[j].addPlayoffAppearance();
        }

        //Record final finishin position
        teamResults[j].incrementOneFinishingPosition(i);
    }
}

void SimulationDB::simulate(League league) {
    teamResults = league.teams;
    league.GrabInitialTeamConfiguration();

    cout << "Simulating " << trials << " trials...." << endl;

    for (int i = 0; i < trials; i++) {
        if ((i + 1) % 10000 == 0) {
            cout << "Trial " << i + 1 << " ";
        }

        simulateOne(league);
    }
}

void SimulationDB::printSimulationResults() {
    printingTeamResults = vector<Team>(teamResults);

    standingsSort(printingTeamResults);
    sortOdds(printingTeamResults);
    for (int i = 0; i < printingTeamResults.size(); i++) {
        long int sumFinishes = 0;

        //Calculate Average finishing position
        for (int j = 0; j < 32; j++) {
            sumFinishes += (printingTeamResults[i].getOneFinishingPosition(j) * (j + 1));
        }

        cout << i + 1 << ":" << endl;
        cout << printingTeamResults[i].getTSN() << " - " << printingTeamResults[i].getName() << endl
             << "W: " << printingTeamResults[i].getWins() << " L: " << printingTeamResults[i].getLosses() << " OTL: " << printingTeamResults[i].getOTL() << endl
             << "Pts: " << printingTeamResults[i].getPoints() << " GP: " << printingTeamResults[i].getGamesPlayed() << endl
             << "GF: " << printingTeamResults[i].getGoalsFor() << " GA: " << printingTeamResults[i].getGoalsAgainst() << " Pyt: " << printingTeamResults[i].getWinPct() << " Exp: " << printingTeamResults[i].getExponent() << endl
             << "Playoff Appearances: " << printingTeamResults[i].getPlayoffAppearances() << " Playoff Odds: " << float (printingTeamResults[i].getPlayoffAppearances()) / trials * 100.0 << endl
             << "Average Finish: " << (float) sumFinishes / trials << endl
             << endl;
    }
}

void SimulationDB::printSimulationResultsToFile(string filename) {
    ofstream f(filename);

    printingTeamResults = vector<Team>(teamResults);

    standingsSort(printingTeamResults);
    sortOdds(printingTeamResults);
    for (int i = 0; i < printingTeamResults.size(); i++) {
        long int sumFinishes = 0;

        //Calculate Average finishing position
        for (int j = 0; j < 32; j++) {
            sumFinishes += (printingTeamResults[i].getOneFinishingPosition(j) * (j + 1));
        }

        f << i + 1 << ":" << endl;
        f << printingTeamResults[i].getTSN() << " - " << printingTeamResults[i].getName() << endl
             << "W: " << printingTeamResults[i].getWins() << " L: " << printingTeamResults[i].getLosses() << " OTL: " << printingTeamResults[i].getOTL() << endl
             << "Pts: " << printingTeamResults[i].getPoints() << " GP: " << printingTeamResults[i].getGamesPlayed() << endl
             << "GF: " << printingTeamResults[i].getGoalsFor() << " GA: " << printingTeamResults[i].getGoalsAgainst() << " Pyt: " << printingTeamResults[i].getWinPct() << " Exp: " << printingTeamResults[i].getExponent() << endl
             << "Playoff Appearances: " << printingTeamResults[i].getPlayoffAppearances() << " Playoff Odds: " << float (printingTeamResults[i].getPlayoffAppearances()) / trials * 100.0 << endl
             << "Average Finish: " << (float) sumFinishes / trials << endl
             << endl;
    }

    f.close();
}

void SimulationDB::printSimulationResultsToJSON(string filename) {
    ofstream f(filename);

    printingTeamResults = vector<Team>(teamResults);

    f << "{\n\t\"teams\": [";

    standingsSort(printingTeamResults);
    sortOdds(printingTeamResults);
    for (int i = 0; i < printingTeamResults.size(); i++) {
        long int sumFinishes = 0;

        //Calculate Average finishing position
        for (int j = 0; j < 32; j++) {
            sumFinishes += (printingTeamResults[i].getOneFinishingPosition(j) * (j + 1));
        }

        f << "\n\t\t{" << endl;
        f << "\t\t\t\"name\": \"" << printingTeamResults[i].getName() << "\"," << endl;
        f << "\t\t\t\"gp\": " << printingTeamResults[i].getGamesPlayed() << "," << endl;
        f << "\t\t\t\"wins\": " << printingTeamResults[i].getWins() << "," << endl;
        f << "\t\t\t\"losses\": " << printingTeamResults[i].getLosses() << "," << endl;
        f << "\t\t\t\"otl\": " << printingTeamResults[i].getOTL() << "," << endl;
        f << "\t\t\t\"gf\": " << printingTeamResults[i].getGoalsFor() << "," << endl;
        f << "\t\t\t\"ga\": " << printingTeamResults[i].getGoalsAgainst() << "," << endl;
        f << "\t\t\t\"gd\": " << printingTeamResults[i].getGoalsFor() - printingTeamResults[i].getGoalsAgainst() << "," << endl;
        f << "\t\t\t\"playoff_odds\": " << float (printingTeamResults[i].getPlayoffAppearances()) / trials * 100.0 << endl;
        if (i == printingTeamResults.size() - 1) {
            f << "\t\t}";
        } else {
            f << "\t\t},";
        }
    }

    f << "\n\t]\n}";

    f.close();
}

void SimulationDB::standingsSort(vector<Team> &v) {
    sortPyth(v);
    sortDifferential(v);
    sortROW(v);
    sortGamesPlayed(v);
    sortPoints(v);
}

void SimulationDB::sortPoints(vector<Team> &v) {
    vector<Team> sorted(v.size());
    vector<int> midStep;


    for (int i = 0; i < v.size(); i++) {
        midStep.push_back(v[i].getPoints());
    }


    sort(midStep.begin(), midStep.end(), greater<int>());


    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < v.size(); j++) {
            if (midStep[i] == v[j].getPoints()) {
                sorted[i] = v[j];
                v.erase(v.begin() + j);
                break;
            }
        }
    }


    v = sorted;
}

void SimulationDB::sortGamesPlayed(vector<Team> &v) {
    vector<Team> sorted(v.size());
    vector<int> midStep;


    for (int i = 0; i < v.size(); i++) {
        midStep.push_back(v[i].getGamesPlayed());
    }


    sort(midStep.begin(), midStep.end());


    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < v.size(); j++) {
            if (midStep[i] == v[j].getGamesPlayed()) {
                sorted[i] = v[j];
                v.erase(v.begin() + j);
                break;
            }
        }
    }


    v = sorted;
}

void SimulationDB::sortROW(vector<Team> &v) {
    vector<Team> sorted(v.size());
    vector<int> midStep;


    for (int i = 0; i < v.size(); i++) {
        midStep.push_back(v[i].getWins() + v[i].getOTW());
    }


    sort(midStep.begin(), midStep.end(), greater<int>());


    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < v.size(); j++) {
            if (midStep[i] == v[j].getWins() + v[j].getOTW()) {
                sorted[i] = v[j];
                v.erase(v.begin() + j);
                break;
            }
        }
    }


    v = sorted;
}

void SimulationDB::sortDifferential(vector<Team> &v) {
    vector<Team> sorted(v.size());
    vector<int> midStep;


    for (int i = 0; i < v.size(); i++) {
        midStep.push_back(v[i].getGoalsFor() - v[i].getGoalsAgainst());
    }


    sort(midStep.begin(), midStep.end(), greater<int>());


    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < v.size(); j++) {
            if (midStep[i] == v[j].getGoalsFor() - v[j].getGoalsAgainst()) {
                sorted[i] = v[j];
                v.erase(v.begin() + j);
                break;
            }
        }
    }


    v = sorted;
}

void SimulationDB::sortPyth(vector<Team> &v) {
    vector<Team> sorted(v.size());
    vector<float> midStep;


    for (int i = 0; i < v.size(); i++) {
        midStep.push_back(v[i].getWinPct());
    }


    sort(midStep.begin(), midStep.end(), greater<float>());


    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < v.size(); j++) {
            if (midStep[i] == v[j].getWinPct()) {
                sorted[i] = v[j];
                v.erase(v.begin() + j);
                break;
            }
        }
    }


    v = sorted;
}

void SimulationDB::sortOdds(vector<Team> &v) {
    vector<Team> sorted(v.size());
    vector<float> midStep;


    for (int i = 0; i < v.size(); i++) {
        midStep.push_back(v[i].getPlayoffAppearances());
    }


    sort(midStep.begin(), midStep.end(), greater<int>());


    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < v.size(); j++) {
            if (midStep[i] == v[j].getPlayoffAppearances()) {
                sorted[i] = v[j];
                v.erase(v.begin() + j);
                break;
            }
        }
    }


    v = sorted;
}
