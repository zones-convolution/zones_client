import { FC, ReactNode } from "react";

import { DiscreteLevelLayout } from "@/components/discrete_level_layout";
import { IUseMetering } from "@/hooks/use_metering";
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

const levels = [0, -3, -6, -9, -12, -18, -30, -60];

const Meter: FC<IUseMetering> = ({ channelGroups, resetChannelClipping }) => {
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

  const meterBars = (
    <div className="flex-grow flex flex-row gap-2 bg-background p-2 border rounded-md">
      {channelGroups.map((group, groupIndex) => {
        return (
          <div className="flex flex-row gap-0.5 w-full" key={groupIndex}>
            {group.map((channel, channelIndex) => {
              return (
                <MeterBar
                  key={channelIndex}
                  peak={channel.smoothedPeak}
                  fill={channel.smoothedTarget}
                  levelBars={
                    <DiscreteLevelLayout levels={levels} showLabels={false} />
                  }
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
      <div className="flex flex-col gap-2 w-12">
        <div className="border flex invisible">
          <ClippingIndicator clipping={false} resetClipping={async () => {}} />
        </div>
        <div className="py-2 h-full border-y border-transparent">
          <DiscreteLevelLayout levels={levels} showLabels={true} />
        </div>
      </div>
      <div className="w-full h-full flex flex-col gap-2">
        {clippingIndicators}
        {meterBars}
      </div>
    </div>
  );
};

export { Meter };
