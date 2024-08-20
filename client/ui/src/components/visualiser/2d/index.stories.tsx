import type { Meta, StoryObj } from "@storybook/react";

import { Visualiser2D } from "@/components/visualiser/2d";

const meta: Meta<typeof Visualiser2D> = {
  component: Visualiser2D,
  parameters: {
    layout: "fullscreen",
  },
  decorators: [
    (story) => <div className="flex min-w-full min-h-full">{story()}</div>,
  ],
};

export default meta;

type Story = StoryObj<typeof Visualiser2D>;

export const Visualiser2DStory: Story = {
  args: {},
};
