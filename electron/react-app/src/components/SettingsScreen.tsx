import "../css/App.css";
import "../css/SettingsScreen.css";
import {useState} from "react";
import {
    defaultGameSettings,
    GameSettings,
    PlayerKind,
} from "../game/settings";

interface PlayerKindSelectorProps {
    label: string;
    value: PlayerKind;
    onChange: (kind: PlayerKind) => void;
}

function PlayerKindSelector(props: PlayerKindSelectorProps) {
    return (
        <div className="settings-row">
            <label>{props.label}</label>
            <div className="settings-options">
                {(["Human", "Computer"] as const).map((kind) => (
                    <button
                        key={kind}
                        type="button"
                        className={
                            "settings-option" +
                            (props.value == kind ? " active" : "")
                        }
                        onClick={() => props.onChange(kind)}
                    >
                        {kind}
                    </button>
                ))}
            </div>
        </div>
    );
}

export interface SettingsScreenProps {
    onStart: (settings: GameSettings) => void;
}

export default function SettingsScreen(props: SettingsScreenProps) {
    let [settings, setSettings] = useState<GameSettings>(defaultGameSettings);

    return (
        <div className="SettingsScreen">
            <div className="SettingsPanel">
                <h1>New Game</h1>
                <PlayerKindSelector
                    label="White"
                    value={settings.white}
                    onChange={(white) => setSettings({...settings, white})}
                />
                <PlayerKindSelector
                    label="Black"
                    value={settings.black}
                    onChange={(black) => setSettings({...settings, black})}
                />
                <button
                    type="button"
                    className="SettingsStartButton"
                    onClick={() => props.onStart(settings)}
                >
                    Start Game
                </button>
            </div>
        </div>
    );
}