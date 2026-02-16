/*
chess_ai.hpp
Defines AI algorithm parameters.
*/

#pragma once

#include "chess_base.hpp"

namespace chess {
    namespace chess_ai {
        // significance values of each piece type
        inline map<char,score_t> PIECE_VAL = {
            {pawn, 1}, {knight, 3}, {bishop, 3},
            {rook, 5}, {queen, 10}, {king, 1000}
        };
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
    }
}