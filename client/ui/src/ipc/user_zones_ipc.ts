import { z } from "zod";

import { ZoneMetadata } from "@/hooks/zone_metadata";
import { withAbort } from "@/lib/abortable";
import { juce } from "@/lib/juce";

const getUserZonesNative = juce.getNativeFunction("get_user_zones_native");

const handleReceiveUserZones = (data: any) => {
  try {
    return z.array(ZoneMetadata).parse(JSON.parse(data));
  } catch (err) {
    console.error("Failed to parse UserZonesIPC!", err);
  }

  return [];
};

export const getUserZones = async (options?: { signal?: AbortSignal }) => {
  return handleReceiveUserZones(
    await withAbort(getUserZonesNative(), options?.signal),
  );
};
