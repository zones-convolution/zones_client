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
}

interface IUseSlider {
  changeCommitted: (value: number[]) => void;
  mouseDown: () => void;
  handleChange: (value: number[]) => void;
  value: number[];
  properties: ISliderProperties;
  getScaledValue: () => number;
}

const useSlider = (identifier: string): IUseSlider => {
  const sliderState = juce.getSliderState(identifier);

  const [value, setValue] = useState(sliderState.getNormalisedValue());
  const [properties, setProperties] = useState(sliderState.properties);

  useEffect(() => {
    const propertiesListenerId = sliderState.propertiesChangedEvent.addListener(
      () => setProperties(sliderState.properties),
    );

    const valueListenerId = sliderState.valueChangedEvent.addListener(() => {
      setValue(sliderState.getNormalisedValue());
    });

    return function cleanup() {
      sliderState.valueChangedEvent.removeListener(valueListenerId);
      sliderState.propertiesChangedEvent.removeListener(propertiesListenerId);
    };
  }, []);

  const handleChange = (newValue: number[]) => {
    sliderState.setNormalisedValue(newValue[0]);
    setValue(newValue[0]);
  };

  const mouseDown = () => {
    sliderState.sliderDragStarted();
  };

  const changeCommitted = (newValue: number[]) => {
    sliderState.setNormalisedValue(newValue[0]);
    sliderState.sliderDragEnded();
  };

  const getScaledValue = (): number => {
    return sliderState.getScaledValue();
  };

  return {
    value: [value],
    properties: properties,
    handleChange: handleChange,
    mouseDown: mouseDown,
    changeCommitted: changeCommitted,
    getScaledValue: getScaledValue,
  };
};

export { useSlider };
