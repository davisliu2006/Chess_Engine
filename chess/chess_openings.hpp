#include "chess_base.hpp"

namespace chess {
    using std::unordered_map;

    namespace openings {
        extern unordered_map<int64_t, vector<int>> mp;

        inline move_pair_t get_best_opening(int64_t opening, ChessBoard& board) {
            int x = mp[opening][randint(0, mp[opening].size()-1)];
            return {board.grid[(x>>12)&0xf][(x>>8)&0xf], {(x>>4)&0xf, x&0xf}};
        };
    }
}