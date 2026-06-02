import {useCallback, useRef, useState} from "react";
import {GameState, Piece} from "./game";
import HistoryStack, {HistoryItem} from "./history_stack";

export interface GameStateHook {
    pieces: Piece[];
    whiteTurn: boolean;
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
    let [selectedPiece, setSelectedPiece] = useState<number | null>(null);

    let reset = useCallback(() => {
        gsRef.current.reset();
        setPiecesState([...gsRef.current.pieces]);
        setWhiteTurn(gsRef.current.whiteTurn);
    }, []);

    let movePiece = useCallback((index: number, x: number, y: number) => {
        gsRef.current.movePiece(index, x, y);
        gsRef.current.endTurn();
        setPiecesState([...gsRef.current.pieces]);
        setWhiteTurn(gsRef.current.whiteTurn);
        setSelectedPiece(null);
    }, []);

    let undo = useCallback(() => {
        try {
            gsRef.current.undo();
            setPiecesState([...gsRef.current.pieces]);
            setWhiteTurn(gsRef.current.whiteTurn);
        } catch (e) {
            console.warn("Undo failed:", e);
        }
    }, []);

    let redo = useCallback(() => {
        try {
            gsRef.current.redo();
            setPiecesState([...gsRef.current.pieces]);
            setWhiteTurn(gsRef.current.whiteTurn);
        } catch (e) {
            console.warn("Redo failed:", e);
        }
    }, []);

    let setPieces = useCallback((p: Piece[]) => {
        let gs = gsRef.current;
        gs.pieces = structuredClone(p);
        // record snapshot
        gs.history.push(new HistoryItem(gs.pieces));
        setPiecesState([...gs.pieces]);
    }, []);

    return {
        pieces, whiteTurn,
        reset, movePiece,
        undo, redo,
        setPieces,
        selectedPiece, setSelectedPiece
    };
}