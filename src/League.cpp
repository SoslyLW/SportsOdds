#include "League.h"

League::League() {
    //ctor
    avgGF = 0;
}

int League::readTeams(string filename) {
    ifstream teamsFile(filename);
    string temp;

    if (!teamsFile.is_open()) {
        cout << "File not found";
        return 404;
    }

    while (!teamsFile.eof()) {
        Team tempTeam;

        getline(teamsFile, temp);
        tempTeam.setName(temp);
        getline(teamsFile, temp);
        tempTeam.setTSN(temp);

        teams.push_back(tempTeam);
    }

    cout << "run success" << endl;
}

int League::readSchedule(string filename) {
    ifstream schedFile(filename);
    string temp1, temp2;
    int temp3, temp4, temp5;

    if (!schedFile.is_open()) {
        cout << "File not found";
        return 404;
    }

    while (schedFile >> temp1) {
        Game tempGame;

//        getline(schedFile, temp1, ' ');
//        getline(schedFile, temp2, ' ');

        //schedFile >> temp1;
        schedFile >> temp2;

        char ab;
        //cout << temp1 << "-" << temp2;
        //cin >> ab;

        for (int i = 0; i < teams.size(); i++) {
            if (teams[i].getTSN() == temp1) {
                tempGame.setHomeTeam(teams[i]);
                break;
            }
        }
        for (int i = 0; i < teams.size(); i++) {
            if (teams[i].getTSN() == temp2) {
                tempGame.setAwayTeam(teams[i]);
                break;
            }
        }

        //cout << 2;

        schedFile >> temp3;

        if (temp3 == -4) {
            schedFile >> temp4;
            schedFile >> temp5;
            //schedFile.ignore();

            tempGame.setHomeScore(temp4);
            tempGame.setAwayScore(temp5);
            tempGame.setOT(true);
            tempGame.setPlayed(true);
        } else if (temp3 != -1) {
            schedFile >> temp4;
            //schedFile.ignore();

            tempGame.setHomeScore(temp3);
            tempGame.setAwayScore(temp4);
            tempGame.setOT(false);
            tempGame.setPlayed(true);
        } else {
            tempGame.setHomeScore(0);
            tempGame.setAwayScore(0);
            tempGame.setPlayed(false);
            //schedFile.ignore();
        }

        sched.push_back(tempGame);

    }

    cout << "run success" << endl;
}

