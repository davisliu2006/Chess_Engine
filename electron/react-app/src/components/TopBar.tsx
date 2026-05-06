import "../css/App.css";
import Piece from "./Piece";

export default function TopBar() {
    return (
        <div className="TopBar">
            <Piece type="p" iswhite={true} onboard={false}></Piece>
            <Piece type="k" iswhite={false} onboard={false}></Piece>
            <Piece type="b" iswhite={false} onboard={false}></Piece>
            <Piece type="r" iswhite={false} onboard={false}></Piece>
            <Piece type="Q" iswhite={false} onboard={false}></Piece>
            <Piece type="K" iswhite={false} onboard={false}></Piece>
        </div>
    );
}