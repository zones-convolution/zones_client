import { timer } from "d3";
import { FC, useEffect, useState } from "react";

import { Separator } from "@/components/ui/separator";

import { ChannelMetering, useMetering } from "@/hooks/use_metering";

const MeterBar: FC<{ peak: number; fill: number }> = ({ fill, peak }) => {
  return (
    <div className="h-full w-full border relative">
      <div
        className="absolute w-full bg-yellow-200 bottom-0"
        style={{ height: `${fill * 100}%` }}
      />
      <div
        className="absolute w-full h-1 bg-cyan-200"
        style={{
          bottom: `${peak * 100}%`,
          transform: "translateY(50%)",
        }}
      />
    </div>
  );
};

interface IChannelMeter {
  smoothedTarget: number;
  smoothedPeak: number;
  peakFadeTimer: number;
}

interface IChannelGroups {
  inputs: IChannelMeter[];
  outputs: IChannelMeter[];
}

const SmoothedValue = (
  valueToSmooth: number,
  target: number,
  smoothingConstant: number,
) => {
  let delta = 1.0 / smoothingConstant;
  let step = (target - valueToSmooth) * delta;
  return valueToSmooth + step;
};

const Meter = () => {
  const meter = useMetering();

  const [channelGroups, setChannelGroups] = useState<IChannelGroups>({
    inputs: [],
    outputs: [],
  });

  const buildChannelGroups = (groups: IChannelGroups) => {
    let metering = meter.metering.current!;

    let smoothedChannelGroups: IChannelGroups;
    let numInputs = channelGroups.inputs.length;
    let numOutputs = channelGroups.inputs.length;

    if (
      numInputs != metering.inputChannels.length ||
      numOutputs != metering.outputChannels.length
    ) {
      console.log("WHY???");
      smoothedChannelGroups = {
        inputs: metering.inputChannels.map(() => ({
          smoothedTarget: 0.0,
          peakFadeTimer: 0,
          smoothedPeak: 0.0,
        })),
        outputs: metering.outputChannels.map(() => ({
          smoothedTarget: 0.0,
          peakFadeTimer: 0,
          smoothedPeak: 0.0,
        })),
      };
    } else {
      smoothedChannelGroups = { ...channelGroups };
    }

    return smoothedChannelGroups;
  };

  const updateMeter = (smoothed: IChannelMeter, target: ChannelMetering) => {
    let peak = target.peakValue;
    let isClipping = target.isClipping;

    smoothed.smoothedTarget =
      peak < smoothed.smoothedTarget
        ? SmoothedValue(smoothed.smoothedTarget, peak, 80.0)
        : peak;

    let smoothedPeak = smoothed.smoothedPeak;

    if (peak >= smoothedPeak) {
      smoothed.smoothedPeak = peak;
      smoothed.peakFadeTimer = 0;
    } else {
      smoothed.peakFadeTimer += 10;
      const kPeakFadeDurationMs = 2000;

      if (smoothed.peakFadeTimer >= kPeakFadeDurationMs)
        smoothed.smoothedPeak = SmoothedValue(smoothed.smoothedPeak, 0, 300.0);
    }
  };

  const updateChannelGroups = () => {
    setChannelGroups((channelGroups) => {
      let smoothedGroups = buildChannelGroups(channelGroups);
      let metering = meter.metering.current!;

      smoothedGroups.inputs.forEach((smoothed, index) => {
        let target = metering.inputChannels[index];
        updateMeter(smoothed, target);
      });

      smoothedGroups.outputs.forEach((smoothed, index) => {
        let target = metering.outputChannels[index];
        updateMeter(smoothed, target);
      });

      return smoothedGroups;
    });
  };

  useEffect(() => {
    const t = timer((elapsed) => {
      let normalised = (Math.sin(elapsed * 0.001) + 1) / 2;
      updateChannelGroups();
    });

    return () => t.stop();
  }, []);

  return (
    <div className="w-full h-80 flex flex-row gap-2">
      {[channelGroups.inputs, channelGroups.outputs].map((group, index) => {
        return (
          <div className="flex flex-row gap-0.5 w-full" key={index}>
            {group.map((channel, index) => {
              return (
                <MeterBar
                  key={index}
                  peak={channel.smoothedPeak}
                  fill={channel.smoothedTarget}
                />
              );
            })}
          </div>
        );
      })}
    </div>
  );
};

export { Meter };
