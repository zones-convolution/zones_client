import { ChannelMeter, MeteringIPC } from "@/ipc/metering_ipc";

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

export class MeterRenderer {
  private readonly meteringIPC;
  private channelGroups: SmoothedChannelGroups = [];

  constructor(meteringIPC: MeteringIPC) {
    this.meteringIPC = meteringIPC;
  }

  public buildChannelGroupsIfUpdated = () => {
    let ipcDimensions = this.meteringIPC.channelGroups.map((grp) => grp.length);
    let smoothedDimensions = this.channelGroups.map((grp) => grp.length);

    if (!compare(ipcDimensions, smoothedDimensions)) {
      this.channelGroups = [];
      this.meteringIPC.channelGroups.forEach((grp) => {
        this.channelGroups.push(
          grp.map(() => ({
            smoothedTarget: 0.0,
            smoothedPeak: 0.0,
            peakFadeTimer: 0,
          })),
        );
      });
    }
  };

  public render = (): SmoothedChannelGroups => {
    this.buildChannelGroupsIfUpdated();

    for (let grpIndex = 0; grpIndex < this.channelGroups.length; ++grpIndex) {
      let grp = this.channelGroups[grpIndex];
      for (let channelIndex = 0; channelIndex < grp.length; ++channelIndex) {
        updateChannelMeter(
          grp[channelIndex],
          this.meteringIPC.channelGroups[grpIndex][channelIndex],
        );
      }
    }

    return this.channelGroups;
  };
}
