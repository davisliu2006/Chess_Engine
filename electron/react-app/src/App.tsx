import "./css/App.css";
import "./assets/font-awesome/css/all.min.css";
import {useEffect, useState} from "react";
import {runEngineSmokeTest} from "./game/engine_smoke";
import GameView from "./components/GameView";
import SettingsScreen from "./components/SettingsScreen";
import {stateHook} from "./include/react_hook";
import {GameConfigHooks} from "./game/config";
import {defaultGameSettings, GameSettings} from "./game/settings";
import useGameState from "./game/useGameState";

type AppScreen = "settings" | "game";

export default function App() {
    useEffect(() => {
        runEngineSmokeTest();
    }, []);

    let [screen, setScreen] = useState<AppScreen>("settings");
    let [gameSettings, setGameSettings] = useState<GameSettings | null>(null);

    let config: GameConfigHooks = {
        flipBoard: stateHook(useState(false)),
        showSuggestions: stateHook(useState(false)),
        pauseComputer: stateHook(useState(false)),
    };

    let gameStateHook = useGameState();

    let startGame = (settings: GameSettings) => {
        setGameSettings(settings);
        gameStateHook.reset();
        gameStateHook.setSelectedPiece(null);
        setScreen("game");
    };

    let applySettings = (settings: GameSettings) => {
        setGameSettings(settings);
        setScreen("game");
    };

    let goToSettings = () => {
        setScreen("settings");
    };

    if (screen == "settings") {
        return (
            <SettingsScreen
                settings={gameSettings ?? defaultGameSettings}
                onStartNewGame={startGame}
                onUpdateCurrentGame={applySettings}
            ></SettingsScreen>
        );
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