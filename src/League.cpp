#include "../include/League.h"

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

    int index = 0;
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

        tempTeam.setIndex(index);
        TSNToIndex[tempTeam.getTSN()] = index;
        TSNtoOriginalIndex[tempTeam.getTSN()] = index;
        nameToIndex[tempTeam.getName()] = index;
        index++;

        teams.push_back(tempTeam);
    }

    return 0;
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

        // Find teams associated with games using name map
        tempGame.setHomeTeam(teams[nameToIndex[homeTeam]]);
        tempGame.setAwayTeam(teams[nameToIndex[awayTeam]]);

        //Record stats of games if it has been played
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

    return 0;
}

//Resets all variables of all Team objects to 0
void League::resetStandings() {
    for (int i = 0; i < teams.size(); i++) {
        teams[i].setWins(0);
        teams[i].setLosses(0);
        teams[i].setOTL(0);
        teams[i].setOTW(0);
        teams[i].setTies(0);
        teams[i].setGamesPlayed(0);
        teams[i].setGoalsFor(0);
        teams[i].setGoalsAgainst(0);
        teams[i].setPoints(0);
    }
}

//Resets all variables of all Team objects to the inital configuration before simulation (Only applicable if runnign SimuationDB::simulate())
void League::resetStandingsToLoadState() {
    for (int i = 0; i < teams.size(); i++) {
        int originalIndex = TSNtoOriginalIndex[teams[i].getTSN()];

        teams[i].setWins(originalTeamConfiguration[originalIndex].getWins());
        teams[i].setLosses(originalTeamConfiguration[originalIndex].getLosses());
        teams[i].setOTL(originalTeamConfiguration[originalIndex].getOTL());
        teams[i].setOTW(originalTeamConfiguration[originalIndex].getOTW());
        teams[i].setTies(originalTeamConfiguration[originalIndex].getTies());
        teams[i].setGamesPlayed(originalTeamConfiguration[originalIndex].getGamesPlayed());
        teams[i].setGoalsFor(originalTeamConfiguration[originalIndex].getGoalsFor());
        teams[i].setGoalsAgainst(originalTeamConfiguration[originalIndex].getGoalsAgainst());
        teams[i].setPoints(0);

    }
    pointsAndPercentCalcs();
}

void League::loadFromSchedule(bool firstTime) {
    if (!firstTime) {
        resetStandings();
    }
    for (int i = 0; i < sched.size(); i++) {
        // If game has not been played, no need to record stats
        if (!sched[i].getPlayed()) {
            continue;
        }

        //Get index of home team using the map
        int j = TSNToIndex[sched[i].getHomeTeam().getTSN()];

        //Add stats to home team based on game result
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

        //Get index of away team using map
        j = TSNToIndex[sched[i].getAwayTeam().getTSN()];

        //Record away team stats
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
    }
}

//Gets the intial loaded standings for all Teams and records the intial positioning of the teams vector to create a map
void League::GrabInitialTeamConfiguration() {
    originalTeamConfiguration = vector<Team>(teams);

    for (int i = 0; i < teams.size(); i++) {
        TSNtoOriginalIndex[teams[i].getTSN()] = i;
    }
}

//Updates team stats for all games in schedule
void League::refreshTeams() {
    for (int i = 0; i < sched.size(); i++) {
        //Get home team index and set the game's home team to that Team
        int j = TSNToIndex[sched[i].getHomeTeam().getTSN()];
        sched[i].setHomeTeam(teams[j]);

        //Get away team index and set the game's away team to that Team
        j = TSNToIndex[sched[i].getAwayTeam().getTSN()];
        sched[i].setAwayTeam(teams[j]);
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

        winPct = (pow(t.getGoalsFor(), t.getExponent()) / (pow(t.getGoalsFor(), t.getExponent()) + pow(t.getGoalsAgainst(), t.getExponent())));
    }

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
             << endl;
    }
}

void League::printTeamsWithOdds(int numSims) {
    standingsSort();
    for (int i = 0; i < teams.size(); i++) {
        cout << i + 1 << ":" << endl;
        cout << teams[i].getTSN() << " - " << teams[i].getName() << endl
             << "W: " << teams[i].getWins() << " L: " << teams[i].getLosses() << " OTW: " << teams[i].getOTW() << " OTL: " << teams[i].getOTL() << endl
             << "Pts: " << teams[i].getPoints() << " GP: " << teams[i].getGamesPlayed() << endl
             << "GF: " << teams[i].getGoalsFor() << " GA: " << teams[i].getGoalsAgainst() << " Pyt: " << teams[i].getWinPct() << " Exp: " << teams[i].getExponent() << endl
             << "Playoff Appearances: " << teams[i].getPlayoffAppearances() << " Playoff Odds: " << float (teams[i].getPlayoffAppearances()) / numSims * 100.0 << endl
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
                sorted[i].setIndex(i);
                TSNToIndex[sorted[i].getTSN()] = i;
                nameToIndex[sorted[i].getName()] = i;
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
                sorted[i].setIndex(i);
                TSNToIndex[sorted[i].getTSN()] = i;
                nameToIndex[sorted[i].getName()] = i;
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
                sorted[i].setIndex(i);
                TSNToIndex[sorted[i].getTSN()] = i;
                nameToIndex[sorted[i].getName()] = i;
                teams.erase(teams.begin() + j);
                break;
            }
        }
    }

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
                sorted[i].setIndex(i);
                TSNToIndex[sorted[i].getTSN()] = i;
                nameToIndex[sorted[i].getName()] = i;
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
                sorted[i].setIndex(i);
                TSNToIndex[sorted[i].getTSN()] = i;
                nameToIndex[sorted[i].getName()] = i;
                teams.erase(teams.begin() + j);
                break;
            }
        }
    }

    teams = sorted;
}
