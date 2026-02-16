/*
chess_ai.cpp
Defines AI algorithm implementation.
*/

#include "chess_base.hpp"
#include "chess_ai.hpp"
#include "../headers/math.hpp"

using namespace std;
using namespace chess;
using namespace chess::chess_ai;

// 0 is black square, 1 is white square
static bool square_parity(int x, int y) {
    return (x+y)%2;
}

// get score for either side, not accounting for move possibilities
score_t ChessBoard::get_score(bool iswhite) const {
    score_t val = 0; // score value

    array<int,8> pawn_count = {}; // special rules: pawns
    int bishop_color = 0; // special rules: bishop

    for (ChessPiece* piece: pieces[iswhite]) { // check all pieces
        if (!piece->onboard) {continue;}
        
        val += PIECE_VAL[piece->type]; // material value
        ChessPiece* opp_king = kings[!iswhite];

        if (piece->type == pawn) { // special rules: pawns
            pawn_count[piece->x]++;
            val += pawn_advance_bonus(iswhite, piece->y);
            if (abs(opp_king->x-piece->x) <= 1) {
                val += PAWN_PROXIMITY_BONUS/dist(opp_king->x-piece->x, opp_king->y-piece->y);
            }
        }
        if (piece->type == bishop) { // special rules: bishops
            bishop_color |= (square_parity(piece->x, piece->y)? 1 : 2);
        }
        if (piece->type == knight) { // special rules: knight
            val += KNIGHT_PROXIMITY_BONUS/dist(opp_king->x-piece->x, opp_king->y-piece->y);
        }
        if (piece->type == queen) { // special rules: queen
            val += QUEEN_PROXIMITY_BONUS/dist(opp_king->x-piece->x, opp_king->y-piece->y);
        }
    }

    // special rules: pawns
    for (int i = 0; i <= 7; i++) {
        if (pawn_count[i] >= 2) { // pawn stack unfavorable
            val -= (pawn_count[i]-1)*PAWN_STACK_PENALTY;
        }
        if ((i == 0 || pawn_count[i-1] == 0)
        && (i == 7 || pawn_count[i+1] == 0)) { // isolated pawn unfavorable
            val -= PAWN_ISOLATE_PENALTY;
        }
    }
    // special rules: bishops
    val += (bishop_color == 3)*BISHOP_PARITY_BONUS; // favour oposite-parity bishops

    return val;
}

// search best move with recursion depth "r"
move_score_t ChessBoard::get_best_move(int r, bool iswhite) {
    // DEFINE MACROS
    // losing later is better (lower "r")
    #define LOSE_SCORE -10000-r*10 - get_score(!iswhite)
    #define STALEMATE_SCORE 0

    vector<move_score_t> vals; // all "best moves"
    vals.reserve(8);
    score_t best_advatage = -1e9; // best score difference
    vector<move_t> moves = get_all_moves(iswhite); // get all possible moves
    if (moves.size() == 0) { // no valid moves, LOSE
        return {move_t::INVALID(), LOSE_SCORE};
    }
    for (const auto& [piece, pos]: moves) { // check score for each move
        const auto& [x, y] = pos;
        ChessPiece* captpiece = grid[x][y];
        int x0 = piece->x, y0 = piece->y; // save old state
        // test move
        if (captpiece) {rem_piece(*captpiece);} // capture
        move_piece(*piece, x, y);
        if (is_check(iswhite)) { // if illegal, END CHECK
            // backtrack
            move_piece(*piece, x0, y0);
            if (captpiece) { // un-capture
                add_piece(*captpiece, captpiece->x, captpiece->y);
            }
            continue;
        }
        if (r == 1) { // base case
            score_t score = get_score(iswhite)-get_score(!iswhite);
            score_t advantage = score;
            if (advantage > best_advatage) { // better than current
                best_advatage = advantage;
                vals.clear();
            }
            if (advantage >= best_advatage) { // better than or same as current
                vals.push_back({{piece, pos}, score});
            }
        } else {
            move_score_t next_move = get_best_move(r-1, !iswhite);
            const score_t& next_score = next_move.score;
            score_t score = -next_score;
            score_t advantage = score;
            if (advantage > best_advatage) { // better than current
                best_advatage = advantage;
                vals.clear();
            }
            if (advantage >= best_advatage) { // better than or same as current
                vals.push_back({{piece, pos}, score});
            }
        }
        // backtrack
        move_piece(*piece, x0, y0);
        if (captpiece) { // un-capture
            add_piece(*captpiece, captpiece->x, captpiece->y);
        }
    }
    if (vals.size() == 0) { // no valid moves, LOSE or STALEMATE
        if (is_check(iswhite)) {return {move_t::INVALID(), LOSE_SCORE};}
        else return {move_t::INVALID(), STALEMATE_SCORE};
    }
    return vals[randint(0, vals.size()-1)]; // return random from list of best

    // UNDEFINE MACROS
    #undef LOSE_SCORE
    #undef STALEMATE_SCORE
}