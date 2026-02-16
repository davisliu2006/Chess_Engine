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
    rng_init(0);
    ChessBoard board;
    board.default_setup();
    bool white_turn = true;
    cout << "FINISH SETUP" << endl;

    const int N = 20;
    double t_tot = 0;

    for (int i = 0; i < N; i++) {
        board.print_board();
        board.print_pcs();
        double t0 = time();
        move_score_t best_mps = board.get_best_move(white_turn, 4);
        double t = time();
        cout << best_mps << " computed in " << t-t0 << '\n';
        t_tot += t-t0;
        if (best_mps.is_invalid()) {return 0;}
        const auto& [move, score] = best_mps;
        const auto& [piece, pos] = move;

        ChessPiece* captpiece = board.grid[pos.x][pos.y];
        if (captpiece) {board.rem_piece(*captpiece);}
        board.move_piece(*piece, pos.x, pos.y);

        white_turn = !white_turn;
        cout << '\n';
    }

    cout << "Average time per move: " << t_tot/N << endl;

    return 0;
}