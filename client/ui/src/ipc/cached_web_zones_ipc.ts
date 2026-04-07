import { z } from "zod";

import { ZoneMetadata, ZoneMetadataOptional } from "@/hooks/zone_metadata";
import {
  addNativeEventListener,
  juce,
  removeNativeEventListener,
} from "@/lib/juce";
import { withAbort } from "@/lib/abortable";

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

export const getCachedWebZones = async (options?: { signal?: AbortSignal }) => {
  return handleReceiveCachedWebZones(
    await withAbort(getCachedWebZonesNative(), options?.signal),
  );
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

export const getCachedWebZone = async (
  zoneId: string,
  options?: { signal?: AbortSignal },
) => {
  return handleReceiveCachedWebZoneOptional(
    await withAbort(getCachedWebZoneNative(JSON.stringify(zoneId)), options?.signal),
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
