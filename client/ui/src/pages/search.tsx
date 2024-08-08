import { instantMeiliSearch } from "@meilisearch/instant-meilisearch";
import type { Hit } from "instantsearch.js";
import React, { FC } from "react";
import { InfiniteHits, InstantSearch, SearchBox } from "react-instantsearch";

import {
  Card,
  CardContent,
  CardDescription,
  CardHeader,
  CardTitle,
} from "@/components/ui/card";

import { getImageUrl } from "@/lib/s3_resources";

const { searchClient } = instantMeiliSearch(
  "https://search.zonesconvolution.com",
  "ccb867a586459db3056e2f92ab53ee33a69340ddc626b38243c3aa1b756f66e1",
);

const ZoneHit: FC<{ hit: Hit<any> }> = ({ hit }) => {
  return (
    <Card key={hit.zoneId}>
      <CardHeader>
        <CardTitle>{hit.title}</CardTitle>
        <CardDescription>{hit.description}</CardDescription>
      </CardHeader>
      <CardContent>
        <img
          src={getImageUrl(hit.zoneId, hit.coverImageId)}
          alt={hit.title}
          className="h-60 w-full object-cover"
        />
      </CardContent>
    </Card>
  );
};

const Search = () => (
  <div className="flex flex-col gap-4 bg-card h-full p-4">
    <InstantSearch indexName="zones" searchClient={searchClient}>
      <SearchBox />
      <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3">
        <InfiniteHits hitComponent={ZoneHit} />
      </div>
    </InstantSearch>
  </div>
);

export default Search;
