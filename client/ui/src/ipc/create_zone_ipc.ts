import { z } from "zod";

import { ZoneMetadata } from "@/hooks/zone_metadata";
import { juce } from "@/lib/juce";

export const CreateZoneSchema = z.object({
  path: z.string(),
  metadata: ZoneMetadata,
});

export type CreateZoneSchema = z.infer<typeof CreateZoneSchema>;

const createZoneNative = juce.getNativeFunction("import_zone_native");

export const createZone = async (createZoneSchema: CreateZoneSchema) => {
  await createZoneNative(JSON.stringify(createZoneSchema));
};
