import { mapFrom01Linear } from "@dsp-ts/math";
import { arc, DefaultArcObject, symbol, symbolTriangle } from "d3";
import { FC } from "react";

const angleMin = -145;
const angleMax = 145;

const arc_gen = arc();
const arc_style: DefaultArcObject = {
  innerRadius: 92,
  outerRadius: 100,
  startAngle: (angleMin / 180) * Math.PI,
  endAngle: (angleMax / 180) * Math.PI,
};

const track = arc_gen(arc_style)!;

const midpoint_indicator = symbol()
  .type(symbolTriangle)
  .size(10 * 10);

const KnobBaseThumb: FC<{ value01: number }> = ({ value01 }) => {
  const angle = mapFrom01Linear(value01, angleMin, angleMax);

  const track_fill = arc_gen({
    ...arc_style,
    endAngle: (angle / 180) * Math.PI,
  })!;

  return (
    <svg className="w-12" viewBox="-100 -100 200 240">
      <path
        className="stroke-0 fill-primary"
        d={midpoint_indicator()!}
        transform="rotate(180) translate(0 95)"
      />
      <path
        className="stroke-0 fill-primary-foreground"
        d={track}
        transform="translate(0, 40)"
      />
      <path
        className="stroke-0 fill-primary"
        d={track_fill}
        transform="translate(0, 40)"
      />
    </svg>
  );
};

export { KnobBaseThumb };
