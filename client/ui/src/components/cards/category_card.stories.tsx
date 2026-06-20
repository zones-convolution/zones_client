import type { Meta, StoryObj } from "@storybook/react";

import ZonesLight from "@/assets/zones_light.svg";
import { CategoryCard } from "@/components/cards/category_card";

const meta: Meta<typeof CategoryCard> = {
  component: CategoryCard,
  tags: ["autodocs"],
  parameters: {
    layout: "centered",
  },
  argTypes: {
    category: { control: "text" },
    imageUrl: { control: "text" },
    rt60: { control: "number" },
  },
  decorators: [(story) => <div className="w-80 h-40">{story()}</div>],
};

export default meta;

type Story = StoryObj<typeof meta>;

export const CategoryCardStory: Story = {
  args: {
    imageUrl: ZonesLight,
    category: "Forest",
    rt60: 12.86,
  },
};
