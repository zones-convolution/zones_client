import { ChannelGroups, ChannelMeter, MeteringIPC } from "@/ipc/metering_ipc";

interface ISmoothedChannelMeter {
  smoothedTarget: number;
  smoothedPeak: number;
  peakFadeTimer: number;
}

export type SmoothedChannelGroups = ISmoothedChannelMeter[][];

const smoothedValue = (
  valueToSmooth: number,
  target: number,
  smoothingConstant: number,
) => {
  let delta = 1.0 / smoothingConstant;
  let step = (target - valueToSmooth) * delta;
  return valueToSmooth + step;
};

const updateChannelMeter = (
  smoothed: ISmoothedChannelMeter,
  target: ChannelMeter,
) => {
  let peak = target.peakValue;
  let isClipping = target.isClipping;

  smoothed.smoothedTarget =
    peak < smoothed.smoothedTarget
      ? smoothedValue(smoothed.smoothedTarget, peak, 80.0)
      : peak;

  let smoothedPeak = smoothed.smoothedPeak;

  if (peak >= smoothedPeak) {
    smoothed.smoothedPeak = peak;
    smoothed.peakFadeTimer = 0;
  } else {
    smoothed.peakFadeTimer += 10;
    const kPeakFadeDurationMs = 1000;

    if (smoothed.peakFadeTimer >= kPeakFadeDurationMs)
      smoothed.smoothedPeak = smoothedValue(smoothed.smoothedPeak, 0, 100.0);
  }
};

const compare = (a1: number[], a2: number[]) =>
  a1.length == a2.length && a1.every((element, index) => element === a2[index]);

const shouldBuildNewGroups = (
  groups: SmoothedChannelGroups,
  targetGroups: ChannelGroups,
) => {
  let ipcDimensions = targetGroups.map((grp) => grp.length);
  let smoothedDimensions = groups.map((grp) => grp.length);
  return !compare(ipcDimensions, smoothedDimensions);
};

const buildChannelGroups = (targetGroups: ChannelGroups) => {
  let newGroups: SmoothedChannelGroups = [];

  targetGroups.forEach((grp) => {
    newGroups.push(
      grp.map(() => ({
        smoothedTarget: 0.0,
        smoothedPeak: 0.0,
        peakFadeTimer: 0,
      })),
    );
  });

  return newGroups;
};

export const renderMeters = (
  groups: SmoothedChannelGroups,
  targetGroups: ChannelGroups,
): SmoothedChannelGroups => {
  let newGroups;

  if (shouldBuildNewGroups(groups, targetGroups))
    newGroups = buildChannelGroups(targetGroups);
  else newGroups = [...groups];

  for (let grpIndex = 0; grpIndex < newGroups.length; ++grpIndex) {
    let grp = newGroups[grpIndex];
    for (
      let channelIndex = 0;
      channelIndex < newGroups.length;
      ++channelIndex
    ) {
      updateChannelMeter(
        grp[channelIndex],
        targetGroups[grpIndex][channelIndex],
      );
    }
  }

  return newGroups;
};
