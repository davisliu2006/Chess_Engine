#pragma once

#include "chess_base.hpp"

using move_t = pair<int,int>;

inline vector<move_t> ChessBoard::getmoves(ChessPiece* piece) {
    int& x = piece->x;
    int& y = piece->y;
    bool& iswhite = piece->iswhite;

    vector<move_t> moves;

    if (piece->type == pawn) { //PAWN
        if (y != 0 && y != 7 && !grid[x][y-1+2*iswhite]) { //single step
            moves.push_back({x, y-1+2*iswhite});
            if (y == 6-5*iswhite && !grid[x][y-2+4*iswhite]) { //double step
                moves.push_back({x, y-2+4*iswhite});
            }
        }
        if (x >= 1) { //left capture
            ChessPiece* captpiece = grid[x-1][y-1+2*iswhite];
            if (captpiece && captpiece->iswhite != iswhite) {
                moves.push_back({x-1, y-1+2*iswhite});
            }
        }
        if (x <= 6) { //right caputre
            ChessPiece* captpiece = grid[x+1][y-1+2*iswhite];
            if (captpiece && captpiece->iswhite != iswhite) {
                moves.push_back({x+1, y-1+2*iswhite});
            }
        }
    }
        
    else if (piece->type == knight) { //KNIGHT
        for (int i = -1; i <= 1; i += 2) { //1 hori, 2 vtc
            for (int j = -2; j <= 2; j += 4) {
                if (x+i >= 0 && x+i <= 7 && y+j >= 0 and y+j <= 7) {
                    ChessPiece* captpiece = grid[x+i][y+j];
                    if (!captpiece || captpiece->iswhite != iswhite) {
                        moves.push_back({x+i, y+j});
                    }
                }
            }
        }
        for (int i = -2; i <= 2; i += 4) { //2 hori, 1 vtc
            for (int j = -1; j <= 1; j += 2) {
                if (x+i >= 0 && x+i <= 7 && y+j >= 0 and y+j <= 7) {
                    ChessPiece* captpiece = grid[x+i][y+j];
                    if (!captpiece || captpiece->iswhite != iswhite) {
                        moves.push_back({x+i, y+j});
                    }
                }
            }
        }
    }
        
    else if (piece->type == bishop or piece->type == queen) { //BISHOP or QUEEN
        for (int i = 1; i <= 7; i++) { //down, left
            if (x-i >= 0 and y-i >= 0) {
                ChessPiece* captpiece = grid[x-i][y-i];
                if (!captpiece || captpiece->iswhite != iswhite) {
                    moves.push_back({x-i, y-i});
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        for (int i = 1; i <= 7; i++) { //up, left
            if (x-i >= 0 and y+i <= 7) {
                ChessPiece* captpiece = grid[x-i][y+i];
                if (!captpiece || captpiece->iswhite != iswhite) {
                    moves.push_back({x-i, y+i});
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        for (int i = 1; i <= 7; i++) { //down, right
            if (x+i <= 7 and y-i >= 0) {
                ChessPiece* captpiece = grid[x+i][y-i];
                if (!captpiece || captpiece->iswhite != iswhite) {
                    moves.push_back({x+i, y-i});
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        for (int i = 1; i <= 7; i++) { //up, right
            if (x+i <= 7 and y+i <= 7) {
                ChessPiece* captpiece = grid[x+i][y+i];
                if (!captpiece || captpiece->iswhite != iswhite) {
                    moves.push_back({x+i, y+i});
                } else {
                    break;
                }
            } else {
                break;
            }
        }
    }
        
    else if (piece->type == rook or piece->type == queen) { //ROOK or QUEEN
        for (int i = 1; i <= 7; i++) { //left
            if (x-i >= 0) {
                ChessPiece* captpiece = grid[x-i][y];
                if (!captpiece || captpiece->iswhite != iswhite) {
                    moves.push_back({x-i, y});
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        for (int i = 1; i <= 7; i++) { //right
            if (x+i <= 7) {
                ChessPiece* captpiece = grid[x+i][y];
                if (!captpiece || captpiece->iswhite != iswhite) {
                    moves.push_back({x+i,y});
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        for (int i = 1; i <= 7; i++) { //down
            if (y-i >= 0) {
                ChessPiece* captpiece = grid[x][y-i];
                if (!captpiece || captpiece->iswhite != iswhite) {
                    moves.push_back({x, y-i});
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        for (int i = 1; i <= 7; i++) { //up
            if (y+i <= 7) {
                ChessPiece* captpiece = grid[x][y+i];
                if (!captpiece || captpiece->iswhite != iswhite) {
                    moves.push_back({x, y+i});
                } else {
                    break;
                }
            } else {
                break;
            }
        }
    }
        
    else if (piece->type == king) { //KING
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if ((i == 0 && j == 0) || x+i < 0 || x+i > 7 || y+j < 0 || y+j > 7) {continue;}
                ChessPiece* captpiece = grid[x+i][y+j];
                if (!captpiece || captpiece->iswhite != iswhite) {
                    moves.push_back({x+i,y+j});
                }
            }
        }
    }

    return moves;
}

inline bool ChessBoard::ischeck(bool iswhite) {
    for (ChessPiece* piece: pieces[!iswhite]) {
        vector<move_t> moves = getmoves(piece);
        for (move_t& move: moves) {
            if (move.first == kings[iswhite]->x and move.second == kings[iswhite]->y) {
                return true;
            }
        }
    }
    return false;
}

inline void ChessBoard::printmoves(ChessPiece* piece) {
    vector<move_t> moves = getmoves(piece);
    for (move_t& move: moves) {
        cout << move.first << " " << move.second << '\n';
    }
}
inline void ChessBoard::print_all_moves(bool iswhite) {
    for (ChessPiece* pc: ChessBoard::pieces[iswhite]) {
        cout << pc->x << pc->y << '\n';
        ChessBoard::printmoves(pc);
    }
}

inline vector<move_pair_t> ChessBoard::get_all_moves(bool iswhite) {
    vector<move_pair_t> val;
    for (ChessPiece* pc: ChessBoard::pieces[iswhite]) {
        vector<move_t> moves = getmoves(pc);
        for (move_t& move: moves) {
            val.push_back({pc, move});
        }
    }
    return val;
}