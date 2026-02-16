/*
chess_debug.hpp
*/

#pragma once

#include "chess_base.hpp"
#include "chess_board.hpp"

namespace chess {
    constexpr array<char,PIECE_T_SIZE> PIECE_TO_CHAR = []() {
        array<char,PIECE_T_SIZE> val = {};
        val[pawn] = 'p'; val[knight] = 'k';
        val[bishop] = 'b'; val[rook] = 'r';
        val[queen] = 'Q'; val[king] = 'K';
        return val;
    }();
    constexpr array<piece_t,256> CHAR_TO_PIECE = []() {
        array<piece_t,256> val = {};
        val['p'] = pawn; val['k'] = knight;
        val['b'] = bishop; val['r'] = rook;
        val['Q'] = queen; val['K'] = king;
        return val;
    }();
    
    // output piece_t
    inline std::ostream& operator <<(std::ostream& out, piece_t pc) {
        return out << PIECE_TO_CHAR[pc];
    }

    // output ChessPiece
    inline std::ostream& operator <<(std::ostream& out, const ChessPiece& piece) {
        return out << piece.type << piece.iswhite << '@' << piece.x << piece.y;
    }

    // output move_t
    inline std::ostream& operator <<(std::ostream& out, const move_t& move) {
        if (move.is_invalid()) {return out << "INVALID MOVE";}
        return out << *move.piece << " to " << move.pos.x << move.pos.y;
    }

    // output move_score_t
    inline std::ostream& operator <<(std::ostream& out, const move_score_t& ms) {
        return out << ms.move << " (" << ms.score << ')';
    }

    // input ChessBoard
    inline std::istream& operator >>(std::istream& in, ChessBoard& board) {
        for (int y = 7; y >= 0; y--) {
            for (int x = 0; x <= 7; x++) {
                string pcstr;
                in >> pcstr;
                cout << pcstr << (x == 7? '\n' : ' ') << std::flush;
                if (pcstr[0] == '-') {
                    // do nothing
                } else {
                    piece_t type = CHAR_TO_PIECE[pcstr[0]];
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