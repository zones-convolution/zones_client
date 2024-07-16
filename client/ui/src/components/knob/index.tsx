import { FC } from "react";

import { KnobBase } from "@/components/knob/knob_base";
import { controlParameterIndexAnnotation } from "@/hooks/use_control_parameter_index_updater";
import { useSlider } from "@/hooks/use_slider";

const Knob: FC<{ identifier: string }> = ({ identifier }) => {
  const {
    properties,
    valueScaled,
    handleChange,
    changeCommitted,
    mouseDown,
    toNormalised,
  } = useSlider(identifier);

  const stepFn = (valueRaw: number) => {
    return properties.interval;
  };

  const stepFnLarger = (valueRaw: number) => {
    return properties.interval * 10;
  };

  const setValueRaw = (valueRaw: number) => {
    handleChange(toNormalised(valueRaw));
  };

  const rawValueCommit = (valueRaw: number) => {
    changeCommitted(toNormalised(valueRaw));
  };

  const valueRawRoundFn = (value: number) => value;

  const valueRawDisplayFn = (valueRaw: number): string =>
    `${valueRaw.toFixed(2)}${properties.label}`;

  return (
    <KnobBase
      onRawValueCommit={rawValueCommit}
      onMouseDown={mouseDown}
      name={properties.name}
      valueRaw={valueScaled}
      setValueRaw={setValueRaw}
      stepFn={stepFn}
      stepLargerFn={stepFnLarger}
      valueMin={properties.start}
      valueMax={properties.end}
      valueRawRoundFn={valueRawRoundFn}
      valueRawDisplayFn={valueRawDisplayFn}
      {...{
        [controlParameterIndexAnnotation]: properties.parameterIndex,
      }}
    />
  );
};

export { Knob };
