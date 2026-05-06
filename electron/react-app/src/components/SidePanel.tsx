import "../css/App.css";
import "../css/SidePanel.css"
import "../assets/font-awesome/css/all.min.css";
import { StateHook } from "../include/react_hook";

export function Timer(props: any) {
    return (
        <div className="Timer">
            <p>0:00:00</p>
        </div>
    )
}

export interface SidePanelButtonProps {
    onClick?: (event: React.MouseEvent<HTMLDivElement>) => void;
    children: React.ReactNode;
}
export function SidePanelButton(props: SidePanelButtonProps) {
    return (
        <div className="SidePanelButton" onClick={props.onClick}>
            {props.children}
        </div>
    );
}

export interface SidePanelProps {
    hooks: {
        flipBoard: StateHook<boolean>;
    };
}
export default function SidePanel(props: SidePanelProps) {
    return (
        <div className="SidePanel">
            <p>Chess Game</p>
            <Timer></Timer>
            <p>Show Hints</p>
            <div>
                <SidePanelButton>
                    <i className="fas fa-chevron-left"></i>
                </SidePanelButton>
                <SidePanelButton>
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
        </div>
    );
}