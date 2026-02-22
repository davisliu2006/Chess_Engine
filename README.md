# Chess_Engine

This project is an attempt at creating a chess AI algorithm from scratch in C++.

It uses a minimax algorithm to compute optimal chess moves by maximizing an evaluation function up to a certain depth over possible game states. The engine also supports multithreading to evaluate moves concurrently and speed up execution time in multi-core processors.

## Headers

```cpp
#include "chess/chess.hpp" // include the main library
```

## Compile Linking

```
<clang++/g++> <files-to-compile.cpp> chess/*.cpp -std=c++17 -O2 -o <output-path>
```

## Concurrency

Use `ChessBoard::get_best_move_concurrent` instead of `ChessBoard::get_best_move` to enable concurrency.