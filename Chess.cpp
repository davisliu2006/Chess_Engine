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
    for (ChessPiece* pc: board->whitepcs) {
        cout << pc->x << pc->y << '\n';
        board->printmoves(pc);
    }

    return 0;
}