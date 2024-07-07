import { interval, timer } from "d3";
import { useEffect, useState } from "react";

import {
  ChannelGroups,
  getMetering,
  resetChannelClipping,
} from "@/ipc/metering_ipc";
import { renderMeters, SmoothedChannelGroups } from "@/lib/meter_renderer";

export interface IUseMetering {
  resetChannelClipping: (
    groupIndex: number,
    channelIndex: number,
  ) => Promise<void>;
  channelGroups: SmoothedChannelGroups;
}

export const useMetering = (): IUseMetering => {
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

  return { channelGroups, resetChannelClipping };
};
