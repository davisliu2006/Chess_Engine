/*
chess_ai.hpp
Defines AI algorithm parameters.
*/

#pragma once

#include "chess_base.hpp"

namespace chess {
    namespace chess_ai {
        // significance values of each piece type
        constexpr array<score_t,PIECE_T_SIZE> PIECE_VAL = []() {
            array<score_t,PIECE_T_SIZE> val = {};
            val[pawn] = 1; val[knight] = 3;
            val[bishop] = 3; val[rook] = 5;
            val[queen] = 10; val[king] = 1000;
            return val;
        }();
        // significance of having more moves availible
        [[deprecated]] inline score_t POS_FACTOR = 0.1;
        
        // special rules
        inline score_t PAWN_STACK_PENALTY = 0.5; // pawn stack unfavorable
        inline score_t PAWN_ISOLATE_PENALTY = 0.5; // isolated pawn unfavorable
        inline score_t PAWN_PROXIMITY_BONUS = 0.3; // pawn approaching opponent king
        inline score_t BISHOP_PARITY_BONUS = 1; // opposite parity bishops
        inline score_t KNIGHT_PROXIMITY_BONUS = 0.5; // knight close to opponent king
        inline score_t QUEEN_PROXIMITY_BONUS = 0.5; // queen close to opponent king
        inline score_t CASTLE_BONUS = 1; // castling
        inline score_t pawn_advance_bonus(bool iswhite, int y) {
            if (iswhite && y >= 5) {return score_t(y-4)/3*0.2;}
            if (!iswhite && y <= 2) {return score_t(3-y)/3*0.2;}
            return 0;
        }

        inline score_t LOSE_SCORE(int r) {
            return -10000-r;
        }
        inline score_t LOSE_SCORE_CUTOFF = -10000;
        inline score_t STALEMATE_SCORE = 0;
        inline score_t INVALID_SCORE = -INFINITY;
    }
}