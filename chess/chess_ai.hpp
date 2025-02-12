#pragma once

#include "chess_base.hpp"

namespace chess {
    // significance values of each piece type
    inline map<char,double> piece_val = {
        {pawn, 1}, {knight, 3}, {bishop, 3},
        {rook, 5}, {queen, 10}, {king, 1000}
    };
    // significance of having more moves availible
    inline double pos_weighting = 0.1;

    enum playstyle_e {
        AGGRESSIVE, PASSIVE, NEUTRAL
    };
    inline playstyle_e playstyle = NEUTRAL;
}