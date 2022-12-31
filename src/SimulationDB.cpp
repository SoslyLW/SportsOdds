#include "SimulationDB.h"

SimulationDB::SimulationDB()
{
    //ctor
    srand(time(NULL));
    random_device rd;
    mt19937 generator(time(0));
    uniform_real_distribution<> dis(0.0, 1.0);
    auto randomNumber = bind(dis, generator);

    prediction = randomNumber();
    predictionChange = randomNumber();

}


void SimulationDB::simulateOne(League league) {
    /*random_device rd;
    mt19937 generator(time(0));
    uniform_real_distribution<> dis(0.0, 1.0);
    auto randomNumber = bind(dis, generator);*/

    ///Steps:
    // 1 - Simulate all the unplayed games
    // 2 - Calculate the updated standings
    // 3 - Determine who makes the playoffs
    // 4 - Save results

    //float prediction = randomNumber();
    //float predictionChange = randomNumber();
    //float prediction = randomNumber();
    //predictionChange = randomNumber();

    for (int i = 0; i < league.sched.size(); i++) {
            prediction += predictionChange;
            if (prediction > 1) {
                prediction--;
            }

        //float prediction = rand() / RAND_MAX;
        //cout << prediction << " ";

        if (!league.sched[i].getPlayed()) {
            //float prediction = randomNumber();
            league.sched[i].setPlayed(true);

            //Need to capture probability of going into overtime
            //Say 20% of games go into OT, then within 10% on either side counts as an OT win
            if (league.sched[i].getProb() <= prediction) {
                // Home team won
                league.sched[i].setHomeScore(1);
                league.sched[i].setAwayScore(0);
            } else {
                // Away team won
                league.sched[i].setHomeScore(0);
                league.sched[i].setAwayScore(1);
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

        //determine which counters to increment
        switch (conf) {
            case 0: eastCounter++;
                    break;
            case 1: westCounter++;
                    break;
        }

        if (westCounter > 8) {
            westCounter--;
            break;
        }
        if (eastCounter > 8) {
            eastCounter--;
            break;
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
            break;
        }
        if (metCounter > 5) {
            metCounter--;
            break;
        }
        if (cenCounter > 5) {
            cenCounter--;
            break;
        }
        if (pacCounter > 5) {
            pacCounter--;
            break;
        }


        //Find matching team
        for (int j = 0; j < teamResults.size(); j++) {
            if (teamResults[j].getName() == league.teams[i].getName()) {
                teamResults[j].addPlayoffAppearance();
                //cout << "added ";
                break;
            }
        }
    }





    //league.printTeams();

    //cout << prediction;
    /*for (int i = 0; i < 32; i++) {
        cout << league.teams[i].getTSN() << " ";
    }
    //cout << league.teams[0].getTSN() << endl;
    cout << endl << endl;*/

    /*if (league.teams[0].getTSN() == "OTT") {
        cout << "OTT" << endl;
    }*/
}

void SimulationDB::simulate(League league) {
    teamResults = league.teams;

    for (int i = 0; i < trials; i++) {
        if ((i + 1) % 10000 == 0) {
            cout << "Trial " << i + 1 << " ";
        }

        simulateOne(league);
    }
}

void SimulationDB::printSimulationResults() {
    //sortOdds(teamResults);
    sortPyth(teamResults);
    for (int i = 0; i < teamResults.size(); i++) {
        cout << i + 1 << ":" << endl;
        cout << teamResults[i].getTSN() << " - " << teamResults[i].getName() << endl
             << "W: " << teamResults[i].getWins() << " L: " << teamResults[i].getLosses() << " OTL: " << teamResults[i].getOTL() << endl
             << "Pts: " << teamResults[i].getPoints() << " GP: " << teamResults[i].getGamesPlayed() << endl
             << "GF: " << teamResults[i].getGoalsFor() << " GA: " << teamResults[i].getGoalsAgainst() << " Pyt: " << teamResults[i].getWinPct() << " Exp: " << teamResults[i].getExponent() << endl
             << "Playoff Appearances: " << teamResults[i].getPlayoffAppearances() << " Playoff Odds: " << float (teamResults[i].getPlayoffAppearances()) / trials * 100.0 << endl
             << endl;
    }
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

    //copy all the titles to a separate vector
    for (int i = 0; i < v.size(); i++) {
        midStep.push_back(v[i].getPoints());
    }

    //sort that vector
    sort(midStep.begin(), midStep.end(), greater<int>());

    //look through a copy of the database to assign the rest of the values in the movies to their given titles
    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < v.size(); j++) {
            if (midStep[i] == v[j].getPoints()) {
                sorted[i] = v[j];
                v.erase(v.begin() + j);
                break;
            }
        }
    }

    //move it all to the movies vector
    v = sorted;
}

