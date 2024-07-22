import { mapFrom01Linear } from "@dsp-ts/math";
import { arc, DefaultArcObject, symbol, symbolTriangle } from "d3";
import { FC } from "react";

const angleMin = -145;
const angleMax = 145;

const arc_gen = arc();
const arc_style: DefaultArcObject = {
  innerRadius: 110,
  outerRadius: 120,
  startAngle: (angleMin / 180) * Math.PI,
  endAngle: (angleMax / 180) * Math.PI,
};

const track = arc_gen(arc_style)!;

const midpoint_indicator = symbol()
  .type(symbolTriangle)
  .size(10 * 10);

const KnobBaseThumb: FC<{
  value01: number;
  midpoint: number;
  showMidpointIndicator: boolean;
  trackFromMidpoint: boolean;
}> = ({ value01, midpoint, showMidpointIndicator, trackFromMidpoint }) => {
  const angle = mapFrom01Linear(value01, angleMin, angleMax);
  const midpointAngle = mapFrom01Linear(midpoint, angleMin, angleMax);

  const track_fill = arc_gen({
    ...arc_style,
    ...(trackFromMidpoint && {
      startAngle: (midpointAngle / 180) * Math.PI,
    }),
    endAngle: (angle / 180) * Math.PI,
  })!;

  return (
    <div className="relative w-16 h-16">
      {showMidpointIndicator && (
        <svg
          className="w-16 h-16 absolute"
          viewBox="-120 -120 240 240"
          style={{
            rotate: `${midpoint * 290 - 145}deg`,
          }}
        >
          <path
            className="stroke-0 fill-primary"
            d={midpoint_indicator()!}
            transform="rotate(180) translate(0 115)"
          />
        </svg>
      )}
      <svg
        className="w-12 h-12 absolute"
        viewBox="-120 -120 240 240"
        style={{
          top: "50%",
          left: "50%",
          transform: "translate(-50%, -50%)",
        }}
      >
        <path className="stroke-0 fill-background" d={track} />
        <path className="stroke-0 fill-primary" d={track_fill} />
      </svg>
    </div>
  );
};

export { KnobBaseThumb };
