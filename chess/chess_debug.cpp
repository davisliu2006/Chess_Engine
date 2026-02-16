#include "chess_base.hpp"
#include "chess_board.hpp"
#include "chess_debug.hpp"

using namespace chess;

void ChessBoard::print_pcs() const {
    for (ChessPiece* piece: pieces[1]) {
        if (!piece->onboard) {
            cout << "-";
        }
        cout << piece->type << "1@" << piece->x << piece->y << " ";
    }
    for (ChessPiece* piece: pieces[0]) {
        if (!piece->onboard) {
            cout << "-";
        }
        cout << piece->type << "0@" << piece->x << piece->y << " ";
    }
    cout << '\n' << std::flush;
}

void ChessBoard::print_board() const {
    for (int y = 7; y >= 0; y--) {
        cout << y << " ";
        for (int x = 0; x <= 7; x++) {
            ChessPiece* piece = grid[x][y];
            if (piece) {
                cout << piece->type << piece->iswhite << " ";
            } else {
                cout << "-- ";
            }
        }
        cout << '\n';
    }
    cout << "  ";
    for (int x = 0; x <= 7; x++) {
        cout << x << "  ";
    }
    cout << std::flush;
}