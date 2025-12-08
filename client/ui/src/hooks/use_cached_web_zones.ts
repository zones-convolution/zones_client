import { useEffect, useState } from "react";

import { ZoneMetadata, ZoneMetadataOptional } from "@/hooks/zone_metadata";
import {
  getCachedWebZones,
  getCachedWebZone,
  cachedWebZoneUpdateListener,
} from "@/ipc/cached_web_zones_ipc";
import { engineUpdateListener } from "@/ipc/engine_ipc";

interface IUseCachedWebZones {
  cachedWebZones: ZoneMetadata[];
}

const useCachedWebZones = (): IUseCachedWebZones => {
  const [cachedWebZones, setCachedWebZones] = useState<ZoneMetadata[]>([]);

  useEffect(() => {
    getCachedWebZones().then(setCachedWebZones);
  }, []);

  return {
    cachedWebZones: cachedWebZones,
  };
};

interface IUseCachedWebZone {
  cachedWebZone: ZoneMetadataOptional;
}

const useCachedWebZone = (zoneId?: string): IUseCachedWebZone => {
  const [cachedWebZone, setCachedWebZone] = useState<ZoneMetadataOptional>({});

  useEffect(() => {
    if (zoneId) {
      getCachedWebZone(zoneId).then(setCachedWebZone);
      return cachedWebZoneUpdateListener(zoneId, setCachedWebZone);
    }
  }, [zoneId]);

  return {
    cachedWebZone: cachedWebZone,
  };
};

export { useCachedWebZones, useCachedWebZone };
