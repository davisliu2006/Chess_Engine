# Chess NAPI Wrapper

## Exposed Functions

```ts
type Coord = [number, number];
type Move = [Coord, Coord]

class Board {
    constructor(
        board: string[][]
    );

    get_valid_moves(
        iswhite: boolean
    ): Array<Move>;

    get_scores_recursive(
        moves: Array<Move>, r: number
    ): Array<number>;
}
```

## Board Encoding

- Coordinates use `[x, y]` with `0 <= x, y <= 7`
    - `[0, 0]` is the white rook on the left in the default setup
    - `[7, 0]` is the white rook on the right in the default setup
- Piece cell strings use `<piece><color>`:
    - pieces: `p` pawn, `k` knight, `b` bishop, `r` rook, `Q` queen, `K` king
    - color: `0` black, `1` white
- Empty cells can be represented as `"-"` (also accepted: `"--"` and `""`)