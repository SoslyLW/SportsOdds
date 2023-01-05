#include "League.h"

League::League() {
    //ctor
}

int League::readTeams(string filename) {
    ifstream teamsFile(filename);
    string discard;

    if (!teamsFile.is_open()) {
        cout << "File not found";
        return 404;
    }

    // Read and do nothing with top row of csv
    getline(teamsFile, discard);

    while (!teamsFile.eof()) {
        Team tempTeam;
        string line, temp;

        getline(teamsFile, line);
        stringstream s(line);
        int i = 0;

        //make sure that an empty team is not submitted
        if (line.empty()) {
            break;
        }

        while (getline(s, temp, ',')) {
            switch (i) {
                case 0: tempTeam.setName(temp);
                        break;
                case 1: tempTeam.setTSN(temp);
                        break;
                case 2: tempTeam.setConference(stoi(temp));
                        break;
                case 3: tempTeam.setDivision(stoi(temp));
                        break;
            }
            i++;
        }

        teams.push_back(tempTeam);
    }
}

int League::readSchedule(string filename) {
    ifstream schedFile(filename);
    string discard;

    if (!schedFile.is_open()) {
        cout << "File not found";
        return 404;
    }

    // Read and do nothing with top row of csv
    getline(schedFile, discard);

    while (!schedFile.eof()) {
        Game tempGame;
        string line, date, homeTeam, awayTeam, homeGoals, awayGoals, OT, attendance, LOG, notes;

        getline(schedFile, line);
        stringstream s(line);

        //make sure that an empty team is not submitted
        if (line.empty()) {
            break;
        }

        getline(s, date, ',');
        getline(s, awayTeam, ',');
        getline(s, awayGoals, ',');
        getline(s, homeTeam, ',');
        getline(s, homeGoals, ',');
        getline(s, OT, ',');
        getline(s, attendance, ',');
        getline(s, LOG, ',');
        getline(s, notes, ',');

        // Find teams associated with games
        for (int i = 0; i < teams.size(); i++) {
            if (teams[i].getName() == homeTeam) {
                tempGame.setHomeTeam(teams[i]);
                break;
            }
        }
        for (int i = 0; i < teams.size(); i++) {
            if (teams[i].getName() == awayTeam) {
                tempGame.setAwayTeam(teams[i]);
                break;
            }
        }

        if (!homeGoals.empty()) {
            tempGame.setPlayed(true);

            tempGame.setHomeScore(stoi(homeGoals));
            tempGame.setAwayScore(stoi(awayGoals));

            if (OT == "OT" || OT == "SO") {
                tempGame.setOT(true);
            }

            if (OT == "SO") {
                tempGame.setSO(true);
            }
        }


        sched.push_back(tempGame);
    }
}

void League::loadFromSchedule() {
    for (int i = 0; i < sched.size(); i++) {
        // If game has not been played, no need to record stats
        if (!sched[i].getPlayed()) {
            continue;
        }

        //Find home team using name (not using pointers to array) and the calculate wins/losses
        for (int j = 0; j < teams.size(); j++) {
            if (sched[i].getHomeTeam().getTSN() == teams[j].getTSN()) {
                sched[i].setHomeTeam(teams[j]);

                if (sched[i].getHomeScore() > sched[i].getAwayScore()) {
                    /*if (sched[i].getOT()) {
                        teams[j].setOTW(teams[j].getOTW() + 1);
                    } else {
                        teams[j].setWins(teams[j].getWins() + 1);
                    }*/

                    teams[j].setWins(teams[j].getWins() + 1);
                } else {
                    if (sched[i].getOT()) {
                        teams[j].setOTL(teams[j].getOTL() + 1);
                    } else {
                        teams[j].setLosses(teams[j].getLosses() + 1);
                    }
                }
                teams[j].setGamesPlayed(teams[j].getGamesPlayed() + 1);
                teams[j].setGoalsFor(teams[j].getGoalsFor() + sched[i].getHomeScore());
                teams[j].setGoalsAgainst(teams[j].getGoalsAgainst() + sched[i].getAwayScore());
                break;
            }
        }

        for (int j = 0; j < teams.size(); j++) {
            if (sched[i].getAwayTeam().getTSN() == teams[j].getTSN()) {
                sched[i].setAwayTeam(teams[j]);

                if (sched[i].getAwayScore() > sched[i].getHomeScore()) {
                    /*if (sched[i].getOT()) {
                        teams[j].setOTW(teams[j].getOTW() + 1);
                    } else {
                        teams[j].setWins(teams[j].getWins() + 1);
                    }*/

                    teams[j].setWins(teams[j].getWins() + 1);
                } else {
                    if (sched[i].getOT()) {
                        teams[j].setOTL(teams[j].getOTL() + 1);
                    } else {
                        teams[j].setLosses(teams[j].getLosses() + 1);
                    }
                }
                teams[j].setGamesPlayed(teams[j].getGamesPlayed() + 1);
                teams[j].setGoalsFor(teams[j].getGoalsFor() + sched[i].getAwayScore());
                teams[j].setGoalsAgainst(teams[j].getGoalsAgainst() + sched[i].getHomeScore());
                break;
            }
        }
    }
}

