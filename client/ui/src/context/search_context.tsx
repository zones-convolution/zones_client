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

const UserZoneRepository = (params: ISearchParams) => {
  const { userZones } = useUserZones();
  const { cachedWebZones } = useCachedWebZones();

  const mergedZones = [...userZones, ...cachedWebZones].sort((a, b) =>
    a.title.localeCompare(b.title),
  );
};

const WebZoneRepository = (params: ISearchParams) => {
  const { data, isLoading } = useSWR<ISearchWebZones>(
    `${Config.API_BASE_URL}/zones?page=${params.page}&pageSize=${params.pageSize}&search=${params.search ?? ""}`,
  );
};

export const SearchProvider: FC<{
  children: ReactNode;
}> = ({ children }) => {
  const [params, setParams] = useState<ISearchParams>({
    page: 1,
    pageSize: 10,
    search: null,
  });

  const { data, isLoading } = useSWR<ISearchWebZones>(
    `${Config.API_BASE_URL}/zones?page=${params.page}&pageSize=${params.pageSize}&search=${params.search ?? ""}`,
  );

  return (
    <SearchContext.Provider
      value={{
        search: data ? mapWebSearch(data) : { count: 0, data: [] },
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
