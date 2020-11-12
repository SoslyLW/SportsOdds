#include "Team.h"

Team::Team() {
    //ctor
    w = 0;
    l = 0;
    otw = 0;
    otl = 0;
    gp = 0;
    pts = 0;
    gf = 0;
    ga = 0;
    t = 0;
    winpct = 0;
    exponent = 0;

    conference = 0;
    division = 0;

    ///fixed value
    vector<int> v(10, 0);
    finishingPositions = v;

    timesInPlayoffs = 0;
    championships = 0;
    inPlayoffs = false;
}

void Team::calcExponent() {
    exponent = pow((gf + ga) / gp, 0.287);
//    cout << exponent << gf << ga << "=";
}

void Team::calcAverages() {
    gfpg = float (gf) / gp;
    gapg = float (ga) / gp;
}
