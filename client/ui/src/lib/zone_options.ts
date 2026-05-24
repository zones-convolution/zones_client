import { ChannelFormat } from "@/hooks/zone_metadata";
import { GenerationType, SpeakerPositions, ZoneLicense } from "@/lib/zones";

interface IZoneOption {
  label: string;
  value: string;
  href?: string;
  description?: string;
}

export type IZoneOptions = IZoneOption[];

export const getFilterOptionByValue = (
  zoneOptions: IZoneOptions,
  optionValue: string,
) => zoneOptions.find((zoneOption) => zoneOption.value === optionValue);

export const SpaceCategoryOptions: IZoneOptions = [
  {
    label: "Church",
    value: "church",
  },
  {
    label: "Canyon",
    value: "canyon",
  },
  {
    label: "Amphitheatre",
    value: "amphitheatre",
  },
  {
    label: "Reverb device",
    value: "reverb-device",
  },
  {
    label: "Sports hall",
    value: "sports-hall",
  },
  {
    label: "Vehicle interior",
    value: "vehicle-interior",
  },
  {
    label: "Arena",
    value: "arena",
  },
  {
    label: "Hall",
    value: "hall",
  },
  {
    label: "Opera house",
    value: "opera-house",
  },
  {
    label: "Scoring stage",
    value: "scoring-stage",
  },
  {
    label: "Stadium",
    value: "stadium",
  },
  {
    label: "Concert hall",
    value: "concert-hall",
  },
  {
    label: "Auditorium",
    value: "auditorium",
  },
  {
    label: "Membrane",
    value: "membrane",
  },
  {
    label: "Recording studio",
    value: "recording-studio",
  },
  {
    label: "Small room",
    value: "small-room",
  },
  {
    label: "Theatre",
    value: "theatre",
  },
  {
    label: "Other",
    value: "other",
  },
];

export const GenerationTypeOptions: IZoneOptions = [
  {
    label: "Real World",
    value: "REAL_WORLD" as GenerationType,
  },
  {
    label: "Computer Model",
    value: "COMPUTER_MODEL" as GenerationType,
  },
  {
    label: "Reverb Device",
    value: "REVERB_DEVICE" as GenerationType,
  },
];

export const ZoneLicenseOptions: IZoneOptions = [
  {
    label: "CC0 1.0 Universal",
    value: "CC0_1_0" as ZoneLicense,
    href: "https://creativecommons.org/publicdomain/zero/1.0/",
    description:
      "Waives copyright and related rights as far as possible, allowing reuse without attribution.",
  },
  {
    label: "Creative Commons Attribution 4.0 International",
    value: "CC_BY_4_0" as ZoneLicense,
    href: "https://creativecommons.org/licenses/by/4.0/",
    description:
      "Allows reuse and adaptation for any purpose, provided appropriate attribution is given.",
  },
];

export const TagOptions: IZoneOptions = [
  {
    label: "Big",
    value: "big",
  },
  {
    label: "Larger Than Life",
    value: "larger-than-life",
  },
  {
    label: "Small",
    value: "small",
  },
  {
    label: "Air",
    value: "air",
  },
];

export const ChannelFormatOptions: IZoneOptions = [
  {
    label: "Stereo",
    value: "STEREO" as ChannelFormat,
  },
  {
    label: "Mono",
    value: "MONO" as ChannelFormat,
  },
  {
    label: "FOA",
    value: "FOA" as ChannelFormat,
  },
];

export const SpeakerPositionsOptions: IZoneOptions = [
  {
    label: "Centre",
    value: "C" as SpeakerPositions,
  },
  {
    label: "Left, Right",
    value: "LR" as SpeakerPositions,
  },
  {
    label: "Centre, Left, Right",
    value: "CLR" as SpeakerPositions,
  },
];
