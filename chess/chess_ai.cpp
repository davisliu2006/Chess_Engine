/*
chess_ai.cpp
Defines AI algorithm implementation.
*/

#include <thread>
#include "chess_base.hpp"
#include "chess_board.hpp"
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

// search move score with recursion depth "r"
score_t ChessBoard::get_score_recursive(const move_t& move, int r) {
    const auto& [piece, pos] = move;
    bool iswhite = piece->iswhite;
    const auto& [x, y] = pos;
    // we need to store return value to backtrack the board state before returning
    score_t val;

    ChessPiece* captpiece = grid[x][y];
    auto x0 = piece->x, y0 = piece->y; // save old state
    // test move
    if (captpiece) {rem_piece(*captpiece);} // capture
    move_piece(*piece, x, y);

    if (is_check(piece->iswhite)) { // if illegal
        val = INVALID_SCORE;
    } else if (r == 1) { // base case
        val = get_score(iswhite)-get_score(!iswhite);
    } else {
        // get all possible opponent moves
        auto opp_moves = get_all_moves(!iswhite);
        // best score difference opponent can achieve
        score_t best_advantage = INVALID_SCORE;
        for (const auto& move: opp_moves) { // check each opponent move
            score_t advantage = get_score_recursive(move, r-1);
            best_advantage = max(best_advantage, advantage);
        }
        // no valid moves: check lose or stalemate
        if (best_advantage == INVALID_SCORE) {
            if (is_check(!iswhite)) {
                best_advantage = LOSE_SCORE(r-1);
            } else {
                best_advantage = STALEMATE_SCORE;
            }
        }
        val = -best_advantage;
    }

    // backtrack
    move_piece(*piece, x0, y0);
    if (captpiece) { // un-capture
        add_piece(*captpiece, captpiece->x, captpiece->y);
    }

    return val;
}

// get best move with recursion depth "r"
move_score_t ChessBoard::get_best_move(bool iswhite, int r) {
    vector<move_score_t> candidates; // all "best moves"
    candidates.reserve(8);
    score_t best_advantage = -1e9;

    // test all possible moves
    auto moves = get_all_moves(iswhite);
    for (const auto& move: moves) {
        score_t advantage = get_score_recursive(move, r);
        if (advantage == INVALID_SCORE) {continue;}
        if (advantage > best_advantage) { // better than current
            best_advantage = advantage;
            candidates.clear();
            candidates.push_back({move, advantage});
        } else if (advantage == best_advantage) { // same as current
            candidates.push_back({move, advantage});
        }
    }

    if (candidates.size() == 0) { // no valid moves, LOSE or STALEMATE
        if (is_check(iswhite)) {return {move_t::INVALID(), LOSE_SCORE(r)};}
        else return {move_t::INVALID(), STALEMATE_SCORE};
    }
    return candidates[randint(0, candidates.size()-1)]; // return random from list of best
}

// get best move with recursion depth "r"
move_score_t ChessBoard::get_best_move_concurrent(bool iswhite, int r) {
    vector<move_score_t> candidates; // all "best moves"
    candidates.reserve(8);
    score_t best_advantage = -1e9;

    // test all possible moves
    auto moves = get_all_moves(iswhite);
    const int n_moves = moves.size();
    const int n_threads = min<int>(moves.size(), thread::hardware_concurrency());
    int block_size = (n_moves+n_threads-1)/n_threads;
    vector<score_t> scores(n_moves);
    vector<thread> threads;
    vector<ChessBoard> boards(n_threads);
    for (int i = 0; i < n_threads; i++) {
        int i0 = block_size*i;
        int i1 = block_size*(i+1);
        copy_board(boards[i], *this);
        threads.push_back(thread(
            [this, r](ChessBoard* board, move_t* m_begin, score_t* s_begin, int size) {
                for (int j = 0; j < size; j++) {
                    m_begin[j].piece += &board->_dealloc[0]-&this->_dealloc[0];
                    s_begin[j] = board->get_score_recursive(m_begin[j], r);
                    m_begin[j].piece -= &board->_dealloc[0]-&this->_dealloc[0];
                }
            },
            &boards[i], &moves.front()+i0, &scores.front()+i0, min(i1, n_moves)-i0
        ));
    }
    for (thread& t: threads) {t.join();}
    
    for (int i = 0; i < n_moves; i++) {
        const auto& move = moves[i];
        const score_t& advantage = scores[i];
        if (advantage == INVALID_SCORE) {continue;}
        if (advantage > best_advantage) { // better than current
            best_advantage = advantage;
            candidates.clear();
            candidates.push_back({move, advantage});
        } else if (advantage == best_advantage) { // same as current
            candidates.push_back({move, advantage});
        }
    }

    if (candidates.size() == 0) { // no valid moves, LOSE or STALEMATE
        if (is_check(iswhite)) {return {move_t::INVALID(), LOSE_SCORE(r)};}
        else return {move_t::INVALID(), STALEMATE_SCORE};
    }
    return candidates[randint(0, candidates.size()-1)]; // return random from list of best
}