import { Loader, Search, Trash } from "lucide-react";
import React, { FC, useEffect, useState } from "react";
import {
  SearchBoxProps,
  useSearchBox,
  useInstantSearch,
} from "react-instantsearch";
import { useDebounceCallback } from "usehooks-ts";

import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";

const ZonesSearchBox: FC<SearchBoxProps> = (props) => {
  const { query, refine, clear } = useSearchBox({
    ...props,
  });
  const { status } = useInstantSearch();

  const [value, setValue] = useState<string>(query);
  const setDebounce = useDebounceCallback(refine, 100);

  useEffect(() => {
    setValue(query);
  }, [query]);

  const setQuery = (queryValue: string) => {
    setValue(queryValue);
    setDebounce(queryValue);
  };

  return (
    <div className="flex flex-row gap-2 items-center">
      {status == "stalled" ? (
        <Loader className="w-4 h-4 animate-spin" />
      ) : (
        <Search className="w-4 h-4" />
      )}
      <Input
        value={value}
        onChange={(event) => setQuery(event.target.value)}
        placeholder="Search for Zones"
      />
      <Button onClick={clear} aria-label="Clear search">
        <Trash className="w-4 h-4" />
      </Button>
    </div>
  );
};

export { ZonesSearchBox };
