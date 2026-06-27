import {useEffect, useMemo, useState} from "react";
import "../css/App.css";
import "../css/Board.css";
import "../css/Piece.css";
import Piece from "./Piece";
import SuggestionLines from "./SuggestionLines";
import {GameConfigHooks} from "../game/config";
import {
    destinationsForPiece,
    EngineCoord,
    getBestMove,
    getSuggestedMoves,
    getValidMoves,
    isEngineAvailable,
    pieceIndexForMove,
    ScoredMove,
} from "../game/engine";
import {GameSettings, isComputerTurn} from "../game/settings";
import {GameStateHook} from "../game/useGameState";

function coordHash(x: number, y: number): number {
    return x*8+y;
}

export interface BoardTileProps {
    x: number;
    y: number;
    gameStateHook: GameStateHook;
    isLegalMove: boolean;
    onClick?: () => void;
    hooks: GameConfigHooks;
}
export function BoardTile(props: BoardTileProps) {
    let selectedPiece = props.gameStateHook.selectedPiece;
    let selected = false;
    if (selectedPiece != null) {
        let piece = props.gameStateHook.pieces[selectedPiece];
        selected = (piece.x == props.x && piece.y == props.y);
    }

    return (
        <div
            className={(props.isLegalMove? "BoardTile legal" : "BoardTile")}
            style={{
                left: (props.hooks.flipBoard.val? `${12.5*(7-props.x)}%` : `${12.5*props.x}%`),
                bottom: (props.hooks.flipBoard.val? `${12.5*(7-props.y)}%` : `${12.5*props.y}%`),
                backgroundColor: (selected?
                    "rgb(180, 210, 255)"
                    : ((props.x+props.y)%2 == 0?
                        "rgb(200, 200, 200)"
                        : "rgb(250, 250, 250)"
                    )
                ),
                cursor: "pointer",
            }}
            onClick={props.onClick}
        ></div>
    );
}

export interface BoardLabelProps {
    axis: "x" | "y";
    index: number;
    hooks: GameConfigHooks;
}
export function BoardLabel(props: BoardLabelProps) {
    if (props.axis == "x") {
        return (
            <div className="BoardLabel" style={{
                left: (props.hooks.flipBoard.val? `${12.5*(7-props.index)}%` : `${12.5*props.index}%`),
                bottom: "-8%",
                width: "12.5%"
            }}>
                <label>{props.index}</label>
            </div>
        );
    } else if (props.axis == "y") {
        return (
            <div className="BoardLabel" style={{
                left: "-8%",
                bottom: (props.hooks.flipBoard.val? `${12.5*(7-props.index)}%` : `${12.5*props.index}%`),
                height: "12.5%"
            }}>
                <label>{props.index}</label>
            </div>
        );
    }
    return null;
}

