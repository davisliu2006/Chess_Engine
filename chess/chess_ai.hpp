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

enum playstyle_e {
    AGGRESSIVE, PASSIVE, NEUTRAL
};
inline playstyle_e playstyle = NEUTRAL;

inline move_pair_score_t ChessBoard::get_best_move(int r, bool iswhite) {
    vector<move_pair_score_t> vals;
    double best_advatage = -1e9;
    vector<move_pair_t> moves = get_all_moves(iswhite);
    for (auto& [piece, move]: moves) {
        ChessPiece* captpiece = grid[move.first][move.second];
        int x0 = piece->x, y0 = piece->y; // save old state
        // test move
        if (captpiece) {rempiece(captpiece);}
        movepiece(piece, move.first, move.second);
        if (r == 1) {
            move_score_t score = {get_score(true), get_score(false)};
            double advantage = (score.first-score.second)*(iswhite? 1 : -1);
            if (advantage > best_advatage) {
                best_advatage = advantage;
                vals.clear();
            }
            if (advantage >= best_advatage) {
                vals.push_back({{piece, move}, score});
            }
        } else {
            move_pair_score_t next_move = get_best_move(r-1, !iswhite);
            const move_score_t& next_score = next_move.move_score;
            move_score_t score = next_score;
            double advantage = (score.first-score.second)*(iswhite? 1 : -1);
            if (advantage > best_advatage) {
                best_advatage = advantage;
                vals.clear();
            }
            if (advantage >= best_advatage) {
                vals.push_back({{piece, move}, score});
            }
        }
        // backtrack
        movepiece(piece, x0, y0);
        if (captpiece) {addpiece(captpiece, captpiece->x, captpiece->y);}
    }
    return vals[randint(0, vals.size()-1)];
}