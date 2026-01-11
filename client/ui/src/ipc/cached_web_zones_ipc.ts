import { z } from "zod";

import { ZoneMetadata, ZoneMetadataOptional } from "@/hooks/zone_metadata";
import {
  addNativeEventListener,
  juce,
  removeNativeEventListener,
} from "@/lib/juce";

const onCachedWebZoneUpdated = "on_cached_web_zone_updated";

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

const handleReceiveCachedWebZoneOptional = (
  data: any,
): ZoneMetadataOptional => {
  try {
    return ZoneMetadataOptional.parse(JSON.parse(data));
  } catch (err) {
    console.error("Failed to parse WebZoneIPC!", err);
  }

  return {};
};

const handleReceiveCachedWebZone = (data: any): ZoneMetadataOptional => {
  try {
    return { zoneMetadata: ZoneMetadata.parse(JSON.parse(data)) };
  } catch (err) {
    console.error("Failed to parse WebZoneIPC!", err);
  }

  return {};
};

export const getCachedWebZone = async (zoneId: string) => {
  return handleReceiveCachedWebZoneOptional(
    await getCachedWebZoneNative(JSON.stringify(zoneId)),
  );
};

export const cachedWebZoneUpdateListener = (
  zoneId: string,
  onUpdate: (cachedWebZone: ZoneMetadataOptional) => void,
) => {
  const listener = addNativeEventListener(
    onCachedWebZoneUpdated,
    (data: any) => {
      const metadata = handleReceiveCachedWebZone(data);
      if (metadata.zoneMetadata && metadata.zoneMetadata.zoneId == zoneId) {
        onUpdate(metadata);
      }
    },
  );

  return () => {
    removeNativeEventListener(listener);
  };
};
