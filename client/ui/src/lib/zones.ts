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

export const ChannelFormats = ["MONO", "STEREO", "FOA"] as const;
export type ChannelFormat = (typeof ChannelFormats)[number];

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
  channelFormat: ChannelFormat;
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
> & { irs: IndexedIr[] };