void League::loadFromSchedule() {
    for (int i = 0; i < sched.size(); i++) {
        if (!sched[i].getPlayed()) {
            continue;
        }
        for (int j = 0; j < teams.size(); j++) {

            if (sched[i].getHomeTeam().getTSN() == teams[j].getTSN()) {
                sched[i].setHomeTeam(teams[j]);

                if (sched[i].getHomeScore() > sched[i].getAwayScore()) {
                    if (sched[i].getOT()) {
                        teams[j].setOTW(teams[j].getOTW() + 1);
                    } else {
                        teams[j].setWins(teams[j].getWins() + 1);
                    }
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
                    if (sched[i].getOT()) {
                        teams[j].setOTW(teams[j].getOTW() + 1);
                    } else {
                        teams[j].setWins(teams[j].getWins() + 1);
                    }
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

void League::pointsAndPercentCalcs() {
    for (int i = 0; i < teams.size(); i++) {
        teams[i].setPoints((teams[i].getWins() * 3) + (teams[i].getOTW() * 2) + (teams[i].getOTL()));
        teams[i].calcExponent();
        teams[i].setWinPct(getWinPct(teams[i]));
    }
}

float League::getWinPct(Team t) {
    float winPct = 0.5;
    if (t.getGamesPlayed() > 0) {
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
        cout << teams[i].getTSN() << " - " << teams[i].getName() << endl
             << "W: " << teams[i].getWins() << " L: " << teams[i].getLosses() << " OTW: " << teams[i].getOTW() << " OTL: " << teams[i].getOTL() << endl
             << "Pts: " << teams[i].getPoints() << " GP: " << teams[i].getGamesPlayed() << endl
             << "GF: " << teams[i].getGoalsFor() << " GA: " << teams[i].getGoalsAgainst() << " Pyt: " << teams[i].getWinPct() << " Exp: " << teams[i].getExponent() << endl
             << "In Playoffs?: " << teams[i].getPlayoffStatus() << endl
             << "Playoff Appearances: " << teams[i].getPlayoffAppearances() << " Playoff Odds: " << float (teams[i].getPlayoffAppearances()) / trials * 100.0 << " Championships: " << teams[i].getChampionships() << endl
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


void League::simulate() {
    calcLeagueAverages();

    //generate random numbers for predictions
    random_device rd;
    mt19937 generator(time(0));
    uniform_real_distribution<> dis(0.0, 1.0);
    auto randomNumber = bind(dis, generator);

    cout << 1;

    for (int i = 0; i < trials; i++) {
        //cout << 2;
        simmedTeams = teams;
        simmedSched = sched;

        loadFromSchedule(simmedTeams, simmedSched);
        pointsAndPercentCalcs(simmedTeams);
        refreshTeams(simmedTeams, simmedSched);
        //cout << 3;

        float prediction = randomNumber();
        float predictionChange = randomNumber();

        //cout << "Randoms: " << prediction << " " << predictionChange << endl;

        //simulate all the games
        for (int j = 0; j < simmedSched.size(); j++) {
            //cout << 4;
            if (simmedSched[j].getPlayed()) {
                continue;
            }

            //cout << simmedSched[j].getProb();

            if (simmedSched[j].getProb() >= prediction) {
                //cout << simmedSched[j].getHomeTeam().getTSN() << " - " << simmedSched[j].getAwayTeam().getTSN();
                //cout << 6;
                ///do a cross here
                //results will change the pythag of the teams if you generate
                //a score but as long as you go through all the games before
                //updating all the probabilities, they have no effect. You will
                //need to reset the probabilities to the originals if you want
                //to display it later
                simmedSched[j].setHomeScore(round(avgGF + (simmedSched[j].getHomeTeam().getGFPG() - avgGF) + (simmedSched[j].getAwayTeam().getGAPG() - avgGF)));
                simmedSched[j].setAwayScore(round(avgGF + (simmedSched[j].getAwayTeam().getGFPG() - avgGF) + (simmedSched[j].getHomeTeam().getGAPG() - avgGF)));
                if (simmedSched[j].getAwayScore() >= simmedSched[j].getHomeScore()) {
                    simmedSched[j].setAwayScore(simmedSched[j].getHomeScore() - 1);
                }
            } else {
                //cout << 7;
                simmedSched[j].setHomeScore(round(avgGF + (simmedSched[j].getHomeTeam().getGFPG() - avgGF) + (simmedSched[j].getAwayTeam().getGAPG() - avgGF)));
                simmedSched[j].setAwayScore(round(avgGF + (simmedSched[j].getAwayTeam().getGFPG() - avgGF) + (simmedSched[j].getHomeTeam().getGAPG() - avgGF)));
                if (simmedSched[j].getHomeScore() >= simmedSched[j].getAwayScore()) {
                    simmedSched[j].setHomeScore(simmedSched[j].getAwayScore() - 1);
                }
            }

            simmedSched[j].setPlayed(true);
            simmedSched[j].setOT(false);

            prediction += predictionChange;
            if (prediction > 1) {
                prediction--;
            }
            //cout << "<<" << prediction << endl;
        }

        //cout << 5;
        loadFromSchedule(simmedTeams, simmedSched);
        pointsAndPercentCalcs(simmedTeams);
        refreshTeams(simmedTeams, simmedSched);

        standingsSort(simmedTeams);

//        if (i == 50) {
//            cout << " Simulation result: " << endl;
//            printTeams(simmedTeams);
//        }

        //cout << 7;
        for (int k = 0; k < simmedTeams.size(); k++) {
            for (int j = 0; j < teams.size(); j++) {
                if (simmedTeams[k].getTSN() == teams[j].getTSN()) {
                    teams[j].incrementOneFinishingPosition(k);
                    if (k < 6) {
                        teams[j].addPlayoffAppearance();
                    }
                    break;
                }
            }
        }
        //cout << 8;

        //cout << i;
        //break;
    }
    cout << "simmed";

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

    //copy all the titles to a separate vector
    for (int i = 0; i < teams.size(); i++) {
        midStep.push_back(teams[i].getPoints());
    }

    //sort that vector
    sort(midStep.begin(), midStep.end(), greater<int>());

    //look through a copy of the database to assign the rest of the values in the movies to their given titles
    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < teams.size(); j++) {
            if (midStep[i] == teams[j].getPoints()) {
                sorted[i] = teams[j];
                teams.erase(teams.begin() + j);
                break;
            }
        }
    }

    //move it all to the movies vector
    teams = sorted;
}

void League::sortGamesPlayed() {
    vector<Team> sorted(teams.size());
    vector<int> midStep;

    //copy all the titles to a separate vector
    for (int i = 0; i < teams.size(); i++) {
        midStep.push_back(teams[i].getGamesPlayed());
    }

    //sort that vector
    sort(midStep.begin(), midStep.end());

    //look through a copy of the database to assign the rest of the values in the movies to their given titles
    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < teams.size(); j++) {
            if (midStep[i] == teams[j].getGamesPlayed()) {
                sorted[i] = teams[j];
                teams.erase(teams.begin() + j);
                break;
            }
        }
    }

    //move it all to the movies vector
    teams = sorted;
}

void League::sortROW() {
    vector<Team> sorted(teams.size());
    vector<int> midStep;

    //copy all the titles to a separate vector
    for (int i = 0; i < teams.size(); i++) {
        midStep.push_back(teams[i].getWins() + teams[i].getOTW());
    }

    //sort that vector
    sort(midStep.begin(), midStep.end(), greater<int>());

    //look through a copy of the database to assign the rest of the values in the movies to their given titles
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

    //copy all the titles to a separate vector
    for (int i = 0; i < teams.size(); i++) {
        midStep.push_back(teams[i].getGoalsFor() - teams[i].getGoalsAgainst());
    }

    //sort that vector
    sort(midStep.begin(), midStep.end(), greater<int>());

    //look through a copy of the database to assign the rest of the values in the movies to their given titles
    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < teams.size(); j++) {
            if (midStep[i] == teams[j].getGoalsFor() - teams[j].getGoalsAgainst()) {
                sorted[i] = teams[j];
                teams.erase(teams.begin() + j);
                break;
            }
        }
    }

    //move it all to the movies vector
    teams = sorted;
}

void League::sortPyth() {
    vector<Team> sorted(teams.size());
    vector<float> midStep;

    //copy all the titles to a separate vector
    for (int i = 0; i < teams.size(); i++) {
        midStep.push_back(teams[i].getWinPct());
    }

    //sort that vector
    sort(midStep.begin(), midStep.end(), greater<float>());

    //look through a copy of the database to assign the rest of the values in the movies to their given titles
    for(int i = 0; i < midStep.size(); i++) {
        for (int j = 0; j < teams.size(); j++) {
            if (midStep[i] == teams[j].getWinPct()) {
                sorted[i] = teams[j];
                teams.erase(teams.begin() + j);
                break;
            }
        }
    }

    //move it all to the movies vector
    teams = sorted;
}

void League::playoffCut() {
    standingsSort();

    for (int i = 0; i < teams.size(); i++) {
        if (i < 6) {
            teams[i].setPlayoffs(true);
        }
    }
}

void League::calcLeagueAverages() {
    float goalsCounter = 0;
    int gamesCounter = 0;

    for (int i = 0; i < sched.size(); i++) {
        if (!sched[i].getPlayed()) {
            continue;
        } else {
            goalsCounter += sched[i].getHomeScore() + sched[i].getAwayScore();
            gamesCounter++;
        }
    }

    avgGF = float (goalsCounter) / (gamesCounter * 2.0);
    //cout << "|||||||||||||||||||||||||||||||||||||" << avgGF << endl;

    for (int i = 0; i < teams.size(); i++) {
        teams[i].calcAverages();
        //cout << teams[i].getTSN() << teams[i].getGFPG() << teams[i].getGAPG() << endl;
    }
}

/*** Simming Functions ***/


int League::readTeams(string filename, vector<Team> v) {
    ifstream teamsFile(filename);
    string temp;

    if (!teamsFile.is_open()) {
        cout << "File not found";
        return 404;
    }

    while (!teamsFile.eof()) {
        Team tempTeam;

        getline(teamsFile, temp);
        tempTeam.setName(temp);
        getline(teamsFile, temp);
        tempTeam.setTSN(temp);

        v.push_back(tempTeam);
    }

    cout << "run success" << endl;
}

int League::readSchedule(string filename, vector<Team> v) {
    ifstream schedFile(filename);
    string temp1, temp2;
    int temp3, temp4, temp5;

    if (!schedFile.is_open()) {
        cout << "File not found";
        return 404;
    }

    while (schedFile >> temp1) {
        Game tempGame;

//        getline(schedFile, temp1, ' ');
//        getline(schedFile, temp2, ' ');

        //schedFile >> temp1;
        schedFile >> temp2;

        char ab;
        //cout << temp1 << "-" << temp2;
        //cin >> ab;

        for (int i = 0; i < v.size(); i++) {
            if (v[i].getTSN() == temp1) {
                tempGame.setHomeTeam(v[i]);
                break;
            }
        }
        for (int i = 0; i < v.size(); i++) {
            if (v[i].getTSN() == temp2) {
                tempGame.setAwayTeam(v[i]);
                break;
            }
        }

        //cout << 2;

        schedFile >> temp3;

        if (temp3 == -4) {
            schedFile >> temp4;
            schedFile >> temp5;
            //schedFile.ignore();

            tempGame.setHomeScore(temp4);
            tempGame.setAwayScore(temp5);
            tempGame.setOT(true);
            tempGame.setPlayed(true);
        } else if (temp3 != -1) {
            schedFile >> temp4;
            //schedFile.ignore();

            tempGame.setHomeScore(temp3);
            tempGame.setAwayScore(temp4);
            tempGame.setOT(false);
            tempGame.setPlayed(true);
        } else {
            tempGame.setHomeScore(0);
            tempGame.setAwayScore(0);
            tempGame.setPlayed(false);
            //schedFile.ignore();
        }

        sched.push_back(tempGame);

    }

    cout << "run success" << endl;
}

void League::loadFromSchedule(vector<Team> &v, vector<Game> &s) {
    for (int i = 0; i < v.size(); i++) {
        v[i].setGoalsFor(0);
        v[i].setGoalsAgainst(0);
        v[i].setGamesPlayed(0);
        v[i].setWins(0);
        v[i].setOTW(0);
        v[i].setOTL(0);
        v[i].setLosses(0);
        v[i].setPoints(0);
    }

    for (int i = 0; i < s.size(); i++) {
        if (!s[i].getPlayed()) {
            continue;
        }
        for (int j = 0; j < v.size(); j++) {

            if (s[i].getHomeTeam().getTSN() == v[j].getTSN()) {
                s[i].setHomeTeam(v[j]);

                if (s[i].getHomeScore() > s[i].getAwayScore()) {
                    if (s[i].getOT()) {
                        v[j].setOTW(v[j].getOTW() + 1);
                    } else {
                        v[j].setWins(v[j].getWins() + 1);
                    }
                } else {
                    if (s[i].getOT()) {
                        v[j].setOTL(v[j].getOTL() + 1);
                    } else {
                        v[j].setLosses(v[j].getLosses() + 1);
                    }
                }
                v[j].setGamesPlayed(v[j].getGamesPlayed() + 1);
                v[j].setGoalsFor(v[j].getGoalsFor() + s[i].getHomeScore());
                v[j].setGoalsAgainst(v[j].getGoalsAgainst() + s[i].getAwayScore());
                break;
            }
        }
        for (int j = 0; j < v.size(); j++) {
            if (s[i].getAwayTeam().getTSN() == v[j].getTSN()) {
                s[i].setAwayTeam(v[j]);

                if (s[i].getAwayScore() > s[i].getHomeScore()) {
                    if (s[i].getOT()) {
                        v[j].setOTW(v[j].getOTW() + 1);
                    } else {
                        v[j].setWins(v[j].getWins() + 1);
                    }
                } else {
                    if (s[i].getOT()) {
                        v[j].setOTL(v[j].getOTL() + 1);
                    } else {
                        v[j].setLosses(v[j].getLosses() + 1);
                    }
                }
                v[j].setGamesPlayed(v[j].getGamesPlayed() + 1);
                v[j].setGoalsFor(v[j].getGoalsFor() + s[i].getAwayScore());
                v[j].setGoalsAgainst(v[j].getGoalsAgainst() + s[i].getHomeScore());
                break;
            }
        }
    }
}

void League::refreshTeams(vector<Team> &v, vector<Game> &s) {
    for (int i = 0; i < s.size(); i++) {
        for (int j = 0; j < v.size(); j++) {
            if (s[i].getHomeTeam().getTSN() == v[j].getTSN()) {
                s[i].setHomeTeam(v[j]);
                break;
            }
        }
        for (int j = 0; j < v.size(); j++) {
            if (s[i].getAwayTeam().getTSN() == v[j].getTSN()) {
                s[i].setAwayTeam(v[j]);
                break;
            }
        }
    }
}

void League::pointsAndPercentCalcs(vector<Team> &v) {
    for (int i = 0; i < v.size(); i++) {
        v[i].setPoints((v[i].getWins() * 3) + (v[i].getOTW() * 2) + (v[i].getOTL()));
        v[i].calcExponent();
        v[i].setWinPct(getWinPct(v[i]));
    }
}

void League::predict(vector<Game> v) {
    for (int i = 0; i < v.size(); i++) {
        if (!v[i].getPlayed()) {
            v[i].log5();
        } else if (v[i].getHomeScore() > v[i].getAwayScore()) {
            v[i].setProb(1.0);
        } else {
            v[i].setProb(0.0);
        }
    }
}

void League::printTeams(vector<Team> &v) {
    standingsSort();
    for (int i = 0; i < v.size(); i++) {
        cout << v[i].getTSN() << " - " << v[i].getName() << endl
             << "W: " << v[i].getWins() << " L: " << v[i].getLosses() << " OTW: " << v[i].getOTW() << " OTL: " << v[i].getOTL() << endl
             << "Pts: " << v[i].getPoints() << " GP: " << v[i].getGamesPlayed() << endl
             << "GF: " << v[i].getGoalsFor() << " GA: " << v[i].getGoalsAgainst() << " Pyt: " << v[i].getWinPct() << " Exp: " << v[i].getExponent() << endl
             << "In Playoffs?: " << v[i].getPlayoffStatus() << endl
             << endl;
    }
}

void League::printPredictSchedule(vector<Game> &v) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i].getPlayed()) {
            cout << v[i].getHomeTeam().getTSN() << " " << v[i].getHomeScore() << " - " << v[i].getAwayScore() << " " << v[i].getAwayTeam().getTSN() << endl;
        } else {
            cout << v[i].getHomeTeam().getTSN() << " " << v[i].getProb() << " " << v[i].getAwayTeam().getTSN() << endl;
        }
    }
}

