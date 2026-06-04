import {StateHook} from "../include/react_hook";

export interface GameConfigHooks {
    flipBoard: StateHook<boolean>;
    showSuggestions: StateHook<boolean>;
}
