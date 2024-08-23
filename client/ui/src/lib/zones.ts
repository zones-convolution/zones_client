import {
  ImageMetadata,
  IrMetadata,
  ZoneMetadata,
  ChannelFormat,
  PositionMap,
} from "@/hooks/zone_metadata";

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

export interface IZone {
  zoneId: string;
  createdAt: Date;
  updatedAt: Date;
  authorId: string;
  title?: string;
  description?: string;
  captureDate?: Date;
  coordinate?: ICoordinate;
  spaceCategory?: string;
  generationType?: GenerationType;
  tags?: string[];
  coverImageId?: string;
  status: ZoneStatus;
  approvedBy?: string;
  approvedAt?: Date;
  versionNumber: number;
}

export interface IImage {
  imageId: string;
  zoneId: string;
  createdAt: Date;
  ttl?: number;
}

export interface IImpulseResponse {
  irId: string;
  zoneId: string;
  createdAt: Date;
  title: string;
  description?: string;
  ttl?: number;
  channelFormat: ZonesChannelFormat;
  speakerPositions: SpeakerPositions;
}

export type IndexedIr = Pick<
  IImpulseResponse,
  "irId" | "title" | "channelFormat" | "speakerPositions" | "description"
>;

export type IndexedZone = Pick<
  IZone,
  | "zoneId"
  | "createdAt"
  | "updatedAt"
  | "title"
  | "description"
  | "captureDate"
  | "coordinate"
  | "spaceCategory"
  | "generationType"
  | "tags"
  | "coverImageId"
  | "versionNumber"
>;

export type IndexedImage = Pick<IImage, "imageId" | "createdAt">;

export type ZoneSearchHit = IndexedZone & { irs: IndexedIr[] };

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

export const toImageMetadata = (image: IImage): ImageMetadata => {
  return {
    imageId: image.imageId,
    imagePath: image.zoneId, // This is wrong
  };
};

export const toIrMetadata = (ir: IImpulseResponse): IrMetadata => {
  return {
    channelFormat: toChannelFormat(ir.channelFormat),
    positionMap: toPositionMap(ir.speakerPositions),
    title: ir.title,
    irId: ir.irId,
    description: ir.description,
    relativePath: `impulse-responses/${ir.irId}`,
  };
};

export const toZoneMetadata = (
  zone: IZone,
  images: IImage[],
  irs: IImpulseResponse[],
): ZoneMetadata => {
  return {
    zoneType: "web",
    title: zone.title || "No title?",
    description: zone.description,
    zoneId: zone.zoneId,
    images: images.map(toImageMetadata),
    irs: irs.map(toIrMetadata),
    coverImageId: zone.coverImageId,
  };
};
