#pragma once

#include "chess_base.hpp"

namespace chess {
    using std::unordered_map;

    namespace openings {
        extern unordered_map<int64_t, vector<int>> mp;

        inline int get_best_opening(int64_t opening) {
            const vector<int>& q = mp[opening];
            if (q.size() == 0) {return 0;}
            return mp[opening][randint(0, q.size()-1)];
        };
        inline move_pair_t to_move_pair(const ChessBoard& board, int x) {
            return {board.grid[(x>>12)&0xf][(x>>8)&0xf], {(x>>4)&0xf, x&0xf}};
        }
    }
}