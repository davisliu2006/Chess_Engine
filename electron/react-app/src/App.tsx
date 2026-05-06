import "./css/App.css";
import "./assets/font-awesome/css/all.min.css";
import {useEffect, useRef, useState} from "react";
import SidePanel from "./components/SidePanel";
import TopBar from "./components/TopBar";
import Board from "./components/Board";
import {stateHook} from "./include/react_hook";
import * as game from "./game/game";
import useGameState from "./game/useGameState";

export default function App() {
    let config = {
        flipBoard: stateHook(useState(false))
    }

    let gameStateHook = useGameState();

    useEffect(() => {
        gameStateHook.reset();
    }, []);

    return (
        <div className="App">
            <SidePanel hooks={{
                flipBoard: config.flipBoard
            }}></SidePanel>
            <div className="game-body">
                <TopBar></TopBar>
                <div className="board-holder">
                    <Board gameStateHook={gameStateHook} hooks={{
                        flipBoard: config.flipBoard
                    }}></Board>
                </div>
            </div>
        </div>
    );
}