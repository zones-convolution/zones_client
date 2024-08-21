import useSWR from "swr";

import { IImage, IImpulseResponse, IZone } from "@/lib/zones";

export interface IUseZone {
  zone: IZone;
  irs: IImpulseResponse[];
  images: IImage[];
}

const useZone = (zoneId: string) => {
  return useSWR<IUseZone>(`/zones/${zoneId}`);
};

export { useZone };
