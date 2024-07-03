#pragma once

#include "chess_base.hpp"

inline istream& operator >>(istream& in, ChessBoard& board) {
    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j <= 7; j++) {
            string pcstr;
            in >> pcstr;
            cout << pcstr << (j == 7? '\n' : ' ') << flush;
            if (pcstr[0] == '-') {
                //
            } else {
                char type = pcstr[0];
                bool iswhite = (pcstr[1] == '1');
                board.add_piece(iswhite, type, j, i);
            }
        }
    }
    return in;
}