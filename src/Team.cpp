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

    //East = 0, West = 1
    conference = 0;
    //Atlantic = 0, Met = 1, Central = 2, Pacific = 3
    division = 0;

    ///fixed value
    vector<int> v(32, 0);
    finishingPositions = v;

    timesInPlayoffs = 0;
}

void Team::calcExponent() {
    exponent = pow((gf + ga) / gp, 0.287);

    //alternatively determined exponent by studies
    //exponent = 1.927;
}