export interface BoardProps {
    gameSettings: GameSettings;
    gameStateHook: GameStateHook;
    hooks: GameConfigHooks;
}
export default function Board(props: BoardProps) {
    const {gameStateHook, gameSettings, hooks} = props;
    const [legalTargets, setLegalTargets] = useState<EngineCoord[]>([]);
    const [suggestedMoves, setSuggestedMoves] = useState<ScoredMove[]>([]);

    const humanTurn = !isComputerTurn(gameSettings, gameStateHook.whiteTurn);
    const showSuggestions = hooks.showSuggestions.val;

    const legalTargetHashes = useMemo(
        () => new Set(legalTargets.map(([x, y]) => coordHash(x, y))),
        [legalTargets]
    );

    // update legal targets when selected piece changes
    useEffect(() => {
        const selected = gameStateHook.selectedPiece;
        if (selected == null || !humanTurn) { // not human turn
            setLegalTargets([]); return;
        }
        const piece = gameStateHook.pieces[selected];
        if (piece.color != gameStateHook.whiteTurn) { // selected invalid peice
            setLegalTargets([]); return;
        }
        let cancelled = false; // async operation cancelled
        (async () => {
            const moves = await getValidMoves(gameStateHook.pieces, gameStateHook.whiteTurn);
            if (cancelled) {return;}
            setLegalTargets(
                destinationsForPiece(moves, piece.x, piece.y)
            );
        })();
        return () => { // async operation cancelled
            cancelled = true;
        };
    }, [
        gameStateHook.selectedPiece,
        gameStateHook.pieces,
        gameStateHook.whiteTurn,
        humanTurn
    ]);

    // update suggested moves when state changes
    useEffect(() => {
        if (!showSuggestions || !humanTurn) {
            setSuggestedMoves([]); return;
        }
        let cancelled = false; // async operation cancelled
        (async () => {
            const moves = await getSuggestedMoves(
                gameStateHook.pieces,
                gameStateHook.whiteTurn
            );
            if (!cancelled) {
                setSuggestedMoves(moves);
            }
        })();
        return () => { // async operation cancelled
            cancelled = true;
        };
    }, [
        showSuggestions,
        gameStateHook.pieces,
        gameStateHook.whiteTurn,
        humanTurn
    ]);

    // make a move when it is the computer's turn
    useEffect(() => {
        if (!isComputerTurn(gameSettings, gameStateHook.whiteTurn)) {return;}
        if (hooks.pauseComputer.val) {return;}
        if (!isEngineAvailable()) {
            console.warn("Computer turn skipped: Chess engine not available.");
            return;
        }

        let cancelled = false;  // async operation cancelled
        const turnColor = gameStateHook.whiteTurn;
        (async () => {
            const move = await getBestMove(gameStateHook.pieces, turnColor);
            if (cancelled || move == null) {return;}
            // need to re-check if it's the right turn after async operation
            if (!isComputerTurn(gameSettings, gameStateHook.whiteTurn)
            || gameStateHook.whiteTurn != turnColor
            || hooks.pauseComputer.val) {return;}

            const pieceIndex = pieceIndexForMove(gameStateHook.pieces, move);
            if (pieceIndex == -1) {return;}

            const [, [toX, toY]] = move;
            gameStateHook.movePiece(pieceIndex, toX, toY);
        })();

        return () => {
            cancelled = true;
        };
    }, [
        gameSettings,
        gameStateHook.pieces,
        gameStateHook.whiteTurn,
        gameStateHook.movePiece,
        hooks.pauseComputer.val,
    ]);

    function handleTileClick(x: number, y: number) {
        const key = coordHash(x, y);

        if (gameStateHook.selectedPiece != null
        && legalTargetHashes.has(key)) { // move piece
            gameStateHook.movePiece(gameStateHook.selectedPiece, x, y);
            setLegalTargets([]);
            setSuggestedMoves([]);
            return;
        }

        // find selected piece if exists
        const pieceIndex = gameStateHook.pieces.findIndex(
            (piece) => piece.onBoard && piece.x == x && piece.y == y
        );
        if (pieceIndex == -1) {
            gameStateHook.setSelectedPiece(null);
            setLegalTargets([]);
            return;
        }

        const piece = gameStateHook.pieces[pieceIndex];
        if (!humanTurn || piece.color != gameStateHook.whiteTurn) {
            gameStateHook.setSelectedPiece(null);
            setLegalTargets([]);
            return;
        }
        gameStateHook.setSelectedPiece(pieceIndex);
    }

    let tiles = [];
    for (let i = 0; i < 8; i++) {
        for (let j = 0; j < 8; j++) {
            tiles.push(
                <BoardTile
                    key={`${i}${j}`} x={i} y={j}
                    gameStateHook={gameStateHook}
                    hooks={props.hooks}
                    isLegalMove={legalTargetHashes.has(coordHash(i, j))}
                    onClick={() => handleTileClick(i, j)}
                ></BoardTile>
            );
        }
    }
    let labels = [];
    for (let i = 0; i < 8; i++) {
        labels.push(
            <BoardLabel key={`x${i}`} axis="x" index={i} hooks={props.hooks}></BoardLabel>
        );
        labels.push(
            <BoardLabel key={`y${i}`} axis="y" index={i} hooks={props.hooks}></BoardLabel>
        );
    }
    let pieces = [];
    for (let i = 0; i < gameStateHook.pieces.length; i++) {
        let piece = gameStateHook.pieces[i];
        if (!piece.onBoard) {continue;}
        pieces.push(
            <Piece
                key={i} onboard={true}
                type={piece.type} iswhite={piece.color}
                x={piece.x} y={piece.y}
                hooks={props.hooks}
            ></Piece>
        );
    }
    return (
        <div className="Board" style={{}}>
            {tiles}
            {showSuggestions && (
                <SuggestionLines
                    moves={suggestedMoves}
                    flipBoard={hooks.flipBoard.val}
                />
            )}
            {labels}
            {pieces}
        </div>
    );
}
