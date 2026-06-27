import "../css/App.css";
import "../css/SidePanel.css"
import "../assets/font-awesome/css/all.min.css";
import {GameConfigHooks} from "../game/config";
import {GameSettings} from "../game/settings";
import {GameStateHook} from "../game/useGameState";

export function Timer(props: any) {
    return (
        <div className="Timer">
            <p>0:00:00</p>
        </div>
    )
}

export interface SidePanelButtonProps {
    className?: string;
    disabled?: boolean;
    onClick?: (event: React.MouseEvent<HTMLDivElement>) => void;
    children: React.ReactNode;
}
export function SidePanelButton(props: SidePanelButtonProps) {
    const className = [
        "SidePanelButton",
        props.className,
        props.disabled? "disabled" : "",
    ].filter(Boolean).join(" ");
    return (
        <div
            className={className}
            onClick={props.disabled? undefined : props.onClick}
        >
            {props.children}
        </div>
    );
}

export interface SidePanelProps {
    gameSettings: GameSettings;
    gameStateHook: GameStateHook;
    hooks: GameConfigHooks;
    goToSettings: () => void;
}
export default function SidePanel(props: SidePanelProps) {
    const hasComputer = props.gameSettings.white == "Computer"
        || props.gameSettings.black == "Computer";

    return (
        <div className="SidePanel">
            <p>Chess Game</p>
            <p className="SidePanelPlayers">
                White: {props.gameSettings.white}
                <br />
                Black: {props.gameSettings.black}
            </p>
            <Timer></Timer>
            <SidePanelButton
                className={props.hooks.showSuggestions.val? "active" : ""}
                onClick={() => {
                    props.hooks.showSuggestions.set(
                        !props.hooks.showSuggestions.val
                    );
                }}
            >
                <i className="fas fa-lightbulb"></i>
            </SidePanelButton>
            <p className="SidePanelHintLabel">
                {props.hooks.showSuggestions.val
                    ? "Suggestions on"
                    : "Suggestions off"}
            </p>
            {hasComputer && (
                <>
                    <SidePanelButton
                        className={props.hooks.pauseComputer.val? "active" : ""}
                        onClick={() => {
                            props.hooks.pauseComputer.set(
                                !props.hooks.pauseComputer.val
                            );
                        }}
                    >
                        <i className={
                            props.hooks.pauseComputer.val?
                                "fas fa-play" : "fas fa-pause"
                        }></i>
                    </SidePanelButton>
                    <p className="SidePanelHintLabel">{
                        props.hooks.pauseComputer.val?
                            "Computer paused" : "Computer playing"
                    }</p>
                </>
            )}
            <div>
                <SidePanelButton
                    disabled={!props.gameStateHook.canUndo}
                    onClick={() => props.gameStateHook.undo()}
                >
                    <i className="fas fa-chevron-left"></i>
                </SidePanelButton>
                <SidePanelButton
                    disabled={!props.gameStateHook.canRedo}
                    onClick={() => props.gameStateHook.redo()}
                >
                    <i className="fas fa-chevron-right"></i>
                </SidePanelButton>
            </div>
            <div>
                <SidePanelButton>
                    <i className="fas fa-info"></i>
                </SidePanelButton>
                <SidePanelButton onClick={function(event) {
                    props.hooks.flipBoard.set(!props.hooks.flipBoard.val)
                }}>
                    <i className="fas fa-sync-alt"></i>
                </SidePanelButton>
            </div>
            <SidePanelButton onClick={props.goToSettings}>
                <i className="fas fa-cog"></i>
            </SidePanelButton>
        </div>
    );
}