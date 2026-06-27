import "../css/App.css";
import SidePanel from "./SidePanel";
import TopBar from "./TopBar";
import Board from "./Board";
import {GameConfigHooks} from "../game/config";
import {GameSettings} from "../game/settings";
import {GameStateHook} from "../game/useGameState";

export interface GameViewProps {
    gameSettings: GameSettings;
    gameStateHook: GameStateHook;
    hooks: GameConfigHooks;
    goToSettings: () => void;
}

export default function GameView(props: GameViewProps) {
    return (
        <div className="App">
            <SidePanel
                gameSettings={props.gameSettings}
                gameStateHook={props.gameStateHook}
                hooks={props.hooks}
                goToSettings={props.goToSettings}
            />
            <div className="game-body">
                <TopBar></TopBar>
                <div className="board-holder">
                    <Board
                        gameSettings={props.gameSettings}
                        gameStateHook={props.gameStateHook}
                        hooks={props.hooks}
                    ></Board>
                </div>
            </div>
        </div>
    );
}