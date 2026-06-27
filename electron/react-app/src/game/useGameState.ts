import {useCallback, useRef, useState} from "react";
import {GameState, Piece} from "./game";
import HistoryStack, {HistoryItem} from "./history_stack";

export interface GameStateHook {
    pieces: Piece[];
    whiteTurn: boolean;
    canUndo: boolean;
    canRedo: boolean;
    reset: () => void;
    movePiece: (index: number, x: number, y: number) => void;
    undo: () => void;
    redo: () => void;
    setPieces: (p: Piece[]) => void;
    selectedPiece: number | null;
    setSelectedPiece: (index: number | null) => void;
};

export default function useGameState(): GameStateHook {
    let gsRef = useRef<GameState>(new GameState());
    let [pieces, setPiecesState] = useState<Piece[]>(gsRef.current.pieces);
    let [whiteTurn, setWhiteTurn] = useState<boolean>(gsRef.current.whiteTurn);
    let [canUndo, setCanUndo] = useState<boolean>(gsRef.current.canUndo());
    let [canRedo, setCanRedo] = useState<boolean>(gsRef.current.canRedo());
    let [selectedPiece, setSelectedPiece] = useState<number | null>(null);

    let syncFromGameState = useCallback(() => {
        setPiecesState([...gsRef.current.pieces]);
        setWhiteTurn(gsRef.current.whiteTurn);
        setCanUndo(gsRef.current.canUndo());
        setCanRedo(gsRef.current.canRedo());
    }, []);

    let reset = useCallback(() => {
        gsRef.current.reset();
        syncFromGameState();
    }, [syncFromGameState]);

    let movePiece = useCallback((index: number, x: number, y: number) => {
        gsRef.current.movePiece(index, x, y);
        gsRef.current.endTurn();
        syncFromGameState();
        setSelectedPiece(null);
    }, [syncFromGameState]);

    let undo = useCallback(() => {
        try {
            gsRef.current.undo();
            syncFromGameState();
            setSelectedPiece(null);
        } catch (e) {
            console.warn("Undo failed:", e);
        }
    }, [syncFromGameState]);

    let redo = useCallback(() => {
        try {
            gsRef.current.redo();
            syncFromGameState();
            setSelectedPiece(null);
        } catch (e) {
            console.warn("Redo failed:", e);
        }
    }, [syncFromGameState]);

    let setPieces = useCallback((p: Piece[]) => {
        let gs = gsRef.current;
        gs.pieces = structuredClone(p);
        gs.history.push(new HistoryItem(gs.pieces));
        syncFromGameState();
    }, [syncFromGameState]);

    return {
        pieces, whiteTurn,
        canUndo, canRedo,
        reset, movePiece,
        undo, redo,
        setPieces,
        selectedPiece, setSelectedPiece
    };
}