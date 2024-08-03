import type { Meta, StoryObj } from "@storybook/react";

import { Visualiser3D } from "@/components/visualiser/3d";

const meta: Meta<typeof Visualiser3D> = {
  component: Visualiser3D,
  parameters: {
    layout: "fullscreen",
  },
  decorators: [
    (story) => <div className="flex min-w-full min-h-full">{story()}</div>,
  ],
};

export default meta;

type Story = StoryObj<typeof Visualiser3D>;

export const Visualiser3DStory: Story = {
  args: {},
};
