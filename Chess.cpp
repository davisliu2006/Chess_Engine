#define DEBUG_MODE

#include <array>
#include <iostream>
#include <stack>
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

    stack<CompressedBoard> history;

    array<bool,2> is_computer = {true, false};
    while (true) {
        history.push(board.compress());
        cout << "Move #" << history.size() << '\n';
        cout << (white_turn? "White" : "Black") << " to move " << (is_computer[white_turn]? "(computer)" : "(human)") << " \n";
        board.print_board();
        board.print_pcs();

        if (is_computer[white_turn]) {
            move_pair_score_t best_mps = board.get_best_move(4, white_turn);
            cout << best_mps << '\n';
            if (best_mps.is_invalid()) {return 0;}
            auto& [move_pair, move_score] = best_mps;
            auto& [piece, move] = move_pair;

            ChessPiece* captpiece = board.grid[move.first][move.second];
            if (captpiece) {board.rem_piece(*captpiece);}
            board.move_piece(*piece, move.first, move.second);
        } else {
            cout << "Player turn to move <x0, y0, x1, y1>: ";
            int x0 = -1, y0 = -1, x1 = -1, y1 = -1;
            vector<move_pair_t> moves = board.get_all_moves(white_turn);
            // cout << moves.size() << '\n';
            const move_pair_t* selected = NULL;
            bool undo = false;
            while (true) {
                cin >> x0 >> y0 >> x1 >> y1;
                cin.ignore(1e5, '\n');
                if (x0 == -1 && y0 == -1 && history.size() >= 3) { // undo
                    undo = true;
                    break;
                }
                for (const auto& move_pair: moves) {
                    const auto& [piece, move] = move_pair;
                    if (x0 == piece->x && y0 == piece->y
                    && x1 == move.first && y1 == move.second) {
                        selected = &move_pair;
                        break;
                    }
                }
                if (selected) {break;}
                cout << "Invalid move\n";
            }

            if (undo) {
                history.pop(); history.pop();
                ChessBoard::reconstruct(board, history.top()); history.pop();
                cout << '\n';
                continue;
            } else {
                const auto& [piece, move] = *selected;
                ChessPiece* captpiece = board.grid[move.first][move.second];
                if (captpiece) {board.rem_piece(*captpiece);}
                board.move_piece(*piece, move.first, move.second);
            }
        }

        white_turn = !white_turn;
        cout << '\n';
    }

    return 0;
}