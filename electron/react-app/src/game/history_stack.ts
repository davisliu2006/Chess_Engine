import {Piece} from "./base";

export class HistoryItem {
    array: Array<Piece> = [];
    constructor (array: Array<Piece>) {
        this.array = structuredClone(array);
    }
}

export default class HistoryStack<T = HistoryItem> {
    private items: Array<T> = [];
    private index: number = -1;

    size(): number {return this.items.length;}
    move_n(): number {return this.index+1;}
    
    push(item: T) {
        if (this.index < this.items.length-1) {
            this.items = this.items.slice(0, this.index+1);
        }
        this.items.push(item);
        this.index++;
    }
    undo() {
        if (this.index > 0) {
            this.index--;
        } else {
            throw new Error("Cannot undo past beginning of history");
        }
    }
    redo() {
        if (this.index < this.items.length-1) {
            this.index++;
        } else {
            throw new Error("Cannot redo past end of history");
        }
    }
    current(): T {
        if (this.index >= 0 && this.index < this.items.length) {
            return this.items[this.index];
        } else {
            throw new Error("No current item in history");
        }
    }
    canUndo(): boolean {
        return this.index > 0;
    }
    canRedo(): boolean {
        return this.index < this.items.length - 1;
    }
}