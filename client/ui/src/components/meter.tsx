import { interval, timer } from "d3";
import { FC, useEffect, useState } from "react";

import { ChannelGroups, getMetering } from "@/ipc/metering_ipc";
import { renderMeters, SmoothedChannelGroups } from "@/lib/meter_renderer";

const gradient = `linear-gradient(to top, rgb(75, 217, 102), rgb(255, 149, 0)`;

const MeterBar: FC<{ peak: number; fill: number }> = ({ fill, peak }) => {
  const barWidth = 1;

  return (
    <div className="h-full w-full border relative">
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
    </div>
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

  return (
    <div className="w-full h-full flex flex-row gap-2">
      {channelGroups.map((group, index) => {
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
