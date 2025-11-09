import { z } from "zod";

const ZoneType = z.enum(["web", "user"]);
const ChannelFormat = z.enum(["mono", "stereo", "foa", "quadraphonic"]);

type ChannelFormat = z.infer<typeof ChannelFormat>;

const TargetFormat = z.enum([
  "mono",
  "stereo",
  "trueStereo",
  "foa",
  "quadraphonic",
]);

type TargetFormat = z.infer<typeof TargetFormat>;

const PositionMap = z.object({
  centre: z.string().optional(),
  left: z.string().optional(),
  right: z.string().optional(),
});

type PositionMap = z.infer<typeof PositionMap>;

const IrMetadata = z.object({
  channelFormat: ChannelFormat.optional(),
  positionMap: PositionMap.optional(),
  title: z.string().optional(),
  irId: z.string().optional(),
  description: z.string().optional(),
  relativePath: z.string(),
});

type IrMetadata = z.infer<typeof IrMetadata>;

const ImageMetadata = z.object({
  imageId: z.string(),
  imagePath: z.string(),
});

type ImageMetadata = z.infer<typeof ImageMetadata>;

const ZoneMetadata = z.object({
  zoneType: ZoneType,
  title: z.string(),
  zoneId: z.string().optional(),
  description: z.string().optional(),
  images: z.array(ImageMetadata),
  coverImageId: z.string().optional(),
  irs: z.array(IrMetadata),
  pathAttribute: z.string().optional(),
  attribution: z.string().optional(),
  captureDate: z.string().optional(),
  coordinate: z
    .object({
      latitude: z.number().optional(),
      longitude: z.number().optional(),
    })
    .optional(),
  spaceCategory: z.string().optional(),
  generationType: z.string().optional(),
  tags: z.array(z.string()).optional(),
  versionNumber: z.number().optional(),
});

type ZoneMetadata = z.infer<typeof ZoneMetadata>;

const ZoneMetadataOptional = z.object({
  zoneMetadata: ZoneMetadata.optional(),
});

type ZoneMetadataOptional = z.infer<typeof ZoneMetadataOptional>;

const IrSelection = z.object({
  zone: ZoneMetadata,
  ir: IrMetadata,
  targetFormat: TargetFormat,
});

type IrSelection = z.infer<typeof IrSelection>;

const IrSelectionOptional = z.object({
  irSelection: IrSelection.optional(),
});

type IrSelectionOptional = z.infer<typeof IrSelectionOptional>;

export {
  ZoneMetadata,
  ZoneMetadataOptional,
  ImageMetadata,
  IrMetadata,
  PositionMap,
  ChannelFormat,
  IrSelection,
  IrSelectionOptional,
  TargetFormat,
};
