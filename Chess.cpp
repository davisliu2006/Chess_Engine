#include <array>
#include <iostream>
#include <list>
#include <queue>
#include <vector>
#include <SDL2/SDL.h>

#include "chess/chess.hpp"

using namespace std;

//MAIN

int main() {
    ChessBoard* board = new ChessBoard();
    board->setup();
    board->printboard();
    board->printpcs();
    board->print_all_moves(true);

    return 0;
}