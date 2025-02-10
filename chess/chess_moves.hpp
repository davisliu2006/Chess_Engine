#pragma once

#include "chess_base.hpp"

// get all moves for a piece
inline vector<move_t> ChessBoard::get_moves(const ChessPiece& piece) {
    const int& x = piece.x;
    const int& y = piece.y;
    const bool& iswhite = piece.iswhite;
    int x1, y1;

    vector<move_t> moves; // all valid moves

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
        for (int i = -1; i <= 1; i += 2) { // 1 hori, 2 vtc
            for (int j = -2; j <= 2; j += 4) {
                x1 = x+i; y1 = y+j;
                if (0 <= x1 && x1 <= 7 && 0 <= y1 && y1 <= 7) { // in range
                    ChessPiece* captpiece = grid[x1][y1];
                    if (!captpiece || captpiece->iswhite != iswhite) { // empty or capture
                        moves.push_back({x1, y1});
                    }
                }
            }
        }
        for (int i = -2; i <= 2; i += 4) { // 2 hori, 1 vtc
            for (int j = -1; j <= 1; j += 2) {
                x1 = x+i; y1 = y+j;
                if (0 <= x1 && x1 <= 7 && 0 <= y1 && y1 <= 7) { // in range
                    ChessPiece* captpiece = grid[x1][y1];
                    if (!captpiece || captpiece->iswhite != iswhite) { // empty or capture
                        moves.push_back({x1, y1});
                    }
                }
            }
        }
    }
        
    if (piece.type == bishop or piece.type == queen) { // BISHOP or QUEEN
        for (int i = 1; i <= 7; i++) { // down, left
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
        for (int i = 1; i <= 7; i++) { // up, left
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
        for (int i = 1; i <= 7; i++) { // down, right
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
        for (int i = 1; i <= 7; i++) { // up, right
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
        for (int i = 1; i <= 7; i++) { // left
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
        for (int i = 1; i <= 7; i++) { // right
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
        for (int i = 1; i <= 7; i++) { // down
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
        for (int i = 1; i <= 7; i++) { // up
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
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                x1 = x+i; y1 = y+j;
                if ((i == 0 && j == 0) || x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7) {continue;}
                ChessPiece* captpiece = grid[x1][y1];
                if (!captpiece || captpiece->iswhite != iswhite) { // empty or capture
                    moves.push_back({x1, y1});
                }
            }
        }
    }

    return moves;
}

// check if side is "in check"
inline bool ChessBoard::is_check(bool iswhite) {
    ChessPiece* king = kings[iswhite];
    assert(king && "Board does not have king.");
    for (ChessPiece* piece: pieces[!iswhite]) { // for each opposing piece
        vector<move_t> moves = get_moves(*piece);
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
inline void ChessBoard::print_moves(const ChessPiece& piece) {
    vector<move_t> moves = get_moves(piece);
    for (const auto& [mx, my]: moves) {
        cout << mx << my << ' ';
    }
    cout << '\n';
}
// print moves for all pieces on a side
inline void ChessBoard::print_all_moves(bool iswhite) {
    for (ChessPiece* pc: ChessBoard::pieces[iswhite]) {
        cout << pc->type << pc->iswhite << '@' << pc->x << pc->y << ": ";
        ChessBoard::print_moves(*pc);
    }
}

// get all moves for a side
inline vector<move_pair_t> ChessBoard::get_all_moves(bool iswhite) {
    vector<move_pair_t> val;
    for (ChessPiece* pc: ChessBoard::pieces[iswhite]) {
        vector<move_t> moves = get_moves(*pc);
        for (const move_t& move: moves) {
            val.push_back({pc, move});
        }
    }
    return val;
}