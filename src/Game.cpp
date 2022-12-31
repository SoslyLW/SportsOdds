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
}

void Game::log5() {
    prob = (home.getWinPct() - (home.getWinPct() * away.getWinPct()))/(home.getWinPct() + away.getWinPct() - (2 * home.getWinPct() * away.getWinPct()));

    //pointer version for safekeeping
    //prob = ((*home).getWinPct() - ((*home).getWinPct() * (*away).getWinPct()))/((*home).getWinPct() + (*away).getWinPct() - (2 * (*home).getWinPct() * (*away).getWinPct()));
}
