#pragma once

#include "chess_moves.hpp"

// WIP
[[deprecated]] inline vector<move_pair_score_t> ChessBoard::get_move_scores(int r, bool iswhite) {
    vector<move_pair_score_t> val;
    vector<move_pair_t> moves = get_all_moves(iswhite);
    for (auto& [piece, move]: moves) {
        ChessPiece* captpiece = grid[move.first][move.second];
        int x0 = piece->x, y0 = piece->y; // save old state
        // test move
        if (captpiece) {rem_piece(*captpiece);}
        move_piece(*piece, move.first, move.second);
        if (r == 1) {
            val.push_back({{piece, move}, {get_score(true), get_score(false)}});
        } else {
            vector<move_pair_score_t> solve = get_move_scores(r-1, !iswhite);
            move_pair_score_t bestmove = {};
            for (const auto& [mvpr, score]: solve) {

            }
            val.push_back({{piece, move}, bestmove.move_score});
        }
        // backtrack
        move_piece(*piece, x0, y0);
        if (captpiece) {add_piece(*captpiece, captpiece->x, captpiece->y);}
    }
    return val;
}

enum playstyle_e {
    AGGRESSIVE, PASSIVE, NEUTRAL
};
inline playstyle_e playstyle = NEUTRAL;

// search best move with recursion depth "r"
inline move_pair_score_t ChessBoard::get_best_move(int r, bool iswhite) {
    vector<move_pair_score_t> vals; // all "best moves"
    double best_advatage = -1e9; // best score difference
    vector<move_pair_t> moves = get_all_moves(iswhite); // get all possible moves
    if (moves.size() == 0) { // no valid moves, LOSE
        return {move_pair_t::INVALID(), { // losing slightly later (lower "r") is better
            iswhite? -10000-r*10 : get_score(false),
            iswhite? get_score(true) : -10000-r*10
        }};
    }
    for (const auto& [piece, move]: moves) { // check score for each move
        const auto& [x, y] = move;
        ChessPiece* captpiece = grid[x][y];
        int x0 = piece->x, y0 = piece->y; // save old state
        // test move
        if (captpiece) {rem_piece(*captpiece);} // capture
        move_piece(*piece, x, y);
        if (r == 1) { // base case
            move_score_t score = {get_score(true), get_score(false)};
            double advantage = (score.first-score.second)*(iswhite? 1 : -1);
            if (advantage > best_advatage) { // better than current
                best_advatage = advantage;
                vals.clear();
            }
            if (advantage >= best_advatage) { // better than or same as current
                vals.push_back({{piece, move}, score});
            }
        } else {
            move_pair_score_t next_move = get_best_move(r-1, !iswhite);
            const move_score_t& next_score = next_move.move_score;
            move_score_t score = next_score;
            double advantage = (score.first-score.second)*(iswhite? 1 : -1);
            if (advantage > best_advatage) { // better than current
                best_advatage = advantage;
                vals.clear();
            }
            if (advantage >= best_advatage) { // better than or same as current
                vals.push_back({{piece, move}, score});
            }
        }
        // backtrack
        move_piece(*piece, x0, y0);
        if (captpiece) { // un-capture
            add_piece(*captpiece, captpiece->x, captpiece->y);
        }
    }
    return vals[randint(0, vals.size()-1)];
}