void League::standingsSort(vector<Team> &v) {
    sortPyth(v);
    sortDifferential(v);
    sortROW(v);
    sortGamesPlayed(v);
    sortPoints(v);
}

void League::sortPoints(vector<Team> &v) {
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

void League::sortGamesPlayed(vector<Team> &v) {
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

void League::sortROW(vector<Team> &v) {
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

void League::sortDifferential(vector<Team> &v) {
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

void League::sortPyth(vector<Team> &v) {
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

void League::playoffCut(vector<Team> &v) {
    standingsSort();

    for (int i = 0; i < v.size(); i++) {
        if (i < 8) {
            v[i].setPlayoffs(true);
            v[i].addPlayoffAppearance();
        }
    }
}

void League::simulateWithPlayoffs() {
    calcLeagueAverages();

    //generate random numbers for predictions
    random_device rd;
    mt19937 generator(time(0));
    uniform_real_distribution<> dis(0.0, 1.0);
    auto randomNumber = bind(dis, generator);

    cout << 1;

    for (int i = 0; i < trials; i++) {
        //cout << 2;
        simmedTeams = teams;
        simmedSched = sched;

        loadFromSchedule(simmedTeams, simmedSched);
        pointsAndPercentCalcs(simmedTeams);
        refreshTeams(simmedTeams, simmedSched);
        //cout << 3;

        float prediction = randomNumber();
        float predictionChange = randomNumber();

        //simulate all the games
        for (int j = 0; j < simmedSched.size(); j++) {
            //cout << 4;
            if (simmedSched[j].getPlayed()) {
                continue;
            }

            if (simmedSched[j].getProb() >= prediction) {
                ///do a cross here
                //results will change the pythag of the teams if you generate
                //a score but as long as you go through all the games before
                //updating all the probabilities, they have no effect. You will
                //need to reset the probabilities to the originals if you want
                //to display it later
                simmedSched[j].setHomeScore(round(avgGF + (simmedSched[j].getHomeTeam().getGFPG() - avgGF) + (simmedSched[j].getAwayTeam().getGAPG() - avgGF)));
                simmedSched[j].setAwayScore(round(avgGF + (simmedSched[j].getAwayTeam().getGFPG() - avgGF) + (simmedSched[j].getHomeTeam().getGAPG() - avgGF)));
                if (simmedSched[j].getAwayScore() >= simmedSched[j].getHomeScore()) {
                    simmedSched[j].setAwayScore(simmedSched[j].getHomeScore() - 1);
                }
            } else {
                //cout << 7;
                simmedSched[j].setHomeScore(round(avgGF + (simmedSched[j].getHomeTeam().getGFPG() - avgGF) + (simmedSched[j].getAwayTeam().getGAPG() - avgGF)));
                simmedSched[j].setAwayScore(round(avgGF + (simmedSched[j].getAwayTeam().getGFPG() - avgGF) + (simmedSched[j].getHomeTeam().getGAPG() - avgGF)));
                if (simmedSched[j].getHomeScore() >= simmedSched[j].getAwayScore()) {
                    simmedSched[j].setHomeScore(simmedSched[j].getAwayScore() - 1);
                }
            }

            simmedSched[j].setPlayed(true);
            simmedSched[j].setOT(false);

            prediction += predictionChange;
            if (prediction > 1) {
                prediction--;
            }
        }

        loadFromSchedule(simmedTeams, simmedSched);
        pointsAndPercentCalcs(simmedTeams);
        refreshTeams(simmedTeams, simmedSched);

        standingsSort(simmedTeams);

//        if (i == 50) {
//            cout << " Simulation result: " << endl;
//            printTeams(simmedTeams);
//        }

        for (int k = 0; k < simmedTeams.size(); k++) {
            for (int j = 0; j < teams.size(); j++) {
                if (simmedTeams[k].getTSN() == teams[j].getTSN()) {
                    teams[j].incrementOneFinishingPosition(k);
                    if (k < 8) {
                        teams[j].addPlayoffAppearance();
                    }
                    break;
                }
            }
        }
    }

    for (int t = 0; t < trials; t++) {
        float prediction = randomNumber();
        vector<Team> contenders = simmedTeams;
        vector<Team> nextRoundTeams = {};
        pointsAndPercentCalcs(contenders);

        int roundIndex = 1;
        int winsRequired[4] = {4,4,4,4};
        int teamIndex = 1, playoffTeams = 8, remainingSeries = 7;

        while (remainingSeries > 0) {
            int seriesCount[2] = {0,0};

            while (seriesCount[0] != winsRequired[roundIndex] && seriesCount[1] != winsRequired[roundIndex]) {
                Game g;
                g.setHomeTeam(contenders[teamIndex - 1]);
                g.setAwayTeam(contenders[playoffTeams - teamIndex]);

                g.setHomeWP(g.getHomeTeam().getWinPct());
                g.setAwayWP(g.getAwayTeam().getWinPct());
                g.log5();

                if (g.getProb() >= prediction) {
                    seriesCount[0]++;
                } else {
                    seriesCount[1]++;
                }
                prediction = randomNumber();
            }

            if (seriesCount[0] == 4) {
                nextRoundTeams.push_back(contenders[teamIndex - 1]);
            } else {
                nextRoundTeams.push_back(contenders[playoffTeams - teamIndex]);
            }
            teamIndex++;
            remainingSeries--;

            if (remainingSeries == 1 || remainingSeries == 3) {
                standingsSort(contenders);
                contenders = nextRoundTeams;
                pointsAndPercentCalcs(contenders);
                nextRoundTeams.clear();

                playoffTeams /= 2;
                teamIndex = 1;
                roundIndex++;
            }
        }

        //cout << "advanced:" << endl;
        for (int i = 0; i < teams.size(); i++) {
            if (teams[i].getTSN() == nextRoundTeams[0].getTSN()) {
                teams[i].incrementOneChampionship();
            }
        }
        //printTeams(nextRoundTeams);
        //cout << "printed contenders";
    }
    cout << "simmed";

}
