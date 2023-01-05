#ifndef TEAM_H
#define TEAM_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>

using namespace std;

class Team {
    public:
        Team();

        //getters and setters
        int getWins() { return w; };
        void setWins(int n) { w = n; };
        int getLosses() { return l; };
        void setLosses(int n) { l = n; };
        int getOTL() { return otl; };
        void setOTL(int n) { otl = n; };
        int getOTW() { return otw; };
        void setOTW(int n) { otw = n; };
        int getTies() { return t; };
        void setTies(int n) { t = n; };
        int getGamesPlayed() { return gp; };
        void setGamesPlayed(int n) { gp = n; };
        int getGoalsFor() { return gf; };
        void setGoalsFor(int n) { gf = n; };
        int getGoalsAgainst() { return ga; };
        void setGoalsAgainst(int n) { ga = n; };
        int getPoints() { return pts; };
        void setPoints(int n) { pts = n; };

        string getName() { return name; };
        void setName(string s) { name = s; };
        string getTSN() { return tsn; };
        void setTSN(string s) { tsn = s; };

        int getConference() { return conference; };
        void setConference(int n) { conference = n; };
        int getDivision() { return division; };
        void setDivision(int n) { division = n; };

        float getWinPct() { return winpct; };
        void setWinPct(float n) { winpct = n; };
        float getExponent() { return exponent; };
        void setExponent(float n) { exponent = n; };
        void calcExponent();

        void setPlayoffs(bool t) { inPlayoffs = t; };
        bool getPlayoffStatus() { return inPlayoffs; };
        void addPlayoffAppearance() { timesInPlayoffs += 1; };
        int getPlayoffAppearances() { return timesInPlayoffs; };
        vector<int> getFinishingPositions() { return finishingPositions; };
        int getOneFinishingPosition(int i) { return finishingPositions[i]; };
        void incrementOneFinishingPosition(int p) { finishingPositions[p] += 1; };
        void incrementOneChampionship() { championships += 1; };

    private:
        int w, l, otl, otw, t, gp, gf, ga, pts;
        string name, tsn;
        float winpct, exponent;

        int conference, division;
        int timesInPlayoffs, championships;
        vector<int> finishingPositions;
        bool inPlayoffs;
};

#endif // TEAM_H
