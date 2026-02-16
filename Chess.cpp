#define DEBUG_MODE

#include <array>
#include <iostream>
#include <stack>
#include <vector>

#include "headers/system.hpp"
#include "headers/rng.hpp"
#include "chess/chess.hpp"
#include "chess/chess_openings.hpp"
#include "chess/chess_state.hpp"

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

    GameStateManager history;
    int64_t opening = 0;

    array<bool,2> is_computer = {true, false};
    while (true) {
        history.push(board, opening);
        cout << "Move #" << history.index()+1 << '\n';
        cout << (white_turn? "White" : "Black") << " to move " << (is_computer[white_turn]? "(computer)" : "(human)") << " \n";
        board.print_board();
        board.print_pcs();

        if (is_computer[white_turn]) {
            int opening1;
            if (history.index() <= 1 && (opening1 = openings::get_best_opening(opening))) {
                move_pair_t move_pair = openings::to_move_pair(board, opening1);
                auto& [piece, pos] = move_pair;

                ChessPiece* captpiece = board.grid[pos.x][pos.y];
                if (captpiece) {board.rem_piece(*captpiece);}
                board.move_piece(*piece, pos.x, pos.y);
                opening = (opening << 16) | opening1;
            } else {
                move_pair_score_t best_mps = board.get_best_move(4, white_turn);
                cout << best_mps << '\n';
                if (best_mps.is_invalid()) {return 0;}
                auto& [move_pair, move_score] = best_mps;
                auto& [piece, pos] = move_pair;

                ChessPiece* captpiece = board.grid[pos.x][pos.y];
                if (captpiece) {board.rem_piece(*captpiece);}
                board.move_piece(*piece, pos.x, pos.y);
            }
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
                if (x0 == -1 && y0 == -1 && history.index() >= 2) { // undo
                    undo = true;
                    break;
                }
                for (const auto& move_pair: moves) {
                    const auto& [piece, pos] = move_pair;
                    if (x0 == piece->x && y0 == piece->y
                    && x1 == pos.x && y1 == pos.y) {
                        selected = &move_pair;
                        break;
                    }
                }
                if (selected) {break;}
                cout << "Invalid move\n";
            }

            if (undo) {
                history.undo(); history.undo();
                GameStateManager::reconstruct_board(board, history.current().board);
                opening = history.current().opening;
                history.undo();
                cout << '\n';
                continue;
            } else {
                const auto& [piece, pos] = *selected;
                ChessPiece* captpiece = board.grid[pos.x][pos.y];
                if (captpiece) {board.rem_piece(*captpiece);}
                opening = (opening << 16) | (piece->x << 12) | (piece->y << 8) | (pos.x << 4) | (pos.y);
                board.move_piece(*piece, pos.x, pos.y);
            }
        }

        white_turn = !white_turn;
        cout << '\n';
    }

    return 0;
}