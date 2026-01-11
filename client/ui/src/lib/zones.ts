import {
  ImageMetadata,
  IrMetadata,
  ZoneMetadata,
  ChannelFormat,
  PositionMap,
} from "@/hooks/zone_metadata";
import { juce } from "@/lib/juce";
import { getImageUrl } from "@/lib/s3_resources";

export const AllSpeakerPositions = ["C", "LR", "CLR"] as const;
export type SpeakerPositions = (typeof AllSpeakerPositions)[number];

export const containsLeftPosition = (speakerPositions: SpeakerPositions) => {
  return speakerPositions == "LR" || speakerPositions == "CLR";
};

export const containsCentrePosition = (speakerPositions: SpeakerPositions) => {
  return speakerPositions == "C" || speakerPositions == "CLR";
};

export const containsRightPosition = (speakerPositions: SpeakerPositions) => {
  return speakerPositions == "LR" || speakerPositions == "CLR";
};

interface ICoordinate {
  latitude: number;
  longitude: number;
}

export const GenerationTypes = [
  "REAL_WORLD",
  "COMPUTER_MODEL",
  "REVERB_DEVICE",
] as const;

export type GenerationType = (typeof GenerationTypes)[number];

export const ZonesChannelFormats = ["MONO", "STEREO", "FOA"] as const;
export type ZonesChannelFormat = (typeof ZonesChannelFormats)[number];

export type ZoneStatus = "CREATING" | "PENDING" | "PUBLISHED";

export interface IUser {
  id: string;
  name: string | null;
  description: string | null;
}

export interface IImage {
  imageId: string;
  zoneId: string;
  createdAt: Date;
  processed: boolean;
}

export interface IImpulseResponse {
  irId: string;
  zoneId: string;
  createdAt: Date;
  title: string;
  description: string | null;
  channelFormat: ZonesChannelFormat;
  speakerPositions: SpeakerPositions;
  processed: boolean;
}

export interface IZone {
  zoneId: string;
  createdAt: string;
  updatedAt: string;
  title: string;
  description: string | null;
  attribution: string | null;
  captureDate: string | null;
  coordinate: ICoordinate | null;
  spaceCategory: string | null;
  generationType: GenerationType | null;
  tags: string[];
  coverImageId: string | null;
  status: ZoneStatus;
  approvedBy: string | null;
  approvedAt: string | null;
  versionNumber: number;

  user: IUser;
  images: IImage[];
  irs: IImpulseResponse[];
}

export const toPositionMap = (
  speakerPositions: SpeakerPositions,
): PositionMap => {
  let pMap: PositionMap = {};
  if (speakerPositions == "CLR" || speakerPositions == "C")
    pMap.centre = "centre.wav";

  if (speakerPositions == "CLR" || speakerPositions == "LR") {
    pMap.left = "left.wav";
    pMap.right = "right.wav";
  }

  return pMap;
};
export const toChannelFormat = (
  channelFormat: ZonesChannelFormat,
): ChannelFormat => {
  switch (channelFormat) {
    case "STEREO":
      return "stereo";
    case "MONO":
      return "mono";
    case "FOA":
      return "foa";
  }
};

export const toImageMetadata = (
  zoneId: string,
  image: IImage,
): ImageMetadata => {
  return {
    imageId: image.imageId,
    imagePath: "",
  };
};

export const toIrMetadata = (ir: IImpulseResponse): IrMetadata => {
  return {
    channelFormat: toChannelFormat(ir.channelFormat),
    positionMap: toPositionMap(ir.speakerPositions),
    title: ir.title,
    irId: ir.irId,
    description: ir.description ?? undefined,
    relativePath: `impulse-responses/${ir.irId}`, // This doesn't appear to actually do anything for web-zones
  };
};

export const toZoneMetadata = (zone: IZone): ZoneMetadata => {
  return {
    zoneType: "web",
    title: zone.title || "No title?",
    description: zone.description ?? undefined,
    zoneId: zone.zoneId,
    images: zone.images.map((image) => toImageMetadata(zone.zoneId, image)),
    irs: zone.irs.map(toIrMetadata),
    coverImageId: zone.coverImageId ?? undefined,
    attribution: zone.attribution ?? undefined,
    captureDate: zone.captureDate ?? undefined,
    coordinate: zone.coordinate ?? undefined,
    spaceCategory: zone.spaceCategory ?? undefined,
    generationType: zone.generationType ?? undefined,
    tags: zone.tags,
    versionNumber: zone.versionNumber,
  };
};

export const getCachedWebZoneImageUrl = (zoneId: string, imageId: string) =>
  juce.getBackendResourceAddress(`web_zone_image/${zoneId}/${imageId}`);

const getUserZoneImageUrl = (zoneId: string, imageId: string) =>
  juce.getBackendResourceAddress(`resolve_user_image_url_here`);

// const resolveImageUrl = (
//   zone: ZoneMetadata,
//   image: ImageMetadata,
//   isCached: boolean,
// ) => {
//   switch (zone.zoneType) {
//     case "user":
//       return getUserZoneImageUrl("???", image.imageId);
//     case "web":
//       return isCached
//         ? getCachedWebZoneImageUrl(zone.zoneId, image.imageId)
//         : getImageUrl(zone.zoneId, image.imageId);
//   }
// };

export const resolveWebZoneImageUrl = (
  zoneId: string,
  imageId: string,
  isCached: boolean,
) => {
  return isCached
    ? getCachedWebZoneImageUrl(zoneId, imageId)
    : getImageUrl(zoneId, imageId);
};
