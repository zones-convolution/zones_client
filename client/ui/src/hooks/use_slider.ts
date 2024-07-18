import { useEffect, useState } from "react";

import { juce } from "@/lib/juce";

interface ISliderProperties {
  start: number;
  end: number;
  skew: number;
  name: string;
  label: string;
  numSteps: number;
  interval: number;
  parameterIndex: number;
  defaultValue: number;
}

interface IUseSlider {
  changeCommitted: (value: number) => void;
  mouseDown: () => void;
  handleChange: (value: number) => void;
  valueNormalised: number;
  valueScaled: number;
  toNormalised: (scaled: number) => number;
  properties: ISliderProperties;
}

const useSlider = (identifier: string): IUseSlider => {
  const sliderState = juce.getSliderState(identifier);

  const [valueNormalised, setValueNormalised] = useState(
    sliderState.getNormalisedValue(),
  );

  const valueScaled = sliderState.normalisedToScaledValue(valueNormalised);

  const [properties, setProperties] = useState(sliderState.properties);

  useEffect(() => {
    const propertiesListenerId = sliderState.propertiesChangedEvent.addListener(
      () => setProperties(sliderState.properties),
    );

    const valueListenerId = sliderState.valueChangedEvent.addListener(() => {
      setValueNormalised(sliderState.getNormalisedValue());
    });

    return function cleanup() {
      sliderState.valueChangedEvent.removeListener(valueListenerId);
      sliderState.propertiesChangedEvent.removeListener(propertiesListenerId);
    };
  }, []);

  const handleChange = (newValue: number) => {
    sliderState.setNormalisedValue(newValue);
    setValueNormalised(newValue);
  };

  const mouseDown = () => {
    sliderState.sliderDragStarted();
  };

  const changeCommitted = (newValue: number) => {
    sliderState.setNormalisedValue(newValue);
    sliderState.sliderDragEnded();
  };

  const toNormalised = (scaledValue: number) => {
    return Math.pow(
      (scaledValue - properties.start) / (properties.end - properties.start),
      properties.skew,
    );
  };

  return {
    valueNormalised: valueNormalised,
    valueScaled: valueScaled,
    properties: properties,
    handleChange: handleChange,
    mouseDown: mouseDown,
    changeCommitted: changeCommitted,
    toNormalised: toNormalised,
  };
};

export { useSlider };
