import React from "react";

export interface StateHook<T> {
    val: T;
    set: (x: T) => void;
}

export function stateHook<T>(x: [T, (x: T) => void]): StateHook<T> {
    return {
        val: x[0], set: x[1]
    }
}