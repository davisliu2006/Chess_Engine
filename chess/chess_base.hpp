/*
chess_base.hpp
Defines base classes for chess library.
*/

#pragma once

#include <array>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#include "../headers/system.hpp"
#include "../headers/rng.hpp"
#define DEBUG_MODE
#include "../headers/debug.hpp"

namespace chess {
    using std::array, std::vector;
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
}