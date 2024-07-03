import { interval, Timer } from "d3";
import { z } from "zod";

import { juce } from "@/lib/juce";

const ChannelMeter = z.object({
  isClipping: z.boolean(),
  peakValue: z.number(),
});
export type ChannelMeter = z.infer<typeof ChannelMeter>;

const ChannelGroups = z.array(z.array(ChannelMeter));
export type ChannelGroups = z.infer<typeof ChannelGroups>;

const getMeteringNative = juce.getNativeFunction("get_metering_native");

export interface IMeteringIPC {
  channelGroups: ChannelGroups;
  update: () => Promise<void>;
}

export class MeteringIPC implements IMeteringIPC {
  public channelGroups: ChannelGroups = [];

  update = async () => {
    this.handleReceiveMetering(await this.getMetering());
  };

  private getMetering = async () => {
    return getMeteringNative();
  };

  private handleReceiveMetering = (data: any) => {
    try {
      this.channelGroups = ChannelGroups.parse(JSON.parse(data));
    } catch (err) {
      console.error("Failed to parse Metering!", err);
    }
  };
}
