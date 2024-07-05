import type { Meta, StoryObj } from "@storybook/react";

import { Meter } from "@/components/meter";

const meta: Meta<typeof Meter> = {
  component: Meter,
  parameters: {
    layout: "centered",
  },
  decorators: [(story) => <div className="w-40 h-80">{story()}</div>],
};

export default meta;

type Story = StoryObj<typeof Meter>;

export const MeterStory: Story = {
  args: {},
};
