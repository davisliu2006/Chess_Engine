#pragma once

#include "chess_moves.hpp"

inline vector<move_pair_score_t> ChessBoard::get_move_scores(int r, bool iswhite) {
    vector<move_pair_score_t> val;
    if (r == 1) {
        vector<move_pair_t> moves = ChessBoard::get_all_moves(iswhite);
        for (move_pair_t& movepair: moves) {
            
        }
    } else {
        
    }
    return val;
}