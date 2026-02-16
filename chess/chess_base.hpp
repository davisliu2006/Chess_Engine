/*
chess_base.hpp
Defines base classes for chess library.
*/

#pragma once

#include <array>
#include <functional>
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

    // target score - opponent score
    using score_t = double;
    // pair of x and y coordinates
    struct pos_t {
        int x, y;
    };

    // piece type identifiers
    enum piece_t: char {
        pawn = 1, knight = 2,
        bishop = 3, rook = 4,
        queen = 5, king = 7
    };
    const int PIECE_T_SIZE = 8;

    // CLASSES

    // pre-declarations
    struct ChessPiece;
    struct ChessBoard;

    struct CompressedPiece {
        char val = 0;
        CompressedPiece() {}
        bool iswhite() const {return val & char(0x80);}
        piece_t type() const {return piece_t(val & char(0x7f));}
        operator bool() const {return val != 0;}
        operator char() const {return val;}
    };
    struct CompressedBoard {
        array<CompressedPiece,64> grid;
        CompressedBoard() {}
    };

    // CHESS PIECE
    struct ChessPiece {
        int x = 0; // 0-7
        int y = 0; // 0-7
        bool iswhite;
        piece_t type;
        bool onboard = false;

        // constructors
        ChessPiece(bool iswhite, piece_t type): iswhite(iswhite), type(type) {}
    };

    // MOVE
    /*
    Pair containing chess piece and move position.
    */
    struct move_t {
        ChessPiece* piece = NULL;
        pos_t pos = {0, 0};

        static move_t INVALID() {return {NULL, {0, 0}};}
        bool is_invalid() const {return piece == NULL;}
    };

    // MOVE SCORE PAIR
    /*
    Pair containing move_t and score_t.
    */
    struct move_score_t {
        move_t move = move_t::INVALID();
        score_t score = 0;

        bool is_invalid() const {return move.is_invalid();}
    };

    // CHESS BOARD
    struct ChessBoard {
        array<array<ChessPiece*, 8>, 8> grid;
        array<vector<ChessPiece*>, 2> pieces;
        array<ChessPiece*, 2> kings = {NULL, NULL};
        vector<ChessPiece> _dealloc;

        // constructors
        ChessBoard() {
            for (auto& row: grid) {row.fill(NULL);}
            pieces[0].reserve(16);
            pieces[1].reserve(16);
            _dealloc.reserve(32);
        }
        ChessBoard(const ChessBoard& board) = delete;

        // destructor
        ~ChessBoard() {}

        // operators
        ChessBoard& operator =(const ChessBoard& board) = delete;

        // add piece
        void add_piece(ChessPiece& piece, int x, int y) {
            debug_assert(0 <= x && x < 8 && 0 <= y && y < 8);
            grid[piece.x = x][piece.y = y] = &piece;
            piece.onboard = true;
        }
        ChessPiece* create_piece(bool iswhite, piece_t type) {
            _dealloc.push_back({iswhite, type});
            return &_dealloc.back();
        }
        ChessPiece* create_piece(bool iswhite, piece_t type, int x, int y) {
            ChessPiece* piece = create_piece(iswhite, type);
            add_piece(*piece, x, y);
            pieces[iswhite].push_back(piece);
            return piece;
        }

        // remove piece
        void rem_piece(ChessPiece& piece) {
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
            for (auto& row: grid) {row.fill(NULL);}
            pieces[0].clear();
            pieces[1].clear();
            kings = {NULL, NULL};
            _dealloc.clear();
        }
        // default set up board
        void default_setup() {
            for (int iw = 0; iw <= 1; iw++) {
                create_piece(iw, rook, 0, 7-7*iw);
                create_piece(iw, knight, 1, 7-7*iw);
                create_piece(iw, bishop, 2, 7-7*iw);
                create_piece(iw, queen, 3, 7-7*iw);
                create_piece(iw, king, 4, 7-7*iw);
                create_piece(iw, bishop, 5, 7-7*iw);
                create_piece(iw, knight, 6, 7-7*iw);
                create_piece(iw, rook, 7, 7-7*iw);
                for (int i = 0; i < 8; i++) {
                    create_piece(iw, pawn, i, 6-5*iw);
                }
            }
            kings = {grid[4][7], grid[4][0]};
        }

        // defined in chess_debug.cpp
        void print_pcs() const;
        void print_board() const;

        // defined in chess_moves.cpp
        vector<pos_t> get_moves(const ChessPiece& piece);
        bool is_check(bool iswhite);
        [[deprecated]] bool is_checkmate(bool iswhite);
        void print_moves(const ChessPiece& piece);
        void print_all_moves(bool iswhite);
        vector<move_t> get_all_moves(bool iswhite);

        // defined in chess_ai.cpp
        score_t get_score(bool iswhite) const;
        [[deprecated]] score_t get_score_of(int r, const move_t& move);
        move_score_t get_best_move(int r, bool iswhite);
    };

    inline void copy_board(ChessBoard& trg, const ChessBoard& ref) {
        for (const ChessPiece& ref_pc: ref._dealloc) {
            ChessPiece* trg_pc = trg.create_piece(ref_pc.iswhite, ref_pc.type, ref_pc.x, ref_pc.y);
            if (ref_pc.type == king) {
                assert(!trg.kings[ref_pc.iswhite]);
                trg.kings[ref_pc.iswhite] = trg_pc;
            }
        }
    }
}