//Updates team stats for all games in schedule
void League::refreshTeams() {
    for (int i = 0; i < sched.size(); i++) {
        for (int j = 0; j < teams.size(); j++) {
            if (sched[i].getHomeTeam().getTSN() == teams[j].getTSN()) {
                sched[i].setHomeTeam(teams[j]);
                break;
            }
        }
        for (int j = 0; j < teams.size(); j++) {
            if (sched[i].getAwayTeam().getTSN() == teams[j].getTSN()) {
                sched[i].setAwayTeam(teams[j]);
                break;
            }
        }
    }
}

//Uses NHL points system of 2 for a win, 1 for OTl, 0 for regulation loss
void League::pointsAndPercentCalcs() {
    for (int i = 0; i < teams.size(); i++) {
        teams[i].setPoints((teams[i].getWins() * 2) + (teams[i].getOTL()));
        teams[i].calcExponent();
        teams[i].setWinPct(getWinPct(teams[i]));
    }
}

void League::outputStrengthOfScheduleToFile(string filename) {
    ofstream f(filename);

    for (int i = 0; i < teams.size(); i++) {
        float sos = 0;
        for (int j = 0; j < sched.size(); j++) {
            if (sched[j].getHomeTeam().getName() == teams[i].getName()) {
                sos += sched[j].getProb();
            } else if (sched[j].getAwayTeam().getName() == teams[i].getName()) {
                sos += (1.0 - sched[j].getProb());
            }
        }

        f << teams[i].getTSN() << ": " << sos << endl;
    }

    f.close();
}

float League::getWinPct(Team t) {
    float winPct = 0.5;
    if (t.getGamesPlayed() > 0) {
        //Give teams with no goals scored 1 goal so as to not give 0% chance of winning
        if (t.getGoalsFor() == 0) {
            t.setGoalsFor(1);
        }

        //cout << t.getExponent() << " - " << t.getGoalsFor() << ", " << t.getGoalsAgainst() << " = ";
        winPct = (pow(t.getGoalsFor(), t.getExponent()) / (pow(t.getGoalsFor(), t.getExponent()) + pow(t.getGoalsAgainst(), t.getExponent())));
    }

    //cout << winPct << endl;
    return winPct;
}

void League::predict() {
    for (int i = 0; i < sched.size(); i++) {
        if (!sched[i].getPlayed()) {
            sched[i].log5();
        } else if (sched[i].getHomeScore() > sched[i].getAwayScore()) {
            sched[i].setProb(1.0);
        } else {
            sched[i].setProb(0.0);
        }
    }
}

void League::printTeams() {
    standingsSort();
    for (int i = 0; i < teams.size(); i++) {
        cout << i + 1 << ":" << endl;
        cout << teams[i].getTSN() << " - " << teams[i].getName() << endl
             << "W: " << teams[i].getWins() << " L: " << teams[i].getLosses() << " OTW: " << teams[i].getOTW() << " OTL: " << teams[i].getOTL() << endl
             << "Pts: " << teams[i].getPoints() << " GP: " << teams[i].getGamesPlayed() << endl
             << "GF: " << teams[i].getGoalsFor() << " GA: " << teams[i].getGoalsAgainst() << " Pyt: " << teams[i].getWinPct() << " Exp: " << teams[i].getExponent() << endl
             << "Playoff Appearances: " << teams[i].getPlayoffAppearances() << " Playoff Odds: " << float (teams[i].getPlayoffAppearances()) / trials * 100.0 << endl
             << endl;
    }
}

