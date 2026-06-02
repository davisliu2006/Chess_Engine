import {Piece} from "./base";
import {encodeBoard} from "./board_encoding";

export type EngineCoord = [number, number];
export type EngineMove = [EngineCoord, EngineCoord];

export function isEngineAvailable(): boolean {
    return typeof window.chessEngine !== "undefined";
}

export async function getValidMoves(
    pieces: Piece[],
    iswhite: boolean
): Promise<EngineMove[]> {
    if (!window.chessEngine) {
        return [];
    }
    return window.chessEngine.getValidMoves(encodeBoard(pieces), iswhite);
}

export function destinationsForPiece(
    moves: EngineMove[],
    x: number,
    y: number
): EngineCoord[] {
    return moves
        .filter(([[fromX, fromY]]) => fromX == x && fromY == y)
        .map(([, [toX, toY]]) => [toX, toY]);
}

export async function getScoresRecursive(
    pieces: Piece[],
    moves: EngineMove[],
    depth: number
): Promise<number[]> {
    if (!window.chessEngine) {
        return [];
    }
    return window.chessEngine.getScoresRecursive(
        encodeBoard(pieces),
        moves,
        depth
    );
}