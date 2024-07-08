import type { Meta, StoryObj } from "@storybook/react";
import { fn } from "@storybook/test";
import { timer } from "d3";
import { useEffect, useState } from "react";

import { Meter } from "@/components/meter";
import {
  deepCopyGroups,
  SmoothedChannelGroup,
  SmoothedChannelGroups,
} from "@/lib/meter_renderer";

const meta: Meta<typeof Meter> = {
  component: Meter,
  parameters: {
    layout: "centered",
  },
  tags: ["autodocs"],
  argTypes: {
    channelGroups: { control: "object" },
  },
  args: {
    resetChannelClipping: fn(),
  },
  decorators: [(story) => <div className="w-40 h-72">{story()}</div>],
};

export default meta;

type Story = StoryObj<typeof meta>;

const createStoryChannelGroups = (numGroups: number, numChannels: number) => {
  let channelGroups: SmoothedChannelGroups = [];

  for (let gi = 0; gi < numGroups; ++gi) {
    let channelGroup: SmoothedChannelGroup = [];

    for (let ci = 0; ci < numChannels; ++ci) {
      channelGroup.push({
        smoothedTarget: 0,
        smoothedPeak: 0,
        peakFadeTimer: 0,
        clipping: false,
      });
    }

    channelGroups.push(channelGroup);
  }

  return channelGroups;
};

export const MeterStory: Story = {
  render: (args) => {
    const [channelGroups, setChannelGroups] = useState<SmoothedChannelGroups>(
      createStoryChannelGroups(2, 4),
    );

    useEffect(() => {
      const renderTimer = timer((elapsed) => {
        setChannelGroups((grps) => {
          let newGroups = deepCopyGroups(grps);

          newGroups.forEach((grp, grpIndex) => {
            grp.forEach((channel, channelIndex) => {
              channel.smoothedTarget =
                (Math.sin(
                  elapsed * 0.001 + (1 + grpIndex) * (1 + channelIndex),
                ) +
                  1.0) /
                2.0;
              channel.smoothedPeak =
                (Math.sin(
                  0.2 + elapsed * 0.001 + (1 + grpIndex) * (1 + channelIndex),
                ) +
                  1.0) /
                2.0;

              channel.clipping = channel.smoothedTarget > 0.8;
            });
          });

          return newGroups;
        });
      });

      return () => {
        renderTimer.stop();
      };
    }, []);

    return (
      <Meter
        resetChannelClipping={args.resetChannelClipping}
        channelGroups={channelGroups}
      />
    );
  },
};
