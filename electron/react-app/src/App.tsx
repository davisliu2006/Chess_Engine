import "./css/App.css";
import "./assets/font-awesome/css/all.min.css";
import {useEffect, useState} from "react";
import {runEngineSmokeTest} from "./game/engine_smoke";
import GameView from "./components/GameView";
import SettingsScreen from "./components/SettingsScreen";
import {stateHook} from "./include/react_hook";
import {GameSettings} from "./game/settings";
import useGameState from "./game/useGameState";

type AppScreen = "settings" | "game";

export default function App() {
    useEffect(() => {
        runEngineSmokeTest();
    }, []);

    let [screen, setScreen] = useState<AppScreen>("settings");
    let [gameSettings, setGameSettings] = useState<GameSettings | null>(null);

    let config = {
        flipBoard: stateHook(useState(false)),
    };

    let gameStateHook = useGameState();

    let startGame = (settings: GameSettings) => {
        setGameSettings(settings);
        gameStateHook.reset();
        gameStateHook.setSelectedPiece(null);
        setScreen("game");
    };

    let goToSettings = () => {
        setScreen("settings");
    };

    if (screen == "settings") {
        return <SettingsScreen onStart={startGame}></SettingsScreen>;
    } else {
        return (
            <GameView
                gameSettings={gameSettings!}
                gameStateHook={gameStateHook}
                hooks={config}
                goToSettings={goToSettings}
            ></GameView>
        );
    }
}