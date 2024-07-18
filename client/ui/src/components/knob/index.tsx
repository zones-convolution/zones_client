import { mapFrom01Linear, mapTo01Linear } from "@dsp-ts/math";
import { FC } from "react";

import { KnobBase } from "@/components/knob/knob_base";
import { controlParameterIndexAnnotation } from "@/hooks/use_control_parameter_index_updater";
import { useSlider } from "@/hooks/use_slider";

const mapTo01Skewed = (
  x: number,
  min: number,
  max: number,
  skew: number,
): number => {
  const linearMapped = mapTo01Linear(x, min, max);
  return Math.pow(linearMapped, skew);
};

const mapFrom01Skewed = (
  x: number,
  min: number,
  max: number,
  skew: number,
): number => {
  let skewedValue = Math.pow(x, 1 / skew);
  return mapFrom01Linear(skewedValue, min, max);
};

const Knob: FC<{
  identifier: string;
  showMidpointIndicator?: boolean;
  trackFromMidpoint?: boolean;
}> = ({
  identifier,
  showMidpointIndicator = true,
  trackFromMidpoint = false,
}) => {
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

  const resetToDefault = () => {
    mouseDown();
    changeCommitted(properties.defaultValue);
  };

  const valueRawRoundFn = (value: number) => value;

  const valueRawDisplayFn = (valueRaw: number): string => {
    const label = properties.label;

    let precision = 1;
    if (label == "%" || label == "ms") precision = 0;

    return `${valueRaw.toFixed(precision)}${properties.label}`;
  };

  const mapTo01 = (x: number, min: number, max: number) =>
    mapTo01Skewed(x, min, max, properties.skew);

  const mapFrom01 = (x: number, min: number, max: number) =>
    mapFrom01Skewed(x, min, max, properties.skew);

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
      mapTo01={mapTo01}
      mapFrom01={mapFrom01}
      midpoint={properties.defaultValue}
      showMidpointIndicator={showMidpointIndicator}
      onResetToDefault={resetToDefault}
      trackFromMidpoint={trackFromMidpoint}
      {...{
        [controlParameterIndexAnnotation]: properties.parameterIndex,
      }}
    />
  );
};

export { Knob };
