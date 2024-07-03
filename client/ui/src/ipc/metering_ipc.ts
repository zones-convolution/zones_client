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

const handleReceiveMetering = (data: any): ChannelGroups => {
  try {
    return ChannelGroups.parse(JSON.parse(data));
  } catch (err) {
    console.error("Failed to parse Metering!", err);
  }

  return [];
};

export const getMetering = async () => {
  return handleReceiveMetering(await getMeteringNative());
};
