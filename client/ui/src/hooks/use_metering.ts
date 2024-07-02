import { interval } from "d3";
import { RefObject, useEffect, useRef } from "react";
import { z } from "zod";

import { juce } from "@/lib/juce";

const ChannelMetering = z.object({
  isClipping: z.boolean(),
  peakValue: z.number(),
});

export type ChannelMetering = z.infer<typeof ChannelMetering>;

const Metering = z.object({
  inputChannels: z.array(ChannelMetering),
  outputChannels: z.array(ChannelMetering),
});

export type Metering = z.infer<typeof Metering>;

const getMeteringNative = juce.getNativeFunction("get_metering_native");

interface IUseMetering {
  metering: RefObject<Metering>;
}

const useMetering = (): IUseMetering => {
  const meteringRef = useRef<Metering>({
    inputChannels: [],
    outputChannels: [],
  });

  const getMetering = async () => {
    return getMeteringNative();
  };

  const handleReceiveMetering = (data: any) => {
    try {
      meteringRef.current = Metering.parse(JSON.parse(data));
    } catch (err) {
      console.error("Failed to parse Metering!", err);
    }
  };

  useEffect(() => {
    const t = interval(async () => {
      handleReceiveMetering(await getMetering());
    }, 20);

    return () => t.stop();
  }, []);

  return {
    metering: meteringRef,
  };
};

export { useMetering };
