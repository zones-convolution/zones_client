import useSWR from "swr";

import { IndexedImage, IndexedIr, IndexedZone } from "@/lib/zones";

export interface IZone {
  zone: IndexedZone;
  irs: IndexedIr[];
  images: IndexedImage[];
}

const useZone = (zoneId: string) => {
  return useSWR<IZone>(`/zones/${zoneId}`);
};

export { useZone };
