#include "chess_base.hpp"
#include "chess_openings.hpp"

using namespace std;
using namespace chess;

unordered_map<int64_t, vector<int>> openings::mp = {
    {0x0000000000000000, {0x3133, 0x4143, 0x6052}},
    {0x0000000000003133, {0x3634, 0x3635}},
    {0x0000000000004143, {0x4644, 0x4645}},
    {0x0000000000006052, {0x3634, 0x6755}}
};