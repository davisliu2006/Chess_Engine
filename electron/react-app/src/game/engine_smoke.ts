import {GameState} from "./game";
import {
    EngineMove,
    getValidMoves,
    isEngineAvailable,
} from "./engine";

const LOG_PREFIX = "(engine smoke test)";

function logMove(move: EngineMove) {
    const [[x0, y0], [x1, y1]] = move;
    console.log(`${LOG_PREFIX}   [${x0}, ${y0}] -> [${x1}, ${y1}]`);
}

export async function runEngineSmokeTest(): Promise<boolean> {
    if (!isEngineAvailable()) {
        console.error(`${LOG_PREFIX} window.chessEngine is not available`);
        return false;
    }

    const game = new GameState();
    game.reset();

    try {
        const moves = await getValidMoves(game.pieces, true);
        if (moves.length == 0) {
            console.error(`${LOG_PREFIX} no legal moves found at starting position`);
            return false;
        }
        console.log(`${LOG_PREFIX} ${moves.length} white moves from start:`);
        for (const move of moves) {
            logMove(move);
        }
        console.log(`${LOG_PREFIX} passed`);
        return true;
    } catch (err) {
        console.error(`${LOG_PREFIX} failed:`, err);
        return false;
    }
}

if (process.env.NODE_ENV == "development") {
    window.runEngineSmokeTest = runEngineSmokeTest;
}