import type { Meta, StoryObj } from "@storybook/react";

import { Visualiser } from "@/components/visualiser/visualiser";

const meta: Meta<typeof Visualiser> = {
  component: Visualiser,
  parameters: {
    layout: "fullscreen",
  },
  decorators: [
    (story) => <div className="flex min-w-full min-h-full">{story()}</div>,
  ],
};

export default meta;

type Story = StoryObj<typeof Visualiser>;

export const VisualiserStory: Story = {
  args: {},
};
