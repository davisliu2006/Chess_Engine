export type Coord = [number, number];
export type Move = [Coord, Coord];

export class Board {
    constructor(board: string[][]);
    get_valid_moves(iswhite: boolean): Move[];
    get_scores_recursive(moves: Move[], r: number): number[];
}