const {contextBridge, ipcRenderer} = require("electron");

contextBridge.exposeInMainWorld("chessEngine", {
    getValidMoves: (board, iswhite) =>
        ipcRenderer.invoke("chess:getValidMoves", board, iswhite),
    getScoresRecursive: (board, moves, depth) =>
        ipcRenderer.invoke("chess:getScoresRecursive", board, moves, depth),
});