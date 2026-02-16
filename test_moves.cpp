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
    ifstream sample_in("chess/test_data/move_correctness/test1.txt");
    assert(sample_in.is_open() && "Sample input file failed to open.");

    while (!sample_in.eof()) {
        board.clear(); // reset board before input
        sample_in >> board;
        bool white_turn = true;
        cout << "FINISH SETUP" << endl;

        board.print_board();
        board.print_pcs();
        cout << "White moves:\n";
        board.print_all_moves(true);
        cout << "Black moves:\n";
        board.print_all_moves(false);
        move_score_t best_mps = board.get_best_move(5, white_turn);
        cout << best_mps << '\n';
    }

    return 0;
}