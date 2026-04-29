#include <napi.h>
#include <vector>

#include "../chess/chess_board.hpp"
#include "../chess/chess_ai.hpp"
#include "../chess/chess_debug.hpp"

using std::string, std::vector;
using chess::ChessBoard, chess::ChessPiece;
using chess::piece_t, chess::pos_t, chess::move_t;

namespace {
    piece_t parse_piece_type(char c) {
        piece_t type = chess::CHAR_TO_PIECE[static_cast<unsigned char>(c)];
        if (!type) {
            throw std::invalid_argument("Invalid piece type character.");
        }
        return type;
    }

    bool parse_piece_color(char c) {
        if (c == '0') {
            return false;
        }
        if (c == '1') {
            return true;
        }
        throw std::invalid_argument("Invalid piece color character.");
    }

    Napi::Array pos_to_js(Napi::Env env, const pos_t& pos) {
        Napi::Array out = Napi::Array::New(env, 2);
        out[static_cast<uint32_t>(0)] = Napi::Number::New(env, pos.x);
        out[static_cast<uint32_t>(1)] = Napi::Number::New(env, pos.y);
        return out;
    }

    pos_t js_to_pos(const Napi::Env& env, const Napi::Value& value, const char* arg_name) {
        if (!value.IsArray()) {
            throw Napi::TypeError::New(env, string(arg_name) + " must be [x, y].");
        }
        Napi::Array arr = value.As<Napi::Array>();
        Napi::Value x_value = arr.Get(static_cast<uint32_t>(0));
        Napi::Value y_value = arr.Get(static_cast<uint32_t>(1));
        if (arr.Length() != 2 || !x_value.IsNumber() || !y_value.IsNumber()) {
            throw Napi::TypeError::New(env, string(arg_name) + " must be [x, y].");
        }
        int x = x_value.As<Napi::Number>().Int32Value();
        int y = y_value.As<Napi::Number>().Int32Value();
        if (x < 0 || x > 7 || y < 0 || y > 7) {
            throw Napi::RangeError::New(env, string(arg_name) + " coordinate out of range.");
        }
        return {x, y};
    }

    move_t js_to_move(const Napi::Env& env, ChessBoard& board, const Napi::Value& value) {
        if (!value.IsArray()) {
            throw Napi::TypeError::New(env, "move must be [[x0, y0], [x1, y1]].");
        }
        Napi::Array move_arr = value.As<Napi::Array>();
        if (move_arr.Length() != 2) {
            throw Napi::TypeError::New(env, "move must be [[x0, y0], [x1, y1]].");
        }
        pos_t from = js_to_pos(env, move_arr[0u], "move[0]");
        pos_t to = js_to_pos(env, move_arr[1u], "move[1]");

        ChessPiece* piece = board.grid[from.x][from.y];
        if (!piece) {
            throw Napi::Error::New(env, "move start square is empty.");
        }
        return {piece, to};
    }

    void load_board_from_js(const Napi::Env& env, ChessBoard& board, const Napi::Value& value) {
        if (!value.IsArray()) {
            throw Napi::TypeError::New(env, "board must be string[8][8].");
        }
        Napi::Array board_rows = value.As<Napi::Array>();
        if (board_rows.Length() != 8) {
            throw Napi::RangeError::New(env, "board must have 8 rows.");
        }

        board.clear();
        for (uint32_t y = 0; y < 8; y++) {
            Napi::Value row_val = board_rows[y];
            if (!row_val.IsArray()) {
                throw Napi::TypeError::New(env, "each board row must be an array.");
            }
            Napi::Array row = row_val.As<Napi::Array>();
            if (row.Length() != 8) {
                throw Napi::RangeError::New(env, "each board row must have 8 cells.");
            }
            for (uint32_t x = 0; x < 8; x++) {
                Napi::Value cell_val = row[x];
                if (!cell_val.IsString()) {
                    throw Napi::TypeError::New(env, "each board cell must be a string.");
                }
                string cell = cell_val.As<Napi::String>().Utf8Value();
                if (cell == "-" || cell == "--" || cell.empty()) {
                    continue;
                }
                if (cell.size() != 2) {
                    throw Napi::Error::New(env, "piece cell must be like p0, k1, K0, or '-'.");
                }
                piece_t type = parse_piece_type(cell[0]);
                bool iswhite = parse_piece_color(cell[1]);
                board.create_piece(iswhite, type, static_cast<int>(x), static_cast<int>(y));
                if (type == chess::king) {
                    if (board.kings[iswhite] != NULL) {
                        throw Napi::Error::New(env, "board has duplicate king for one side.");
                    }
                    board.kings[iswhite] = board.grid[x][y];
                }
            }
        }

        if (!board.kings[0] || !board.kings[1]) {
            throw Napi::Error::New(env, "board must contain exactly two kings.");
        }
    }
}

class BoardWrap : public Napi::ObjectWrap<BoardWrap> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports) {
        Napi::Function ctor = DefineClass(
            env,
            "Board",
            {
                InstanceMethod("get_valid_moves", &BoardWrap::GetValidMoves),
                InstanceMethod("get_scores_recursive", &BoardWrap::GetScoresRecursive),
            }
        );
        constructor() = Napi::Persistent(ctor);
        constructor().SuppressDestruct();
        exports.Set("Board", ctor);
        return exports;
    }

    BoardWrap(const Napi::CallbackInfo& info) : Napi::ObjectWrap<BoardWrap>(info) {
        Napi::Env env = info.Env();
        if (info.Length() != 1) {
            throw Napi::TypeError::New(env, "Board constructor expects board string[8][8].");
        }
        load_board_from_js(env, board_, info[0]);
    }

private:
    ChessBoard board_;

    static Napi::FunctionReference& constructor() {
        static Napi::FunctionReference ctor;
        return ctor;
    }

    Napi::Value GetValidMoves(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        if (info.Length() != 1 || !info[0].IsBoolean()) {
            throw Napi::TypeError::New(env, "get_valid_moves expects iswhite boolean.");
        }
        bool iswhite = info[0].As<Napi::Boolean>().Value();
        vector<move_t> all_moves = board_.get_all_moves(iswhite);

        Napi::Array out = Napi::Array::New(env);
        uint32_t idx = 0;
        for (const move_t& move : all_moves) {
            // get_all_moves includes pseudo-legal moves; filter self-check.
            if (board_.get_score_recursive(move, 1) == chess::chess_ai::INVALID_SCORE) {
                continue;
            }
            Napi::Array js_move = Napi::Array::New(env, 2);
            js_move[0u] = pos_to_js(env, {move.piece->x, move.piece->y});
            js_move[1u] = pos_to_js(env, move.pos);
            out[idx++] = js_move;
        }
        return out;
    }

    Napi::Value GetScoresRecursive(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        if (info.Length() != 2 || !info[0].IsArray() || !info[1].IsNumber()) {
            throw Napi::TypeError::New(env, "get_scores_recursive expects moves array and depth r.");
        }
        int r = info[1].As<Napi::Number>().Int32Value();
        if (r < 1) {
            throw Napi::RangeError::New(env, "r must be >= 1.");
        }

        Napi::Array moves_arr = info[0].As<Napi::Array>();
        Napi::Array out = Napi::Array::New(env, moves_arr.Length());
        for (uint32_t i = 0; i < moves_arr.Length(); i++) {
            move_t move = js_to_move(env, board_, moves_arr[i]);
            double score = board_.get_score_recursive(move, r);
            out[i] = Napi::Number::New(env, score);
        }
        return out;
    }
};

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    return BoardWrap::Init(env, exports);
}

NODE_API_MODULE(chess_engine_napi, InitAll)