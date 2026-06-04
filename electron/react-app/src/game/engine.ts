import {Piece} from "./base";
import {encodeBoard} from "./board_encoding";

export type EngineCoord = [number, number];
export type EngineMove = [EngineCoord, EngineCoord];

export function isEngineAvailable(): boolean {
    return typeof window.chessEngine != "undefined";
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

const SUGGESTION_DEPTH = 4;

export async function getSuggestedMoves(
    pieces: Piece[],
    iswhite: boolean,
    depth: number = SUGGESTION_DEPTH
): Promise<EngineMove[]> {
    function fromSquareKey(x: number, y: number): number {
        return x*8+y;
    }

    const moves = await getValidMoves(pieces, iswhite);
    if (moves.length == 0) {return [];}

    const scores = await getScoresRecursive(pieces, moves, depth);
    // this is a temporary solution
    const bestPerPiece = new Map<number,
        {score: number; move: EngineMove}
    >();

    for (let i = 0; i < moves.length; i++) {
        const move = moves[i];
        const [[fromX, fromY]] = move;
        const key = fromSquareKey(fromX, fromY);
        const score = scores[i];
        const prev = bestPerPiece.get(key);
        if (prev == undefined || score > prev.score) {
            bestPerPiece.set(key, {score, move});
        }
    }

    return [...bestPerPiece.values()].map((entry) => entry.move);
}