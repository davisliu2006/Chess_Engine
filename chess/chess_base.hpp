#pragma once

#include <array>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../headers/system.hpp"
#include "../headers/rng.hpp"
#define DEBUG_MODE
#include "../headers/debug.hpp"

namespace chess {
    using std::array, std::vector, std::set, std::map;
    using std::string, std::pair;
    using std::cin, std::cout;

    // DEFINITIONS

    // pair of x and y coordinates
    using move_t = pair<int,int>;
    // pair of white and black score, respectively
    using move_score_t = pair<double,double>;

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
    inline std::ostream& operator <<(std::ostream& out, const ChessPiece& piece) {
        return out << piece.type << piece.iswhite << '@' << piece.x << piece.y;
    }

    // MOVE PAIR
    /*
    Pair containing chess piece and move position.
    */
    struct move_pair_t {
        ChessPiece* piece = NULL;
        move_t move = {0, 0};

        static move_pair_t INVALID() {return {NULL, {0, 0}};}
        bool is_invalid() const {return piece == NULL;}
    };
    // output
    inline std::ostream& operator <<(std::ostream& out, const move_pair_t& mp) {
        if (mp.is_invalid()) {return out << "INVALID MOVE PAIR";}
        return out << *mp.piece << " to " << mp.move.first << mp.move.second;
    }

    // MOVE PAIR SCORE
    /*
    Pair containing move_pair_t and move_score_t.
    */
    struct move_pair_score_t {
        move_pair_t move_pair = move_pair_t::INVALID();
        move_score_t move_score = {0, 0};

        bool is_invalid() const {return move_pair.is_invalid();}
    };
    // output
    inline std::ostream& operator <<(std::ostream& out, const move_pair_score_t& mps) {
        // if (mps.is_invalid()) {return out << "INVALID MOVE PAIR SCORE";}
        return out << mps.move_pair << " (" << mps.move_score.first << ',' << mps.move_score.second << ')';
    }

    // CHESS BOARD
    struct ChessBoard {
        array<array<ChessPiece*, 8>, 8> grid ;
        array<set<ChessPiece*>, 2> pieces;
        array<ChessPiece*, 2> kings = {NULL, NULL};

        // constructors
        ChessBoard() {
            for (auto& row: grid) {row.fill(NULL);}
        }

        ChessPiece*& pos(int x, int y) {
            return grid[x][y];
        }

        // add piece
        void add_piece(ChessPiece& piece, int x, int y) {
            debug_assert(0 <= x && x < 8 && 0 <= y && y < 8);
            pieces[piece.iswhite].insert(&piece);
            grid[piece.x = x][piece.y = y] = &piece;
            piece.onboard = true;
        }
        void add_piece(bool iswhite, char type, int x, int y) {
            ChessPiece* piece = new ChessPiece(iswhite, type);
            add_piece(*piece, x, y);
        }

        // remove piece
        void rem_piece(ChessPiece& piece) {
            auto f = pieces[piece.iswhite].find(&piece);
            debug_assert(f != pieces[piece.iswhite].end());
            pieces[piece.iswhite].erase(f);
            grid[piece.x][piece.y] = NULL;
            piece.onboard = false;
        }

        // move piece
        void move_piece(ChessPiece& piece, int x, int y) {
            debug_assert(0 <= x && x < 8 && 0 <= y && y < 8);
            grid[piece.x][piece.y] = NULL;
            grid[piece.x = x][piece.y = y] = &piece;
        }
        void move_piece(int x0, int y0, int x1, int y1) {
            debug_assert(0 <= x0 && x0 < 8 && 0 <= y0 && y0 < 8);
            debug_assert(0 <= x1 && x1 < 8 && 0 <= y1 && y1 < 8);
            ChessPiece* piece = grid[x0][y0];
            debug_assert(piece);
            grid[x0][y0] = NULL;
            grid[piece->x = x1][piece->y = y1] = piece;
        }

        // clear board
        void clear() {
            for (auto& row: grid) {
                row.fill(NULL);
            }
            pieces[0].clear(); pieces[1].clear();
            kings = {NULL, NULL};
        }
        // default set up board
        void default_setup() {
            for (int iw = 0; iw <= 1; iw++) {
                add_piece(iw, rook, 0, 7-7*iw);
                add_piece(iw, knight, 1, 7-7*iw);
                add_piece(iw, bishop, 2, 7-7*iw);
                add_piece(iw, queen, 3, 7-7*iw);
                add_piece(iw, king, 4, 7-7*iw);
                add_piece(iw, bishop, 5, 7-7*iw);
                add_piece(iw, knight, 6, 7-7*iw);
                add_piece(iw, rook, 7, 7-7*iw);
                for (int i = 0; i < 8; i++) {
                    add_piece(iw, pawn, i, 6-5*iw);
                }
            }
            kings = {grid[4][7], grid[4][0]};
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
                if (!piece->onboard) {
                    cout << "-";
                }
                cout << piece->type << "1@" << piece->x << piece->y << " ";
            }
            for (ChessPiece* piece: pieces[0]) {
                if (!piece->onboard) {
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
        vector<move_t> get_moves(const ChessPiece& piece);
        bool is_check(bool iswhite);
        bool is_checkmate(bool iswhite);
        void print_moves(const ChessPiece& piece);
        void print_all_moves(bool iswhite);
        vector<move_pair_t> get_all_moves(bool iswhite);

        // defined in chess_ai.hpp
        vector<move_pair_score_t> get_move_scores(int r, bool iswhite);
        move_pair_score_t get_best_move(int r, bool iswhite);
    };
}