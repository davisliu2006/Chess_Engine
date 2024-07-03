#pragma once

#include <array>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../headers/system.hpp"
#include "../headers/rng.hpp"

using namespace std;

// DEFINITIONS

using move_t = pair<int,int>; // x and y
using move_score_t = pair<double,double>; // white and black score

// piece type identifiers
enum piece_e : char {
    pawn = 'p', knight = 'k',
    bishop = 'b', rook = 'r',
    queen = 'Q', king = 'K'
};
// significance values of each piece type
inline map<char,double> pieceval = {
    {pawn, 1}, {knight, 3}, {bishop, 3},
    {rook, 5}, {queen, 10}, {king, 1000}
};

//CLASSES

// pre-declarations
class ChessPiece;
class ChessBoard;

// CHESS PIECE
struct ChessPiece {
    bool iswhite;
    char type;
    int x = 0; // 0-7
    int y = 0; // 0-7
    bool onboard = false;

    // constructors
    ChessPiece(bool iswhite1, char type1) {
        iswhite = iswhite1;
        type = type1;
    }
};
// output
inline ostream& operator <<(ostream& out, const ChessPiece& piece) {
    return out << piece.type << piece.iswhite << '@' << piece.x << piece.y;
}

// piece and move position pair
struct move_pair_t {
    ChessPiece* piece;
    move_t move;

    static move_pair_t INVALID() {return {NULL, {0, 0}};}
    bool is_invalid() const {return piece == NULL;}
};
// output
inline ostream& operator <<(ostream& out, const move_pair_t& mp) {
    if (mp.is_invalid()) {return out << "INVALID MOVE PAIR";}
    return out << *mp.piece << " to " << mp.move.first << mp.move.second;
}

// move_pair and move score pair
struct move_pair_score_t {
    move_pair_t move_pair;
    move_score_t move_score;\

    bool is_invalid() const {return move_pair.is_invalid();}
};
// output
inline ostream& operator <<(ostream& out, const move_pair_score_t& mps) {
    // if (mps.is_invalid()) {return out << "INVALID MOVE PAIR SCORE";}
    return out << mps.move_pair << " (" << mps.move_score.first << ',' << mps.move_score.second << ')';
}

// CHESS BOARD
struct ChessBoard {
    ChessPiece* grid[8][8];
    array<set<ChessPiece*>,2> pieces;
    array<ChessPiece*,2> kings = {NULL, NULL};

    // constructors
    ChessBoard() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                grid[i][j] = NULL;
            }
        }
    }

    ChessPiece*& pos(int x, int y) {
        return grid[x][y];
    }

    // add piece
    void add_piece(bool iswhite, char type, int x, int y) {
        ChessPiece* piece = new ChessPiece(iswhite, type);
        pieces[iswhite].insert(piece);
        grid[piece->x = x][piece->y = y] = piece;
        piece->onboard = true;
    }
    void add_piece(ChessPiece* piece, int x, int y) {
        pieces[piece->iswhite].insert(piece);
        grid[piece->x = x][piece->y = y] = piece;
        piece->onboard = true;
    }

    // remove piece
    void rem_piece(ChessPiece* piece) {
        pieces[piece->iswhite].erase(piece);
        grid[piece->x][piece->y] = NULL;
        piece->onboard = false;
    }

    // move piece
    void move_piece(ChessPiece* piece, int x, int y) {
        grid[piece->x][piece->y] = NULL;
        grid[piece->x = x][piece->y = y] = piece;
    }
    void move_piece(int x0, int y0, int x1, int y1) {
        ChessPiece* piece = grid[x0][y0];
        grid[x0][y0] = NULL;
        grid[piece->x = x1][piece->y = y1] = piece;
    }

    // clear board
    void clear() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                grid[i][j] = NULL;
            }
        }
        pieces[0].clear(); pieces[1].clear();
        kings = {NULL, NULL};
    }
    // default set up board
    void default_setup() {
        for (int a = 0; a <= 1; a++) {
            add_piece(a, rook, 0, 7-7*a);
            add_piece(a, knight, 1, 7-7*a);
            add_piece(a, bishop, 2, 7-7*a);
            add_piece(a, queen, 3, 7-7*a);
            add_piece(a, king, 4, 7-7*a);
            add_piece(a, bishop, 5, 7-7*a);
            add_piece(a, knight, 6, 7-7*a);
            add_piece(a, rook, 7, 7-7*a);
            for (int b = 0; b < 8; b++) {
                add_piece(a, pawn, b, 6-5*a);
            }
        }
    }

    // get score for either side
    double get_score(bool iswhite) {
        double val = 0;
        for (ChessPiece* piece: pieces[iswhite]) {
            val += pieceval[piece->type];
        }
        return val;
    }

    // print pieces as list
    void print_pcs() {
        for (ChessPiece* piece: pieces[1]) {
            if (not piece->onboard) {
                cout << "-";
            }
            cout << piece->type << "1@" << piece->x << piece->y << " ";
        }
        for (ChessPiece* piece: pieces[0]) {
            if (not piece->onboard) {
                cout << "-";
            }
            cout << piece->type << "0@" << piece->x << piece->y << " ";
        }
        cout << '\n';
    }
    // print board as grid
    void print_board() {
        for (int i = 7; i >= 0; i--) {
            for (int j = 0; j <= 7; j++) {
                ChessPiece* piece = grid[j][i];
                if (piece) {
                    cout << piece->type << piece->iswhite << " ";
                } else {
                    cout << "-- ";
                }
            }
            cout << '\n';
        }
    }

    // defined in moves.hpp
    vector<move_t> get_moves(ChessPiece* piece);
    bool is_check(bool iswhite);
    bool is_checkmate(bool iswhite);
    void print_moves(ChessPiece* piece);
    void print_all_moves(bool iswhite);
    vector<move_pair_t> get_all_moves(bool iswhite);

    // defined in chess_ai.hpp
    vector<move_pair_score_t> get_move_scores(int r, bool iswhite);
    move_pair_score_t get_best_move(int r, bool iswhite);
};