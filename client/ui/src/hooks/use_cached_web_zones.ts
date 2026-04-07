import { useEffect, useState } from "react";

import { ZoneMetadata, ZoneMetadataOptional } from "@/hooks/zone_metadata";
import {
  getCachedWebZones,
  getCachedWebZone,
  cachedWebZoneUpdateListener,
} from "@/ipc/cached_web_zones_ipc";
import { isAbortError } from "@/lib/abortable";

interface IUseCachedWebZones {
  cachedWebZones: ZoneMetadata[];
}

const useCachedWebZones = (): IUseCachedWebZones => {
  const [cachedWebZones, setCachedWebZones] = useState<ZoneMetadata[]>([]);

  useEffect(() => {
    const controller = new AbortController();

    getCachedWebZones({ signal: controller.signal })
      .then(setCachedWebZones)
      .catch((error) => {
        if (!isAbortError(error)) console.error(error);
      });

    return () => {
      controller.abort();
    };
  }, []);

  return {
    cachedWebZones: cachedWebZones,
  };
};

interface IUseCachedWebZone {
  cachedWebZone: ZoneMetadataOptional;
  isCached: boolean;
}

const useCachedWebZone = (zoneId?: string): IUseCachedWebZone => {
  const [cachedWebZone, setCachedWebZone] = useState<ZoneMetadataOptional>({});

  useEffect(() => {
    if (!zoneId) {
      setCachedWebZone({});
      return;
    }

    const controller = new AbortController();
    const unsubscribe = cachedWebZoneUpdateListener(zoneId, setCachedWebZone);

    getCachedWebZone(zoneId, { signal: controller.signal })
      .then(setCachedWebZone)
      .catch((error) => {
        if (!isAbortError(error)) console.error(error);
      });

    return () => {
      controller.abort();
      unsubscribe();
    };
  }, [zoneId]);

  return {
    cachedWebZone: cachedWebZone,
    isCached: cachedWebZone.zoneMetadata != undefined,
  };
};

export { useCachedWebZones, useCachedWebZone };
