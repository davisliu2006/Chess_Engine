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
                if (type == king) {
                    assert(!board.kings[iswhite] && "Board cannot have more than one king.");
                    board.kings[iswhite] = board.grid[j][i];
                }
            }
        }
    }
    assert(board.kings[0] && board.kings[1] && "Board must have exactly two kings.");
    return in;
}