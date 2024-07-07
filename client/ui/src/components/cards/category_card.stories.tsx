import type { Meta, StoryObj } from "@storybook/react";

import Forest1 from "@/assets/forest-1.png";
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
    imageUrl: Forest1,
    category: "Forest",
    rt60: 12.86,
  },
};
