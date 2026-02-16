/*
chess_moves.hpp
Defines move calculation implementation.
*/

#include "chess_base.hpp"

using namespace std;
using namespace chess;

static constexpr array<pos_t,8> knight_mov = {
    pos_t{-1, -2}, pos_t{-1, 2}, pos_t{1, -2}, pos_t{1, 2},
    pos_t{-2, -1}, pos_t{-2, 1}, pos_t{2, -1}, pos_t{2, 1}
};
static constexpr array<pos_t,8> king_mov = {
    pos_t{-1, -1}, pos_t{-1, 0}, pos_t{-1, 1},
    pos_t{0, -1}, pos_t{0, 1},
    pos_t{1, -1}, pos_t{1, 0}, pos_t{1, 1},
};

// get all moves for a piece
vector<pos_t> ChessBoard::get_moves(const ChessPiece& piece) {
    const auto& x = piece.x;
    const auto& y = piece.y;
    const bool& iswhite = piece.iswhite;
    int x1, y1;

    vector<pos_t> moves; // all valid moves
    moves.reserve(16);

    if (piece.type == pawn) { // PAWN
        if (0 < y && y < 7 && !grid[x1 = x][y1 = y-1+2*iswhite]) { // single step
            moves.push_back({x1, y1});
            if (y == 6-5*iswhite && !grid[x1 = x][y1 = y-2+4*iswhite]) { // double step
                moves.push_back({x1, y1});
            }
        }
        if (x > 0 && 0 < y && y < 7) { // left capture
            ChessPiece* captpiece = grid[x1 = x-1][y1 = y-1+2*iswhite];
            if (captpiece && captpiece->iswhite != iswhite) {
                moves.push_back({x1, y1});
            }
        }
        if (x < 7 && 0 < y && y < 7) { // right caputre
            ChessPiece* captpiece = grid[x1 = x+1][y1 = y-1+2*iswhite];
            if (captpiece && captpiece->iswhite != iswhite) {
                moves.push_back({x1, y1});
            }
        }
    }
    
    if (piece.type == knight) { // KNIGHT
        for (auto [i, j]: knight_mov) { // 1 hori, 2 vtc
            x1 = x+i; y1 = y+j;
            if (0 <= x1 && x1 <= 7 && 0 <= y1 && y1 <= 7) { // in range
                ChessPiece* captpiece = grid[x1][y1];
                if (!captpiece || captpiece->iswhite != iswhite) { // empty or capture
                    moves.push_back({x1, y1});
                }
            }
        }
    }
    
    if (piece.type == bishop or piece.type == queen) { // BISHOP or QUEEN
        for (int i = 1; true; i++) { // down, left
            x1 = x-i; y1 = y-i;
            if (x1 >= 0 && y1 >= 0) { // in range
                ChessPiece* captpiece = grid[x1][y1];
                if (!captpiece) { // empty
                    moves.push_back({x1, y1});
                } else if (captpiece->iswhite != iswhite) { // capture
                    moves.push_back({x1, y1}); break;
                } else { // blocked
                    break;
                }
            } else { // out of range
                break;
            }
        }
        for (int i = 1; true; i++) { // up, left
            x1 = x-i; y1 = y+i;
            if (x1 >= 0 && y1 <= 7) { // in range
                ChessPiece* captpiece = grid[x1][y1];
                if (!captpiece) { // empty
                    moves.push_back({x1, y1});
                } else if (captpiece->iswhite != iswhite) { // capture
                    moves.push_back({x1, y1}); break;
                } else { // blocked
                    break;
                }
            } else { // out of range
                break;
            }
        }
        for (int i = 1; true; i++) { // down, right
            x1 = x+i; y1 = y-i;
            if (x1 <= 7 && y1 >= 0) { // in range
                ChessPiece* captpiece = grid[x1][y1];
                if (!captpiece) { // empty
                    moves.push_back({x1, y1});
                } else if (captpiece->iswhite != iswhite) { // capture
                    moves.push_back({x1, y1}); break;
                } else { // blocked
                    break;
                }
            } else { // out of range
                break;
            }
        }
        for (int i = 1; true; i++) { // up, right
            x1 = x+i; y1 = y+i;
            if (x1 <= 7 && y1 <= 7) { // in range
                ChessPiece* captpiece = grid[x1][y1];
                if (!captpiece) { // empty
                    moves.push_back({x1, y1});
                } else if (captpiece->iswhite != iswhite) { // capture
                    moves.push_back({x1, y1}); break;
                } else { // blocked
                    break;
                }
            } else { // out of range
                break;
            }
        }
    }
    
    if (piece.type == rook or piece.type == queen) { // ROOK or QUEEN
        for (int i = 1; true; i++) { // left
            x1 = x-i; y1 = y;
            if (x1 >= 0) { // in range
                ChessPiece* captpiece = grid[x1][y1];
                if (!captpiece) { // empty
                    moves.push_back({x1, y1});
                } else if (captpiece->iswhite != iswhite) { // capture
                    moves.push_back({x1, y1}); break;
                } else { // blocked
                    break;
                }
            } else { // out of range
                break;
            }
        }
        for (int i = 1; true; i++) { // right
            x1 = x+i; y1 = y;
            if (x1 <= 7) { // in range
                ChessPiece* captpiece = grid[x1][y1];
                if (!captpiece) { // empty
                    moves.push_back({x1, y1});
                } else if (captpiece->iswhite != iswhite) { // capture
                    moves.push_back({x1, y1}); break;
                } else { // blocked
                    break;
                }
            } else { // out of range
                break;
            }
        }
        for (int i = 1; true; i++) { // down
            x1 = x; y1 = y-i;
            if (y1 >= 0) { // in range
                ChessPiece* captpiece = grid[x1][y1];
                if (!captpiece) { // empty
                    moves.push_back({x1, y1});
                } else if (captpiece->iswhite != iswhite) { // capture
                    moves.push_back({x1, y1}); break;
                } else { // blocked
                    break;
                }
            } else { // out of range
                break;
            }
        }
        for (int i = 1; true; i++) { // up
            x1 = x; y1 = y+i;
            if (y1 <= 7) { // in range
                ChessPiece* captpiece = grid[x1][y1];
                if (!captpiece) { // empty
                    moves.push_back({x1, y1});
                } else if (captpiece->iswhite != iswhite) { // capture
                    moves.push_back({x1, y1}); break;
                } else { // blocked
                    break;
                }
            } else { // out of range
                break;
            }
        }
    }
    
    if (piece.type == king) { // KING
        for (auto [i, j]: king_mov) {
            x1 = x+i; y1 = y+j;
            if ((i == 0 && j == 0) || x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7) {continue;}
            ChessPiece* captpiece = grid[x1][y1];
            if (!captpiece || captpiece->iswhite != iswhite) { // empty or capture
                moves.push_back({x1, y1});
            }
        }
    }

    return moves;
}

