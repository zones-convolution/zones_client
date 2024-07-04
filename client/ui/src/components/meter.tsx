import { interval, timer } from "d3";
import { FC, ReactNode, useEffect, useState } from "react";
import { Simulate } from "react-dom/test-utils";

import { Separator } from "@/components/ui/separator";

import {
  ChannelGroups,
  getMetering,
  resetChannelClipping,
} from "@/ipc/metering_ipc";
import { renderMeters, SmoothedChannelGroups } from "@/lib/meter_renderer";
import { cn } from "@/lib/utils";

const gradient = `linear-gradient(to top, rgb(75, 217, 102), rgb(255, 149, 0)`;

const MeterBar: FC<{ peak: number; fill: number; levelBars: ReactNode }> = ({
  fill,
  peak,
  levelBars,
}) => {
  const barWidth = 1;
  return (
    <div className="h-full w-full bg-card relative">
      <div
        className="absolute w-full h-full"
        style={{
          background: gradient,
          clipPath: `inset(${(1 - fill) * 100}% 0px 0px)`,
        }}
      />
      <div
        className="absolute w-full h-full"
        style={{
          background: gradient,
          clipPath: `inset(${(1 - peak) * 100}% 0px ${peak * 100 - barWidth}% 0px)`,
        }}
      />
      <div className="absolute w-full h-full">{levelBars}</div>
    </div>
  );
};

const ClippingIndicator: FC<{
  clipping: Boolean;
  resetClipping: () => Promise<void>;
}> = ({ clipping, resetClipping }) => {
  return (
    <button
      onClick={async () => {
        await resetClipping();
      }}
      className={cn("h-1.5 w-full bg-background", {
        "bg-red-500": clipping,
      })}
    />
  );
};

const Meter = () => {
  const [channelGroups, setChannelGroups] = useState<SmoothedChannelGroups>([]);

  useEffect(() => {
    let targetGroups: ChannelGroups = [];
    let lastUpdate = 0;

    const updateTimer = interval(async () => {
      targetGroups = await getMetering();
    }, 10);

    const renderTimer = timer((elapsed) => {
      let frameDelta = elapsed - lastUpdate;
      setChannelGroups((groups) => {
        return renderMeters(groups, targetGroups, frameDelta);
      });
      lastUpdate = elapsed;
    });

    return () => {
      renderTimer.stop();
      updateTimer.stop();
    };
  }, []);

  const clippingIndicators = (
    <div className="flex flex-row gap-2 px-2 border border-transparent">
      {channelGroups.map((group, groupIndex) => {
        return (
          <div
            className="flex flex-row gap-0.5 w-full bg-card"
            key={groupIndex}
          >
            {group.map((channel, channelIndex) => {
              return (
                <ClippingIndicator
                  key={channelIndex}
                  clipping={channel.clipping}
                  resetClipping={async () => {
                    await resetChannelClipping(groupIndex, channelIndex);
                  }}
                />
              );
            })}
          </div>
        );
      })}
    </div>
  );

  const levelBars = (
    <div className="w-full h-full flex flex-col justify-around opacity-80">
      <Separator />
      <Separator />
      <Separator />
      <Separator />
      <Separator />
      <Separator />
    </div>
  );

  const meterBars = (
    <div className="flex-grow flex flex-row gap-2 bg-background p-2 border">
      {channelGroups.map((group, groupIndex) => {
        return (
          <div className="flex flex-row gap-0.5 w-full" key={groupIndex}>
            {group.map((channel, channelIndex) => {
              return (
                <MeterBar
                  key={channelIndex}
                  peak={channel.smoothedPeak}
                  fill={channel.smoothedTarget}
                  levelBars={levelBars}
                />
              );
            })}
          </div>
        );
      })}
    </div>
  );

  return (
    <div className="w-full h-full flex flex-row">
      <div className="w-full h-full flex flex-col gap-2">
        {clippingIndicators}
        {meterBars}
      </div>
    </div>
  );
};

export { Meter };
