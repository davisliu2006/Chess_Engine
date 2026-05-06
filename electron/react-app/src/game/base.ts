export type PieceEnum = "p" | "k" | "b" | "r" | "Q" | "K";

export class Piece {
    onBoard: boolean = true;
    type: PieceEnum;
    color: boolean;
    x: number;
    y: number;

    constructor(type: PieceEnum, color: boolean, x: number, y: number) {
        this.type = type;
        this.color = color;
        this.x = x;
        this.y = y;
    }
}