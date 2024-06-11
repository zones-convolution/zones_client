import { FC } from "react";

import { Slider } from "@/components/ui/slider";

import { KnobPercentage } from "@/components/knob_percentage";
import {
  useControlParameterIndexUpdater,
  controlParameterIndexAnnotation,
} from "@/hooks/use_control_parameter_index_updater";
import { useSlider } from "@/hooks/use_slider";

const JuceSlider: FC<{
  identifier: string;
  title: string;
}> = ({ identifier, title }) => {
  const {
    properties,
    value,
    handleChange,
    changeCommitted,
    mouseDown,
    getScaledValue,
  } = useSlider(identifier);

  return (
    <div
      {...{
        [controlParameterIndexAnnotation]: properties.parameterIndex,
      }}
    >
      <h2 className="mt-1">
        {properties.name}: {getScaledValue()} {properties.label}
      </h2>
      <Slider
        aria-label={title}
        value={value}
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
  useControlParameterIndexUpdater();

  return (
    <div className="flex flex-row justify-around p-4 bg-card">
      {/*<JuceSlider identifier="dry_wet_mix_parameter" title="Wet/Dry" />*/}
      <KnobPercentage label="Input" />
      <KnobPercentage label="Output" />
      <KnobPercentage label="Wet/Dry" />
    </div>
  );
};

export default Editor;
