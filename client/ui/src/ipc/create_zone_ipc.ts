import { z } from "zod";

import { ZoneMetadata } from "@/hooks/zone_metadata";
import { juce } from "@/lib/juce";

export const CreateZoneSchema = z.object({
  path: z.string(),
  metadata: ZoneMetadata,
});

export type CreateZoneSchema = z.infer<typeof CreateZoneSchema>;

const createZoneNative = juce.getNativeFunction("import_zone_native");

const handleCreateZone = (data: any) => {
  try {
    return JSON.parse(data);
  } catch (err) {
    console.error("Failed to parse CreateZoneIPC!", err);
  }

  return false;
};

export const createZone = async (createZoneSchema: CreateZoneSchema) => {
  return handleCreateZone(
    await createZoneNative(JSON.stringify(createZoneSchema)),
  );
};

const getUserZonesNative = juce.getNativeFunction("get_user_zones_native");
