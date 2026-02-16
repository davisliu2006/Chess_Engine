/*
chess_board.hpp
*/

#pragma once

#include "chess_base.hpp"

namespace chess {
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