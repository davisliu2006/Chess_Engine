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

using move_t = pair<int,int>;
using move_score_t = pair<double,double>;

enum piece_e : char {
    pawn = 'p', knight = 'k',
    bishop = 'b', rook = 'r',
    queen = 'Q', king = 'K'
};
inline map<piece_e,double> pieceval = {
    {pawn, 1}, {knight, 3}, {bishop, 3},
    {rook, 5}, {queen, 10}
};

//CLASSES

class ChessPiece;
class ChessBoard;

struct ChessPiece {
    bool iswhite;
    char type;
    int x = 0; // 1-8
    int y = 0; // 1-8
    bool onboard = false;

    ChessPiece(bool iswhite1, piece_e type1) {
        iswhite = iswhite1;
        type = type1;
    }
};
inline ostream& operator <<(ostream& out, const ChessPiece& piece) {
    return out << piece.type << piece.iswhite << '@' << piece.x << piece.y;
}

struct move_pair_t {
    ChessPiece* piece;
    move_t move;
};
inline ostream& operator <<(ostream& out, const move_pair_t& mp) {
    return out << *mp.piece << " to " << mp.move.first << mp.move.second;
}
struct move_pair_score_t {
    move_pair_t move_pair;
    move_score_t move_score;
};
inline ostream& operator <<(ostream& out, const move_pair_score_t& mps) {
    return out << mps.move_pair << " (" << mps.move_score.first << ',' << mps.move_score.second << ')';
}

struct ChessBoard {
    ChessPiece* grid[8][8];
    array<set<ChessPiece*>,2> pieces;
    array<ChessPiece*,2> kings = {NULL, NULL};

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

    void addpiece(bool iswhite, piece_e type, int x, int y) {
        ChessPiece* piece = new ChessPiece(iswhite, type);
        pieces[iswhite].insert(piece);
        grid[piece->x = x][piece->y = y] = piece;
        piece->onboard = true;
    }
    void addpiece(ChessPiece* piece, int x, int y) {
        pieces[piece->iswhite].insert(piece);
        grid[piece->x = x][piece->y = y] = piece;
        piece->onboard = true;
    }

    void rempiece(ChessPiece* piece) {
        pieces[piece->iswhite].erase(piece);
        grid[piece->x][piece->y] = NULL;
        piece->onboard = false;
    }

    void movepiece(ChessPiece* piece, int x, int y) {
        grid[piece->x][piece->y] = NULL;
        grid[piece->x = x][piece->y = y] = piece;
    }
    void movepiece(int x0, int y0, int x1, int y1) {
        ChessPiece* piece = grid[x0][y0];
        grid[x0][y0] = NULL;
        grid[piece->x = x1][piece->y = y1] = piece;
    }

    void setup() {
        for (int a = 0; a <= 1; a++) {
            addpiece(a, rook, 0, 7-7*a);
            addpiece(a, knight, 1, 7-7*a);
            addpiece(a, bishop, 2, 7-7*a);
            addpiece(a, queen, 3, 7-7*a);
            addpiece(a, king, 4, 7-7*a);
            addpiece(a, bishop, 5, 7-7*a);
            addpiece(a, knight, 6, 7-7*a);
            addpiece(a, rook, 7, 7-7*a);
            for (int b = 0; b < 8; b++) {
                addpiece(a, pawn, b, 6-5*a);
            }
        }
    }

    double get_score(bool iswhite) {
        double val = 0;
        for (ChessPiece* piece: pieces[iswhite]) {
            val += pieceval[(piece_e)piece->type];
        }
        return val;
    }

    void printpcs() {
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
    void printboard() {
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

    vector<move_t> getmoves(ChessPiece* piece);
    bool ischeck(bool iswhite);
    void printmoves(ChessPiece* piece);
    void print_all_moves(bool iswhite);
    vector<move_pair_t> get_all_moves(bool iswhite);
    vector<move_pair_score_t> get_move_scores(int r, bool iswhite);
    move_pair_score_t get_best_move(int r, bool iswhite);
};