// check if side is "in check"
bool ChessBoard::is_check(bool iswhite) {
    ChessPiece* king = kings[iswhite];
    assert(king && "Board does not have king.");
    for (ChessPiece* piece: pieces[!iswhite]) { // for each opposing piece
        if (!piece->onboard) {continue;}
        vector<pos_t> moves = get_moves(*piece);
        for (const auto& [mx, my]: moves) { // for each move
            if (mx == king->x && my == king->y) { // can capture king
                return true;
            }
        }
    }
    return false;
}
// WIP
[[deprecated]] inline bool ChessBoard::is_checkmate(bool iswhite) {

}

// print moves for a piece
void ChessBoard::print_moves(const ChessPiece& piece) {
    vector<pos_t> moves = get_moves(piece);
    for (const auto& [mx, my]: moves) {
        cout << mx << my << ' ';
    }
    cout << '\n';
}
// print moves for all pieces on a side
void ChessBoard::print_all_moves(bool iswhite) {
    for (ChessPiece* pc: ChessBoard::pieces[iswhite]) {
        if (!pc->onboard) {continue;}
        cout << pc->type << pc->iswhite << '@' << pc->x << pc->y << ": ";
        ChessBoard::print_moves(*pc);
    }
}

// get all moves for a side
vector<move_t> ChessBoard::get_all_moves(bool iswhite) {
    vector<move_t> val;
    val.reserve(32);
    for (ChessPiece* pc: ChessBoard::pieces[iswhite]) {
        if (!pc->onboard) {continue;}
        vector<pos_t> moves = get_moves(*pc);
        for (const pos_t& pos: moves) {
            val.push_back({pc, pos});
        }
    }
    return val;
}