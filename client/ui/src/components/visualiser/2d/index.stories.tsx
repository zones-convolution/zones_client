import type { Meta, StoryObj } from "@storybook/react";
import { fn } from "@storybook/test";
import { DataTexture, RedFormat } from "three";

import { Visualiser2D } from "@/components/visualiser/2d";
import {
  defaultHeight,
  defaultWidth,
} from "@/components/visualiser/visualiser_utils";

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

const createVisualiserTexture = (): Uint8Array => {
  const size = defaultWidth * defaultHeight;
  const data = new Uint8Array(size);

  for (let channel = 0; channel < defaultHeight; ++channel) {
    const channelOffset = channel * defaultHeight;
    for (let fftPoint = 0; fftPoint < defaultHeight; ++fftPoint) {
      if ((fftPoint + channelOffset + channel) % 8 == 0)
        data[fftPoint + channelOffset] =
          ((Math.sin(fftPoint) + 1.0) / 2.0) * 255;
    }
  }

  return data;
};

// export const Visualiser2DStory: Story = {
//   args: {
//     context: {
//       render: createVisualiserTexture(),
//       sensitivity: 1.0,
//       contrast: 1.0,
//       scale: "linear",
//       colourMap: "jet",
//       setColourMap: fn(),
//       setScale: fn(),
//       setSensitivity: fn(),
//       setContrast: fn(),
//     },
//   },
// };
