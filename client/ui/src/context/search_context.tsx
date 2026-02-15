import {
  createContext,
  Dispatch,
  FC,
  ReactNode,
  SetStateAction,
  useContext,
  useState,
} from "react";
import useSWR from "swr";

import { useCachedWebZones } from "@/hooks/use_cached_web_zones";
import { useUserZones } from "@/hooks/use_user_zones";
import { ZoneMetadata } from "@/hooks/zone_metadata";
import { Config } from "@/lib/config";
import { IUser, IZone, toZoneMetadata } from "@/lib/zones";

export interface ISearchZone {
  zone: ZoneMetadata;
  user?: IUser;
}
export interface ISearchZones {
  data: ISearchZone[];
  count: number;
}
export interface ISearchWebZones {
  data: IZone[];
  count: number;
}

export interface ISearchParams {
  page: number;
  pageSize: number;
  search: string | null;
}

export interface ISearchRepositoryResult {
  search: ISearchZones;
  isLoading: boolean;
}
export type SearchRepository = (
  params: ISearchParams,
) => ISearchRepositoryResult;

interface ISearchContext {
  search: ISearchZones;
  params: ISearchParams;
  setParams: Dispatch<SetStateAction<ISearchParams>>;
  isLoading: boolean;
}

const SearchContext = createContext<ISearchContext | null>(null);

const mapIZoneToSearch = (zone: IZone): ISearchZone => {
  return { zone: toZoneMetadata(zone), user: zone.user };
};
const mapWebSearch = (webZoneSearch: ISearchWebZones): ISearchZones => {
  return {
    data: webZoneSearch.data.map(mapIZoneToSearch),
    count: webZoneSearch.count,
  };
};

export const useUserZoneRepository: SearchRepository = (
  params: ISearchParams,
) => {
  const { userZones } = useUserZones();
  const { cachedWebZones } = useCachedWebZones();

  const mergedMap = new Map<string | number, ZoneMetadata>();
  for (const z of [...userZones, ...cachedWebZones]) {
    const key = (z as any).id ?? z.title;
    if (!mergedMap.has(key)) mergedMap.set(key, z);
  }
  const mergedZones = Array.from(mergedMap.values()).sort((a, b) =>
    a.title.localeCompare(b.title),
  );

  const query = (params.search ?? "").trim().toLowerCase();
  const filtered = query
    ? mergedZones.filter((z) => z.title.toLowerCase().includes(query))
    : mergedZones;

  const start = Math.max(0, (params.page - 1) * params.pageSize);
  const end = start + params.pageSize;
  const pageData = filtered.slice(start, end);

  return {
    search: {
      data: pageData.map((zone) => ({
        zone: zone,
      })),
      count: filtered.length,
    },
    isLoading: false,
  };
};

export const useWebZoneRepository: SearchRepository = (
  params: ISearchParams,
) => {
  const { data, isLoading } = useSWR<ISearchWebZones>(
    `${Config.API_BASE_URL}/zones?page=${params.page}&pageSize=${params.pageSize}&search=${params.search ?? ""}`,
  );

  return {
    search: data ? mapWebSearch(data) : { count: 0, data: [] },
    isLoading,
  };
};

export const SearchProvider: FC<{
  children: ReactNode;
  repository?: SearchRepository;
}> = ({ children, repository = useWebZoneRepository }) => {
  const [params, setParams] = useState<ISearchParams>({
    page: 1,
    pageSize: 10,
    search: null,
  });

  const { search, isLoading } = repository(params);

  return (
    <SearchContext.Provider
      value={{
        search,
        params,
        setParams,
        isLoading,
      }}
    >
      {children}
    </SearchContext.Provider>
  );
};
export const useSearchContext = () => {
  const context = useContext(SearchContext);
  if (!context)
    throw new Error(
      "No SearchContext.Provider found when calling useSearchContext.",
    );

  return context;
};
