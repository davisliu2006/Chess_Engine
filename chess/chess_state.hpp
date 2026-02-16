/*
chess_state.hpp
Defines classes and functions that help track the game's state.
*/

#pragma once

#include <deque>
#include <stack>

#include "chess_base.hpp"
#include "chess_board.hpp"

namespace chess {
    using std::stack, std::deque;

    struct GameState {
        // add any other game state flags to this class
        CompressedBoard board;
        int64_t opening;
        GameState(const CompressedBoard& cb): board(cb), opening(0) {}
        GameState(const CompressedBoard& cb, int64_t opening): board(cb), opening(opening) {}
    };

    struct GameStateManager {
        protected:
        deque<GameState> history;
        int i = -1;

        public:
        void push(const ChessBoard& board, int64_t opening = 0) {
            if (++i < history.size()) {
                history[i] = GameState(compress_board(board), opening);
            } else {
                history.push_back(GameState(compress_board(board), opening));
            }
        }
        void undo() {
            assert(i-- >= 0 && "Attempted to pop from empty GameStateManager");
        }
        void go_to(int i1) {
            assert(0 <= i1 && i1 < history.size() && "Attempted to go to invalid GameState index");
            i = i1;
        }
        
        GameState current() {
            return history[i];
        }
        int size() {return history.size();}
        int index() {return i;}
        int move_n() {return i+1;}

        static CompressedBoard compress_board(const ChessBoard& board) {
            CompressedBoard cb;
            for (int x = 0; x <= 7; x++) {
                for (int y = 0; y <= 7; y++) {
                    ChessPiece* piece = board.grid[x][y];
                    if (piece) {
                        cb.grid[x*8+y].val = piece->type | char(0x80)*piece->iswhite;
                    }
                }
            }
            return cb;
        }
        static void reconstruct_board(ChessBoard& board, const CompressedBoard& trg) {
            // reset data
            for (auto& row: board.grid) {row.fill(NULL);}
            board.pieces[0].clear();
            board.pieces[1].clear();
            board.kings = {NULL, NULL};
            board._dealloc.clear();
            // fill board
            for (int x = 0; x <= 7; x++) {
                for (int y = 0; y <= 7; y++) {
                    CompressedPiece cp = trg.grid[x*8+y];
                    if (cp) {
                        bool iswhite = cp.iswhite();
                        piece_t type = cp.type();
                        board.create_piece(iswhite, type, x, y);
                        if (type == king) {
                            assert(!board.kings[iswhite] && "Board cannot have more than one king.");
                            board.kings[iswhite] = board.grid[x][y];
                        }
                    } else {
                        board.grid[x][y] = NULL;
                    }
                }
            }
            assert(board.kings[0] && board.kings[1] && "Board must have exactly two kings.");
        }
    };
}