void SimulationDB::sortGamesPlayed(vector<Team> &v) {
    vector<Team> sorted(v.size());
    vector<int> midStep;

    //copy all the titles to a separate vector
    for (int i = 0; i < v.size(); i++) {
        midStep.push_back(v[i].getGamesPlayed());
    }

    //sort that vector
    sort(midStep.begin(), midStep.end());

    //look through a copy of the database to assign the rest of the values in the movies to their given titles
    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < v.size(); j++) {
            if (midStep[i] == v[j].getGamesPlayed()) {
                sorted[i] = v[j];
                v.erase(v.begin() + j);
                break;
            }
        }
    }

    //move it all to the movies vector
    v = sorted;
}

void SimulationDB::sortROW(vector<Team> &v) {
    vector<Team> sorted(v.size());
    vector<int> midStep;

    //copy all the titles to a separate vector
    for (int i = 0; i < v.size(); i++) {
        midStep.push_back(v[i].getWins() + v[i].getOTW());
    }

    //sort that vector
    sort(midStep.begin(), midStep.end(), greater<int>());

    //look through a copy of the database to assign the rest of the values in the movies to their given titles
    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < v.size(); j++) {
            if (midStep[i] == v[j].getWins() + v[j].getOTW()) {
                sorted[i] = v[j];
                v.erase(v.begin() + j);
                break;
            }
        }
    }

    //move it all to the vector
    v = sorted;
}

void SimulationDB::sortDifferential(vector<Team> &v) {
    vector<Team> sorted(v.size());
    vector<int> midStep;

    //copy all the titles to a separate vector
    for (int i = 0; i < v.size(); i++) {
        midStep.push_back(v[i].getGoalsFor() - v[i].getGoalsAgainst());
    }

    //sort that vector
    sort(midStep.begin(), midStep.end(), greater<int>());

    //look through a copy of the database to assign the rest of the values in the movies to their given titles
    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < v.size(); j++) {
            if (midStep[i] == v[j].getGoalsFor() - v[j].getGoalsAgainst()) {
                sorted[i] = v[j];
                v.erase(v.begin() + j);
                break;
            }
        }
    }

    //move it all to the movies vector
    v = sorted;
}

void SimulationDB::sortPyth(vector<Team> &v) {
    vector<Team> sorted(v.size());
    vector<float> midStep;

    //copy all the titles to a separate vector
    for (int i = 0; i < v.size(); i++) {
        midStep.push_back(v[i].getWinPct());
    }

    //sort that vector
    sort(midStep.begin(), midStep.end(), greater<float>());

    //look through a copy of the database to assign the rest of the values in the movies to their given titles
    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < v.size(); j++) {
            if (midStep[i] == v[j].getWinPct()) {
                sorted[i] = v[j];
                v.erase(v.begin() + j);
                break;
            }
        }
    }

    //move it all to the movies vector
    v = sorted;
}

void SimulationDB::sortOdds(vector<Team> &v) {
    vector<Team> sorted(v.size());
    vector<float> midStep;

    //copy all the titles to a separate vector
    for (int i = 0; i < v.size(); i++) {
        midStep.push_back(v[i].getPlayoffAppearances());
    }

    //sort that vector
    sort(midStep.begin(), midStep.end(), greater<int>());

    //look through a copy of the database to assign the rest of the values in the movies to their given titles
    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < v.size(); j++) {
            if (midStep[i] == v[j].getPlayoffAppearances()) {
                sorted[i] = v[j];
                v.erase(v.begin() + j);
                break;
            }
        }
    }

    //move it all to the movies vector
    v = sorted;
}