import { mapFrom01Linear, mapTo01Linear } from "@dsp-ts/math";
import { ComponentProps, useId } from "react";
import {
  KnobHeadless,
  KnobHeadlessLabel,
  KnobHeadlessOutput,
  useKnobKeyboardControls,
} from "react-knob-headless";

import { KnobBaseThumb } from "@/components/knob/knob_base_thumb";
import { cn } from "@/lib/utils";

type KnobHeadlessProps = ComponentProps<typeof KnobHeadless>;

export type KnobBaseProps = Pick<
  KnobHeadlessProps,
  | "valueMin"
  | "valueMax"
  | "valueRawRoundFn"
  | "valueRawDisplayFn"
  | "orientation"
  | "mapTo01"
  | "mapFrom01"
> & {
  readonly onRawValueCommit: (valueRaw: number) => void;
  readonly onMouseDown: () => void;
  readonly valueRaw: number;
  readonly setValueRaw: (valueRaw: number) => void;
  readonly label: string;
  readonly stepFn: (valueRaw: number) => number;
  readonly stepLargerFn: (valueRaw: number) => number;
};

const KnobBase = ({
  label,
  valueRaw,
  setValueRaw,
  valueMin,
  valueMax,
  valueRawRoundFn,
  valueRawDisplayFn,
  orientation,
  stepFn,
  stepLargerFn,
  mapTo01 = mapTo01Linear,
  mapFrom01 = mapFrom01Linear,
  onMouseDown,
  onRawValueCommit,
}: KnobBaseProps) => {
  const knobId = useId();
  const labelId = useId();

  const value01 = mapTo01(valueRaw, valueMin, valueMax);
  const step = stepFn(valueRaw);
  const stepLarger = stepLargerFn(valueRaw);
  const dragSensitivity = 0.006;

  const keyboardControlHandlers = useKnobKeyboardControls({
    valueRaw,
    valueMin,
    valueMax,
    step,
    stepLarger,
    onValueRawChange: setValueRaw,
  });

  return (
    <div
      className={cn(
        "w-fit flex flex-col gap-2 justify-center items-center text-xs select-none",
        "outline-none focus-within:outline-1 focus-within:outline-offset-4 focus-within:outline-muted",
      )}
    >
      <KnobHeadlessLabel id={labelId} className="text-primary">
        {label}
      </KnobHeadlessLabel>
      <KnobHeadless
        id={knobId}
        aria-labelledby={labelId}
        className="outline-none w-fit"
        valueMin={valueMin}
        valueMax={valueMax}
        valueRaw={valueRaw}
        valueRawRoundFn={valueRawRoundFn}
        valueRawDisplayFn={valueRawDisplayFn}
        dragSensitivity={dragSensitivity}
        orientation={orientation}
        mapTo01={mapTo01}
        mapFrom01={mapFrom01}
        onValueRawChange={setValueRaw}
        onMouseDown={onMouseDown}
        onMouseUp={() => {
          onRawValueCommit(valueRaw);
        }}
        {...keyboardControlHandlers}
      >
        <KnobBaseThumb value01={value01} />
      </KnobHeadless>
      <KnobHeadlessOutput htmlFor={knobId}>
        {valueRawDisplayFn(valueRaw)}
      </KnobHeadlessOutput>
    </div>
  );
};

export { KnobBase };