#pragma once

#include "chess_moves.hpp"

inline vector<move_pair_score_t> ChessBoard::get_move_scores(int r, bool iswhite) {
    vector<move_pair_score_t> val;
    vector<move_pair_t> moves = get_all_moves(iswhite);
    for (auto& [piece, move]: moves) {
        ChessPiece* captpiece = grid[move.first][move.second];
        int x0 = piece->x, y0 = piece->y; // save old state
        // test move
        if (captpiece) {rempiece(captpiece);}
        movepiece(piece, move.first, move.second);
        if (r == 1) {
            val.push_back({{piece, move}, {get_score(true), get_score(false)}});
        } else {
            vector<move_pair_score_t> solve = get_move_scores(r-1, !iswhite);
            move_pair_score_t bestmove = {};
            for (auto& [mvpr, score]: solve) {

            }
            val.push_back({{piece, move}, bestmove.move_score});
        }
        // backtrack
        movepiece(piece, x0, y0);
        if (captpiece) {addpiece(captpiece, captpiece->x, captpiece->y);}
    }
    return val;
}