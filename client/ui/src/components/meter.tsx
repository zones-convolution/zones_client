import { interval, Timer, timer } from "d3";
import { FC, useEffect, useState } from "react";

import { MeteringIPC } from "@/ipc/metering_ipc";
import { MeterRenderer, SmoothedChannelGroups } from "@/lib/meter_renderer";

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

const Meter = () => {
  const [channelGroups, setChannelGroups] = useState<SmoothedChannelGroups>([]);

  useEffect(() => {
    const meterIPC = new MeteringIPC();
    const meterRenderer = new MeterRenderer(meterIPC);

    const i = interval(async () => {
      await meterIPC.update();
    }, 20);

    const t = timer((elapsed) => {
      setChannelGroups([...meterRenderer.render()]);
    });

    return () => {
      t.stop();
      i.stop();
    };
  }, []);

  return (
    <div className="w-full h-80 flex flex-row gap-2">
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
