import {ScoredMove} from "../game/engine";

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
    moves: ScoredMove[];
    flipBoard: boolean;
}

export default function SuggestionLines(props: SuggestionLinesProps) {
    if (props.moves.length == 0) {
        return null;
    }

    const bestScore = props.moves[0].score;
    const worstScore = props.moves[props.moves.length - 1].score;

    function strokeWidth(score: number): number {
        if (bestScore == worstScore) {return 1;}
        const t = (score - worstScore) / (bestScore - worstScore);
        return 0.5 + t*0.5;
    }

    return (
        <svg
            className="BoardSuggestions"
            viewBox="0 0 100 100"
            preserveAspectRatio="none"
        >
            {props.moves.map(({move, score}, index) => {
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
                        strokeWidth={strokeWidth(score)}
                    />
                );
            })}
        </svg>
    );
}
