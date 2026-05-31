const {app, BrowserWindow, ipcMain} = require("electron");
const path = require("path");
const {Board} = require("chess-engine-napi");

ipcMain.handle("chess:getValidMoves", (_event, board, iswhite) => {
    const engine = new Board(board);
    return engine.get_valid_moves(iswhite);
});

ipcMain.handle("chess:getScoresRecursive", (_event, board, moves, depth) => {
    const engine = new Board(board);
    return engine.get_scores_recursive(moves, depth);
});

function smokeTestEngine() {
    function startingBoard() {
        return [
            ["r1", "k1", "b1", "Q1", "K1", "b1", "k1", "r1"],
            ["p1", "p1", "p1", "p1", "p1", "p1", "p1", "p1"],
            ["--", "--", "--", "--", "--", "--", "--", "--"],
            ["--", "--", "--", "--", "--", "--", "--", "--"],
            ["--", "--", "--", "--", "--", "--", "--", "--"],
            ["--", "--", "--", "--", "--", "--", "--", "--"],
            ["p0", "p0", "p0", "p0", "p0", "p0", "p0", "p0"],
            ["r0", "k0", "b0", "Q0", "K0", "b0", "k0", "r0"],
        ];
    }

    try {
        const board = new Board(startingBoard());
        const moves = board.get_valid_moves(true);
        console.log(`(engine smoke test) loaded; ${moves.length} white moves at start:`);
        for (const [[x0, y0], [x1, y1]] of moves) {
            console.log(`(engine smoke test) [${x0}, ${y0}] -> [${x1}, ${y1}]`);
        }
    } catch (err) {
        console.error("(engine smoke test) failed:", err);
    }
}

function createWindow() {
    let win = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            preload: path.join(__dirname, "preload.js"),
            contextIsolation: true,
            nodeIntegration: false
        }
    });

    if (process.env.NODE_ENV == "development") {
        win.loadURL("http://localhost:3000");
    } else {
        win.loadFile("./react-app/build/index.html");
    }
}

app.whenReady().then(() => {
    smokeTestEngine();
    createWindow()

    app.on("activate", () => {
        if (BrowserWindow.getAllWindows().length == 0) {createWindow();}
    })

    app.on("window-all-closed", () => {
        if (process.platform != "darwin") {app.quit();}
    })
})