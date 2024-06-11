import { ComponentProps } from "react";

import { KnobBase } from "@/components/knob_base";

type KnobBaseProps = ComponentProps<typeof KnobBase>;
type KnobPercentageProps = Pick<KnobBaseProps, "label" | "orientation">;

const valueMin = 0;
const valueMax = 100;
const valueDefault = 50;

const stepFn = (valueRaw: number): number => 1;
const stepLargerFn = (valueRaw: number): number => 10;
const valueRawRoundFn = Math.round;
const valueRawDisplayFn = (valueRaw: number): string =>
  `${valueRawRoundFn(valueRaw)}%`;

const KnobPercentage = (props: KnobPercentageProps) => {
  return (
    <KnobBase
      valueDefault={valueDefault}
      valueMin={valueMin}
      valueMax={valueMax}
      stepFn={stepFn}
      stepLargerFn={stepLargerFn}
      valueRawRoundFn={valueRawRoundFn}
      valueRawDisplayFn={valueRawDisplayFn}
      {...props}
    />
  );
};

export { KnobPercentage };
