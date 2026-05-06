import "../css/App.css";
import "../css/Piece.css";
import { StateHook } from "../include/react_hook";
import * as game from "../game/game";

const pieceMap: Record<game.PieceEnum, string> = {
    "p": "fa-chess-pawn",
    "k": "fa-chess-knight",
    "b": "fa-chess-bishop",
    "r": "fa-chess-rook",
    "Q": "fa-chess-queen",
    "K": "fa-chess-king",
}

export interface PieceProps {
    type: game.PieceEnum;
    iswhite: boolean;
    onboard: boolean;
    x?: number;
    y?: number;
    hooks?: {
        flipBoard: StateHook<boolean>;
    };
}
export default function Piece(props: PieceProps) {
    let style: React.CSSProperties = {
        color: (props.iswhite? "grey" : "black")
    }
    if (props.onboard) {
        style.position = "absolute";
        style.left = (props.hooks!.flipBoard.val? 12.5*(7-props.x!)+"%" : 12.5*props.x!+"%");
        style.bottom = (props.hooks!.flipBoard.val? 12.5*(7-props.y!)+"%" : 12.5*props.y!+"%");
        style.width = "12.5%"; style.height = "12.5%";
        style.margin = "0px";
        style.pointerEvents = "none";
    }
    return (
        <div className="Piece" style={style}>
            <i className={"fas "+pieceMap[props.type]}></i>
        </div>
    )
}