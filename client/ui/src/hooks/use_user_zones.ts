import { useEffect, useState } from "react";
import { z } from "zod";

import { ZoneMetadata } from "@/hooks/zone_metadata";
import { juce } from "@/lib/juce";

interface IUseUserZones {
  userZones: ZoneMetadata[];
}

const getUserZonesNative = juce.getNativeFunction("get_user_zones_native");

const useUserZones = (): IUseUserZones => {
  const [userZones, setUserZones] = useState<ZoneMetadata[]>([]);

  const getUserZones = async () => {
    return getUserZonesNative();
  };

  useEffect(() => {
    getUserZones().then(handleReceiveUserZones);
  }, []);

  const handleReceiveUserZones = (data: any) => {
    try {
      setUserZones(z.array(ZoneMetadata).parse(JSON.parse(data)));
    } catch (err) {
      console.error("Failed to parse user zones!", err);
    }
  };

  return {
    userZones: userZones,
  };
};

export { useUserZones };
