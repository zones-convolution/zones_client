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

import { Config } from "@/lib/config";
import { IZone } from "@/lib/zones";

export interface ISearchZones {
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

export const SearchProvider: FC<{
  children: ReactNode;
}> = ({ children }) => {
  const [params, setParams] = useState<ISearchParams>({
    page: 1,
    pageSize: 10,
    search: null,
  });

  const { data, isLoading } = useSWR<ISearchZones>(
    `${Config.API_BASE_URL}/zones?page=${params.page}&pageSize=${params.pageSize}&search=${params.search ?? ""}`,
  );

  return (
    <SearchContext.Provider
      value={{
        search: data ?? { count: 0, data: [] },
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
