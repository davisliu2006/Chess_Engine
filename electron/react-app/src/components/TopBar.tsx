import "../css/App.css";
import {Piece as PieceModel, PieceEnum} from "../game/base";
import Piece from "./Piece";

const PIECE_SORT_ORDER: Record<PieceEnum, number> = {
    K: 0, Q: 1, r: 2, b: 3, k: 4, p: 5,
};

function sortCaptured(pieces: PieceModel[]): PieceModel[] {
    return [...pieces].sort(
        (a, b) => PIECE_SORT_ORDER[a.type] - PIECE_SORT_ORDER[b.type]
    );
}

export interface TopBarProps {
    pieces: PieceModel[];
}

export default function TopBar(props: TopBarProps) {
    const captured = props.pieces.filter((piece) => !piece.onBoard);
    const capturedByWhite = sortCaptured(
        captured.filter((piece) => !piece.color)
    );
    const capturedByBlack = sortCaptured(
        captured.filter((piece) => piece.color)
    );

    return (
        <div className="TopBar">
            <div className="TopBarCaptures TopBarCapturesByWhite">
                {capturedByWhite.map((piece, index) => (
                    <Piece
                        key={`white-capture-${index}`}
                        type={piece.type}
                        iswhite={piece.color}
                        onboard={false}
                    ></Piece>
                ))}
            </div>
            <div className="TopBarCaptures TopBarCapturesByBlack">
                {capturedByBlack.map((piece, index) => (
                    <Piece
                        key={`black-capture-${index}`}
                        type={piece.type}
                        iswhite={piece.color}
                        onboard={false}
                    ></Piece>
                ))}
            </div>
        </div>
    );
}
