import { mapFrom01Linear, mapTo01Linear } from "@dsp-ts/math";
import { Meta, StoryObj } from "@storybook/react";
import React, { useState } from "react";

import { KnobBase, KnobBaseProps } from "@/components/knob/knob_base";

const meta: Meta<typeof KnobBase> = {
  component: KnobBase,
  tags: ["autodocs"],
  argTypes: {
    name: { control: "text" },
    valueMin: { control: "number" },
    valueMax: { control: "number" },
    valueRaw: { control: "number" },
    orientation: { control: "radio", options: ["vertical", "horizontal"] },
    onMouseDown: { action: "mouse down" },
    onRawValueCommit: { action: "raw value commit" },
  },
  parameters: {
    layout: "centered",
  },
};
export default meta;

type Story = StoryObj<typeof KnobBase>;

const Template: StoryObj<KnobBaseProps> = {
  render: (args) => {
    const [valueRaw, setValueRaw] = useState(args.valueRaw);
    return <KnobBase {...args} valueRaw={valueRaw} setValueRaw={setValueRaw} />;
  },
};

export const Default: Story = {
  ...Template,
  args: {
    name: "Volume",
    valueMin: 0,
    valueMax: 100,
    valueRaw: 50,
    valueRawRoundFn: (value) => Math.round(value),
    valueRawDisplayFn: (value) => `${value.toFixed(0)}%`,
    orientation: "vertical",
    stepFn: (value) => 1,
    stepLargerFn: (value) => 10,
    mapTo01: mapTo01Linear,
    mapFrom01: mapFrom01Linear,
  },
};

export const FineControl: Story = {
  ...Template,
  args: {
    ...Default.args,
    name: "Fine Volume",
    valueRaw: 25,
    stepFn: (value) => 0.1,
    stepLargerFn: (value) => 1,
  },
};

export const CoarseControl: Story = {
  ...Template,
  args: {
    ...Default.args,
    name: "Coarse Volume",
    valueRaw: 75,
    stepFn: (value) => 5,
    stepLargerFn: (value) => 20,
  },
};
