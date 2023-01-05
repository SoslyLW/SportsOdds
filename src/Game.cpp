#include "Game.h"

Game::Game() {
    hScore = 0;
    aScore = 0;
    ot = false;
    so = false;
    played = false;
    hWP = false;
    aWP = false;
    prob = 0.5;

    ranProb = 0;
}

void Game::log5() {
    //prob = (home.getWinPct() - (home.getWinPct() * away.getWinPct()))/(home.getWinPct() + away.getWinPct() - (2 * home.getWinPct() * away.getWinPct()));

    //pointer version for safekeeping
    //prob = ((*home).getWinPct() - ((*home).getWinPct() * (*away).getWinPct()))/((*home).getWinPct() + (*away).getWinPct() - (2 * (*home).getWinPct() * (*away).getWinPct()));

    prob = (home.getWinPct() * (1.0 - away.getWinPct())) / ((home.getWinPct() * (1.0 - away.getWinPct())) + ((1.0 - home.getWinPct()) * away.getWinPct()));
}
