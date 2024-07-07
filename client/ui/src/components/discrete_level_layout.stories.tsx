import type { Meta, StoryObj } from "@storybook/react";

import { DiscreteLevelLayout } from "@/components/discrete_level_layout";
import { Meter } from "@/components/meter";

const meta: Meta<typeof DiscreteLevelLayout> = {
  component: DiscreteLevelLayout,
  parameters: {
    layout: "centered",
  },
  argTypes: {
    levels: { control: "object" },
  },
  decorators: [(story) => <div className="w-40 h-80">{story()}</div>],
};

export default meta;

type Story = StoryObj<typeof meta>;

export const DiscreteLevelLayoutStory: Story = {
  args: {
    levels: [0, -3, -6, -18, -30, -60],
  },
};
