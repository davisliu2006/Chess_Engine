import "../css/App.css";
import "../css/Board.css";
import "../css/Piece.css";
import Piece from "./Piece"
import {StateHook} from "../include/react_hook";
import * as game from "../game/game";
import {GameStateHook} from "../game/useGameState";

export interface BoardTileProps {
    x: number;
    y: number;
    gameStateHook: GameStateHook;
    onClick?: () => void;
    hooks: {
        flipBoard: StateHook<boolean>;
    };
}
export function BoardTile(props: BoardTileProps) {
    let selectedPiece = props.gameStateHook.selectedPiece;
    let selected = false;
    if (selectedPiece) {
        let piece = props.gameStateHook.pieces[selectedPiece];
        selected = (piece.x == props.x && piece.y == props.y);
    }

    return (
        <div className="BoardTile" style={{
            left: (props.hooks.flipBoard.val? 12.5*(7-props.x)+"%" : 12.5*props.x+"%"),
            bottom: (props.hooks.flipBoard.val? 12.5*(7-props.y)+"%" : 12.5*props.y+"%"),
            backgroundColor: selected
                ? "rgb(180, 210, 255)"
                : ((props.x+props.y)%2 == 0? "rgb(200, 200, 200)" : "rgb(250, 250, 250)"),
            cursor: "pointer"
        }} onClick={props.onClick}>

        </div>
    );
}

export interface BoardLabelProps {
    axis: "x" | "y";
    index: number;
    hooks: {
        flipBoard: StateHook<boolean>;
    };
}
export function BoardLabel(props: BoardLabelProps) {
    if (props.axis == "x") {
        return (
            <div className="BoardLabel" style={{
                left: (props.hooks.flipBoard.val? 12.5*(7-props.index)+"%" : 12.5*props.index+"%"),
                bottom: "-8%",
                width: "12.5%"
            }}>
                <label>{props.index}</label>
            </div>
        )
    } else if (props.axis == "y") {
        return (
            <div className="BoardLabel" style={{
                left: "-8%",
                bottom: (props.hooks.flipBoard.val? 12.5*(7-props.index)+"%" : 12.5*props.index+"%"),
                height: "12.5%"
            }}>
                <label>{props.index}</label>
            </div>
        )
    }
    return null;
}

export interface BoardProps {
    gameStateHook: GameStateHook;
    hooks: {
        flipBoard: StateHook<boolean>;
    };
}
export default function Board(props: BoardProps) {
    let tiles = [];
    for (let i = 0; i < 8; i++) {
        for (let j = 0; j < 8; j++) {
            tiles.push(
                <BoardTile
                    key={`${i}${j}`} x={i} y={j}
                    gameStateHook={props.gameStateHook}
                    hooks={props.hooks}
                    onClick={() => {
                        let pieceIndex = props.gameStateHook.pieces.findIndex(
                            (piece) => piece.x == i && piece.y == j && piece.onBoard
                        );
                        props.gameStateHook.setSelectedPiece(
                            pieceIndex == -1 ? null : pieceIndex
                        );
                    }}
                ></BoardTile>
            )
        }
    }
    let labels = [];
    for (let i = 0; i < 8; i++) {
        labels.push(<BoardLabel axis="x" index={i} hooks={props.hooks}></BoardLabel>)
        labels.push(<BoardLabel axis="y" index={i} hooks={props.hooks}></BoardLabel>)
    }
    let pieces = [];
    for (let i = 0; i < props.gameStateHook.pieces.length; i++) {
        let piece = props.gameStateHook.pieces[i];
        pieces.push(
            <Piece
                key={i}
                type={piece.type} iswhite={piece.color} onboard={true} x={piece.x} y={piece.y}
                hooks={props.hooks}
            ></Piece>
        );
    }
    return (
        <div className="Board" style={{}}>
            {tiles}
            {labels}
            {pieces}
        </div>
    );
}