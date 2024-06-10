import { FC, useEffect, useState } from "react";

import { Slider } from "@/components/ui/slider";

import { juce } from "@/lib/juce";

const controlParameterIndexAnnotation = "controlparameterindex";

const JuceSlider: FC<{
  identifier: string;
  title: string;
}> = ({ identifier, title }) => {
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

  const handleChange = (newValue: any) => {
    sliderState.setNormalisedValue(newValue);
    setValue(newValue);
  };

  const mouseDown = () => {
    sliderState.sliderDragStarted();
  };

  const changeCommitted = (newValue: any) => {
    sliderState.setNormalisedValue(newValue);
    sliderState.sliderDragEnded();
  };

  return (
    <div
      {...{
        [controlParameterIndexAnnotation]:
          sliderState.properties.parameterIndex,
      }}
    >
      <h2 className="mt-1">
        {properties.name}: {sliderState.getScaledValue()} {properties.label}
      </h2>
      <Slider
        aria-label={title}
        value={[value]}
        onValueChange={(v) => handleChange(v)}
        min={0}
        max={1}
        step={1 / (properties.numSteps - 1)}
        onValueCommit={changeCommitted}
        onMouseDown={mouseDown}
      />
    </div>
  );
};

const Editor = () => {
  return (
    <div className="p-4">
      <JuceSlider identifier="dry_wet_mix_parameter" title="Wet/Dry" />
    </div>
  );
};

export default Editor;
