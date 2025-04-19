import { Loader, Search, Trash } from "lucide-react";
import React, { useCallback, useState } from "react";
import { useDebounceCallback } from "usehooks-ts";

import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";

import { useSearchContext } from "@/context/search_context";

const ZonesSearchBox = () => {
  const { params, setParams, isLoading } = useSearchContext();

  const [value, setValue] = useState<string | null>(params.search);

  const updateSearchParams = useCallback((query: string) => {
    setParams((p) => ({ ...params, search: query }));
  }, []);
  const setDebounce = useDebounceCallback(updateSearchParams, 400);

  const setQuery = (queryValue: string) => {
    setValue(queryValue);
    setDebounce(queryValue);
  };

  const clear = () => {
    setParams((p) => ({ ...p, search: null }));
    setValue(null);
  };

  return (
    <div className="flex flex-row gap-2 items-center">
      {isLoading ? (
        <Loader className="w-4 h-4 animate-spin" />
      ) : (
        <Search className="w-4 h-4" />
      )}
      <Input
        value={value ?? ""}
        onChange={(event) => setQuery(event.target.value)}
        placeholder="Search for Zones"
      />
      <Button onClick={clear} aria-label="Clear">
        <Trash className="w-4 h-4" />
      </Button>
    </div>
  );
};

export { ZonesSearchBox };
