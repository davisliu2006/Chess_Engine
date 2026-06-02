import {Piece, PieceEnum} from "./base";
import HistoryStack, { HistoryItem } from "./history_stack";

export class GameState {
    pieces: Array<Piece> = [];
    whiteTurn: boolean = true;
    history: HistoryStack = new HistoryStack();

    reset() {
        this.pieces = [];
        this.whiteTurn = true;
        this.history = new HistoryStack();
        this.defaultSetup();
    }

    defaultSetup() {
        for (let i = 0; i < 8; i++) {
            this.pieces.push(new Piece("p", true, i, 1));
            this.pieces.push(new Piece("p", false, i, 6));
        }
        this.pieces.push(new Piece("r", true, 0, 0));
        this.pieces.push(new Piece("k", true, 1, 0));
        this.pieces.push(new Piece("b", true, 2, 0));
        this.pieces.push(new Piece("Q", true, 3, 0));
        this.pieces.push(new Piece("K", true, 4, 0));
        this.pieces.push(new Piece("b", true, 5, 0));
        this.pieces.push(new Piece("k", true, 6, 0));
        this.pieces.push(new Piece("r", true, 7, 0));
        this.pieces.push(new Piece("r", false, 0, 7));
        this.pieces.push(new Piece("k", false, 1, 7));
        this.pieces.push(new Piece("b", false, 2, 7));
        this.pieces.push(new Piece("Q", false, 3, 7));
        this.pieces.push(new Piece("K", false, 4, 7));
        this.pieces.push(new Piece("b", false, 5, 7));
        this.pieces.push(new Piece("k", false, 6, 7));
        this.pieces.push(new Piece("r", false, 7, 7));
        this.history.push(new HistoryItem(this.pieces));
    }

    movePiece(index: number, x: number, y: number) {
        if (index < 0 || index >= this.pieces.length) {
            throw new Error("Invalid piece index");
        }
        this.captPiece(x, y);
        this.pieces[index].x = x;
        this.pieces[index].y = y;
    }

    captPiece(x: number, y: number) {
        for (const piece of this.pieces) {
            if (piece.onBoard && piece.x == x && piece.y == y) {
                piece.onBoard = false;
            }
        }
    }

    endTurn() {
        this.whiteTurn = !this.whiteTurn;
        this.history.push(new HistoryItem(this.pieces));
    }

    undo() {
        this.history.undo();
        let cur = this.history.current();
        this.pieces = structuredClone(cur.array);
        this.whiteTurn = !this.whiteTurn;
    }

    redo() {
        this.history.redo();
        let cur = this.history.current();
        this.pieces = structuredClone(cur.array);
        this.whiteTurn = !this.whiteTurn;
    }
}

export {Piece, HistoryStack};
export type {PieceEnum};