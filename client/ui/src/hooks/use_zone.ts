import useSWR, { mutate } from "swr";

import { fetcher } from "@/lib/api";
import { IImage, IImpulseResponse, IZone } from "@/lib/zones";

export interface IUseZone {
  zone: IZone;
  irs: IImpulseResponse[];
  images: IImage[];
}

const getZoneUrl = (zoneId: string) => `/zones/${zoneId}`;

const fetchZoneAndMutate = async (zoneId: string) => {
  const zoneUrl = getZoneUrl(zoneId);
  const data = (await fetcher(zoneUrl)) as IUseZone;
  await mutate(zoneUrl, data);
  return data;
};

const useZone = (zoneId: string) => {
  return useSWR<IUseZone>(getZoneUrl(zoneId));
};

export { useZone, fetchZoneAndMutate };
