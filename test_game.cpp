#define DEBUG_MODE

#include <array>
#include <fstream>
#include <iostream>
#include <vector>

#include "headers/system.hpp"
#include "headers/rng.hpp"
#include "chess/chess.hpp"

using namespace std;
using namespace chess;

//MAIN

int main() {
    cout << "START PROGRAM" << endl;
    rng_init(time(NULL));
    ChessBoard board;
    // ifstream sample_in("chess/test_data/test1.txt");
    // assert(sample_in.is_open() && "Sample input file failed to open.");
    // sample_in >> board;
    board.default_setup();
    bool white_turn = true;
    cout << "FINISH SETUP" << endl;

    for (int i = 0; i < 20; i++) {
        board.print_board();
        board.print_pcs();
        // cout << "White moves:\n";
        // board.print_all_moves(true);
        // cout << "Black moves:\n";
        // board.print_all_moves(false);
        double t0 = time();
        move_score_t best_mps = board.get_best_move(4, white_turn);
        double t = time();
        cout << best_mps << " computed in " << t-t0 << '\n';
        if (best_mps.is_invalid()) {return 0;}
        const auto& [move, score] = best_mps;
        const auto& [piece, pos] = move;

        ChessPiece* captpiece = board.grid[pos.x][pos.y];
        if (captpiece) {board.rem_piece(*captpiece);}
        board.move_piece(*piece, pos.x, pos.y);

        white_turn = !white_turn;
        cout << '\n';
    }

    return 0;
}