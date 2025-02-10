#define DEBUG_MODE

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
    cout << "START PROGRAM" << endl;
    rng_init(time(NULL));
    ChessBoard board;
    ifstream sample_in("chess/test_data/test2.txt");
    cout << sample_in.is_open() << endl;
    sample_in >> board;
    // board->default_setup();
    bool white_turn = true;
    cout << "FINISH SETUP" << endl;

    for (int i = 0; i < 7; i++) {
        board.print_board();
        board.print_pcs();
        cout << "white moves:\n";
        board.print_all_moves(true);
        cout << "black moves:\n";
        board.print_all_moves(false);
        move_pair_score_t best_mps = board.get_best_move(5, white_turn);
        cout << best_mps << '\n';
        if (best_mps.is_invalid()) {return 0;}
        auto& [move_pair, move_score] = best_mps;
        auto& [piece, move] = move_pair;

        ChessPiece* captpiece = board.grid[move.first][move.second];
        if (captpiece) {board.rem_piece(*captpiece);}
        board.move_piece(*piece, move.first, move.second);

        white_turn = !white_turn;
    }

    return 0;
}