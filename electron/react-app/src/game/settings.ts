export type PlayerKind = "Human" | "Computer";

export interface GameSettings {
    white: PlayerKind;
    black: PlayerKind;
}

export const defaultGameSettings: GameSettings = {
    white: "Human",
    black: "Computer",
};

export function isComputerTurn(settings: GameSettings, whiteTurn: boolean): boolean {
    return whiteTurn ? settings.white === "Computer" : settings.black === "Computer";
}