#include <array>
#include <iostream>
#include <list>
#include <queue>
#include <vector>

#include "headers/system.hpp"
#include "headers/rng.hpp"
#include "chess/chess.hpp"

using namespace std;

//MAIN

int main() {
    rng_init(time(NULL));
    ChessBoard* board = new ChessBoard();
    board->setup();
    bool white_turn = false;

    for (int i = 0; i < 5; i++) {
        board->printboard();
        board->printpcs();
        move_pair_score_t best_mps = board->get_best_move(5, white_turn);
        cout << best_mps << '\n';
        auto& [move_pair, move_score] = best_mps;
        auto& [piece, move] = move_pair;

        ChessPiece* captpiece = board->grid[move.first][move.second];
        if (captpiece) {board->rempiece(captpiece);}
        board->movepiece(piece, move.first, move.second);

        white_turn = !white_turn;
    }

    return 0;
}