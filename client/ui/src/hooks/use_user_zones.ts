import { useEffect, useState } from "react";

import { ZoneMetadata } from "@/hooks/zone_metadata";
import { getUserZones } from "@/ipc/user_zones_ipc";
import { isAbortError } from "@/lib/abortable";

interface IUseUserZones {
  userZones: ZoneMetadata[];
}

const useUserZones = (): IUseUserZones => {
  const [userZones, setUserZones] = useState<ZoneMetadata[]>([]);

  useEffect(() => {
    const controller = new AbortController();

    getUserZones({ signal: controller.signal })
      .then(setUserZones)
      .catch((error) => {
        if (!isAbortError(error)) console.error(error);
      });

    return () => {
      controller.abort();
    };
  }, []);

  return {
    userZones: userZones,
  };
};

export { useUserZones };
