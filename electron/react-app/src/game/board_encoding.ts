import {Piece} from "./base";

export function encodeBoard(pieces: Piece[]): string[][] {
    const grid = Array.from({length: 8}, () => Array(8).fill("-"));
    for (const piece of pieces) {
        if (!piece.onBoard) {
            continue;
        }
        grid[piece.y][piece.x] = `${piece.type}${piece.color ? "1" : "0"}`;
    }
    return grid;
}