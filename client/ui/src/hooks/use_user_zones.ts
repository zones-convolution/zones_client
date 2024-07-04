import { useEffect, useState } from "react";

import { ZoneMetadata } from "@/hooks/zone_metadata";
import { getUserZones } from "@/ipc/user_zones_ipc";

interface IUseUserZones {
  userZones: ZoneMetadata[];
}

const useUserZones = (): IUseUserZones => {
  const [userZones, setUserZones] = useState<ZoneMetadata[]>([]);

  useEffect(() => {
    getUserZones().then(setUserZones);
  }, []);

  return {
    userZones: userZones,
  };
};

export { useUserZones };
