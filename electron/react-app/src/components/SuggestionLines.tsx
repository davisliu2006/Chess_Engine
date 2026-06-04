import {EngineMove} from "../game/engine";

function squareCenterSvg(
    x: number,
    y: number,
    flipBoard: boolean
): [number, number] {
    const leftPct = flipBoard ? 12.5*(7-x) : 12.5*x;
    const bottomPct = flipBoard ? 12.5*(7-y) : 12.5*y;
    const cx = leftPct+6.25;
    const cy = 100-bottomPct-6.25;
    return [cx, cy];
}

export interface SuggestionLinesProps {
    moves: EngineMove[];
    flipBoard: boolean;
}

export default function SuggestionLines(props: SuggestionLinesProps) {
    if (props.moves.length == 0) {
        return null;
    }

    return (
        <svg
            className="BoardSuggestions"
            viewBox="0 0 100 100"
            preserveAspectRatio="none"
        >
            {props.moves.map((move, index) => {
                const [[fromX, fromY], [toX, toY]] = move;
                const [x1, y1] = squareCenterSvg(
                    fromX, fromY,
                    props.flipBoard
                );
                const [x2, y2] = squareCenterSvg(toX, toY, props.flipBoard);
                return (
                    <line
                        key={index}
                        x1={x1} y1={y1}
                        x2={x2} y2={y2}
                    />
                );
            })}
        </svg>
    );
}
