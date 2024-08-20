import type { Meta, StoryObj } from "@storybook/react";
import { fn } from "@storybook/test";

import Forest1 from "@/assets/forest-1.png";
import { ZoneCard } from "@/components/cards/zone_card";

const meta: Meta<typeof ZoneCard> = {
  component: ZoneCard,
  tags: ["autodocs"],
  parameters: {
    layout: "centered",
  },
  argTypes: {
    category: { control: "text" },
    imageUrl: { control: "text" },
    rt60: { control: "number" },
    loading: { control: "boolean" },
    disabled: { control: "boolean" },
    canLoad: { control: "boolean" },
  },
  args: {
    onLoad: fn(),
    onView: fn(),
  },
  decorators: [(story) => <div className="w-80 h-40">{story()}</div>],
};

export default meta;

type Story = StoryObj<typeof meta>;

export const Default: Story = {
  args: {
    imageUrl: Forest1,
    category: "Forest",
    rt60: 12.86,
    loading: false,
    disabled: false,
    canLoad: true,
  },
};

export const Disabled: Story = {
  args: {
    ...Default.args,
    disabled: true,
  },
};

export const Loading: Story = {
  args: {
    ...Default.args,
    loading: true,
  },
};
