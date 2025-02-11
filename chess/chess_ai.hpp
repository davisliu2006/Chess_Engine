#pragma once

#include "chess_base.hpp"

namespace chess {
    // significance values of each piece type
    inline map<char,double> pieceval = {
        {pawn, 1}, {knight, 3}, {bishop, 3},
        {rook, 5}, {queen, 10}, {king, 1000}
    };

    enum playstyle_e {
        AGGRESSIVE, PASSIVE, NEUTRAL
    };
    inline playstyle_e playstyle = NEUTRAL;
}