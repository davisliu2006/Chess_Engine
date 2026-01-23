/*
chess_debug.hpp
*/

#pragma once

#include "chess_base.hpp"

namespace chess {
    inline std::istream& operator >>(std::istream& in, ChessBoard& board) {
        for (int y = 7; y >= 0; y--) {
            for (int x = 0; x <= 7; x++) {
                string pcstr;
                in >> pcstr;
                cout << pcstr << (x == 7? '\n' : ' ') << std::flush;
                if (pcstr[0] == '-') {
                    // do nothing
                } else {
                    char type = pcstr[0];
                    bool iswhite = (pcstr[1] == '1');
                    board.create_piece(iswhite, type, x, y);
                    if (type == king) {
                        assert(!board.kings[iswhite] && "Board cannot have more than one king.");
                        board.kings[iswhite] = board.grid[x][y];
                    }
                }
            }
        }
        assert(board.kings[0] && board.kings[1] && "Board must have exactly two kings.");
        return in;
    }
}