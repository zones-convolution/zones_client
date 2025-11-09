import { z } from "zod";

import { ZoneMetadata, ZoneMetadataOptional } from "@/hooks/zone_metadata";
import { juce } from "@/lib/juce";

const getCachedWebZonesNative = juce.getNativeFunction(
  "get_cached_web_zones_native",
);

const handleReceiveCachedWebZones = (data: any) => {
  try {
    return z.array(ZoneMetadata).parse(JSON.parse(data));
  } catch (err) {
    console.error("Failed to parse WebZonesIPC!", err);
  }

  return [];
};

export const getCachedWebZones = async () => {
  return handleReceiveCachedWebZones(await getCachedWebZonesNative());
};

const getCachedWebZoneNative = juce.getNativeFunction(
  "get_cached_web_zone_native",
);

const handleReceiveCachedWebZone = (data: any): ZoneMetadataOptional => {
  try {
    return ZoneMetadataOptional.parse(JSON.parse(data));
  } catch (err) {
    console.error("Failed to parse WebZoneIPC!", err);
  }

  return {};
};

export const getCachedWebZone = async (zoneId: string) => {
  return handleReceiveCachedWebZone(
    await getCachedWebZoneNative(JSON.stringify(zoneId)),
  );
};
