# Chess_Engine

Attempt at creating a chess AI algorithm.

## Headers

```cpp
#include "chess/chess.hpp"
```

## Compile Linking

```
<clang++/g++> <files-to-compile.cpp> chess/*.cpp -std=c++17 -O2 -o <output-path>
```

## Concurrency

Use `ChessBoard::get_best_move_concurrent` instead of `ChessBoard::get_best_move` to enable concurrency.