void League::printPredictSchedule() {
    for (int i = 0; i < sched.size(); i++) {
        if (sched[i].getPlayed()) {
            cout << sched[i].getHomeTeam().getTSN() << " " << sched[i].getHomeScore() << " - " << sched[i].getAwayScore() << " " << sched[i].getAwayTeam().getTSN() << endl;
        } else {
            cout << sched[i].getHomeTeam().getTSN() << " " << sched[i].getProb() << " " << sched[i].getAwayTeam().getTSN() << endl;
        }
    }
}

void League::printTeamSchedule(string teamName) {
    for (int i = 0; i < sched.size(); i++) {
        if (sched[i].getHomeTeam().getName() == teamName || sched[i].getAwayTeam().getName() == teamName) {
            if (sched[i].getPlayed()) {
                cout << sched[i].getHomeTeam().getTSN() << " " << sched[i].getHomeScore() << " - " << sched[i].getAwayScore() << " " << sched[i].getAwayTeam().getTSN() << " " << sched[i].getProb() << endl;
            } else {
                cout << sched[i].getHomeTeam().getTSN() << " " << sched[i].getProb() << " " << sched[i].getAwayTeam().getTSN() << endl;
            }
        }
    }
}


void League::standingsSort() {
    sortPyth();
    sortDifferential();
    sortROW();
    sortGamesPlayed();
    sortPoints();
}

void League::sortPoints() {
    vector<Team> sorted(teams.size());
    vector<int> midStep;


    for (int i = 0; i < teams.size(); i++) {
        midStep.push_back(teams[i].getPoints());
    }


    sort(midStep.begin(), midStep.end(), greater<int>());


    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < teams.size(); j++) {
            if (midStep[i] == teams[j].getPoints()) {
                sorted[i] = teams[j];
                teams.erase(teams.begin() + j);
                break;
            }
        }
    }


    teams = sorted;
}

void League::sortGamesPlayed() {
    vector<Team> sorted(teams.size());
    vector<int> midStep;


    for (int i = 0; i < teams.size(); i++) {
        midStep.push_back(teams[i].getGamesPlayed());
    }


    sort(midStep.begin(), midStep.end());


    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < teams.size(); j++) {
            if (midStep[i] == teams[j].getGamesPlayed()) {
                sorted[i] = teams[j];
                teams.erase(teams.begin() + j);
                break;
            }
        }
    }


    teams = sorted;
}

//not completely working (treats shootout wins as overtime wins)
void League::sortROW() {
    vector<Team> sorted(teams.size());
    vector<int> midStep;


    for (int i = 0; i < teams.size(); i++) {
        midStep.push_back(teams[i].getWins() + teams[i].getOTW());
    }


    sort(midStep.begin(), midStep.end(), greater<int>());


    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < teams.size(); j++) {
            if (midStep[i] == teams[j].getWins() + teams[j].getOTW()) {
                sorted[i] = teams[j];
                teams.erase(teams.begin() + j);
                break;
            }
        }
    }

    //move it all to the vector
    teams = sorted;
}

void League::sortDifferential() {
    vector<Team> sorted(teams.size());
    vector<int> midStep;


    for (int i = 0; i < teams.size(); i++) {
        midStep.push_back(teams[i].getGoalsFor() - teams[i].getGoalsAgainst());
    }


    sort(midStep.begin(), midStep.end(), greater<int>());


    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < teams.size(); j++) {
            if (midStep[i] == teams[j].getGoalsFor() - teams[j].getGoalsAgainst()) {
                sorted[i] = teams[j];
                teams.erase(teams.begin() + j);
                break;
            }
        }
    }


    teams = sorted;
}

void League::sortPyth() {
    vector<Team> sorted(teams.size());
    vector<float> midStep;


    for (int i = 0; i < teams.size(); i++) {
        midStep.push_back(teams[i].getWinPct());
    }


    sort(midStep.begin(), midStep.end(), greater<float>());


    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < teams.size(); j++) {
            if (midStep[i] == teams[j].getWinPct()) {
                sorted[i] = teams[j];
                teams.erase(teams.begin() + j);
                break;
            }
        }
    }


    teams = sorted;
}
