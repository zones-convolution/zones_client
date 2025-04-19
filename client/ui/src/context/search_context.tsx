import { createContext, FC, ReactNode, useContext, useState } from "react";
import useSWR from "swr";

import { Config } from "@/lib/config";
import { IZone } from "@/lib/zones";

export interface ISearchZones {
  data: IZone[];
  count: number;
}

interface ISearchContext {
  search: ISearchZones;
}

const SearchContext = createContext<ISearchContext | null>(null);

export const SearchProvider: FC<{
  children: ReactNode;
}> = ({ children }) => {
  const [page, setPage] = useState<number>(1);
  const [pageSize, setPageSize] = useState<number>(10);

  const { data } = useSWR<ISearchZones>(`${Config.API_BASE_URL}/zones`);

  return (
    <SearchContext.Provider value={{ search: data ?? { count: 0, data: [] } }}>
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
