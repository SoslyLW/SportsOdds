#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>

#include "Team.h"

using namespace std;

class Game {
    public:
        Game();

        //getters and setters
        int getHomeScore() { return hScore; };
        void setHomeScore(int n) { hScore = n; };
        int getAwayScore() { return aScore; };
        void setAwayScore(int n) { aScore = n; };
        Team getHomeTeam() { return home; };
        void setHomeTeam(Team t) { home = t; };
        Team getAwayTeam() { return away; };
        void setAwayTeam(Team t) { away = t; };
        bool getOT() { return ot; };
        void setOT(bool b) { ot = b; };
        bool getPlayed() { return played; };
        void setPlayed(bool b) { played = b; };
        float getHomeWP() { return hWP; };
        void setHomeWP(float f) { hWP = f; };
        float getAwayWP() { return aWP; };
        void setAwayWP(float f) { aWP = f; };
        double getProb() { return prob; };
        void setProb(float f) { prob = f; };

        void log5();

    private:
        Team home, away;
        int hScore, aScore;
        bool ot, played;
        float hWP, aWP;
        double prob;
};

#endif // GAME_H
