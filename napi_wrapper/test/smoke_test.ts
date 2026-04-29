import { Board, Move } from "..";

const boardGrid: string[][] = [
    ["r1", "k1", "b1", "Q1", "K1", "b1", "k1", "r1"],
    ["p1", "p1", "p1", "p1", "p1", "p1", "p1", "p1"],
    ["--", "--", "--", "--", "--", "--", "--", "--"],
    ["--", "--", "--", "--", "--", "--", "--", "--"],
    ["--", "--", "--", "--", "--", "--", "--", "--"],
    ["--", "--", "--", "--", "--", "--", "--", "--"],
    ["p0", "p0", "p0", "p0", "p0", "p0", "p0", "p0"],
    ["r0", "k0", "b0", "Q0", "K0", "b0", "k0", "r0"],
];

const iswhite = true;
const depth = 3;

const board = new Board(boardGrid);
const moves: Move[] = board.get_valid_moves(iswhite);
const scores: number[] = board.get_scores_recursive(moves, depth);

console.log(`Found ${moves.length} legal moves for ${iswhite? "white" : "black"} at depth ${depth}:`);
for (let i = 0; i < moves.length; i++) {
    const [[x0, y0], [x1, y1]] = moves[i];
    const score = scores[i];
    console.log(`[${x0}, ${y0}] -> [${x1}, ${y1}] score = ${score}`);
}
