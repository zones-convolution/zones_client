import { instantMeiliSearch } from "@meilisearch/instant-meilisearch";
import {
  Hits,
  HitsPerPage,
  InfiniteHits,
  InstantSearch,
  Pagination,
} from "react-instantsearch";

import { ZonesHits } from "@/components/zones_hits";
import { ZonesSearchBox } from "@/components/zones_search_box";
import { Config } from "@/lib/config";

const { searchClient } = instantMeiliSearch(
  Config.SEARCH_HOST,
  Config.SEARCH_PUBLIC_KEY,
);

const Search = () => (
  <InstantSearch indexName="zones" searchClient={searchClient}>
    <ZonesSearchBox />
    {/*<HitsPerPage*/}
    {/*  items={[*/}
    {/*    { label: "8 hits per page", value: 8, default: true },*/}
    {/*    { label: "16 hits per page", value: 16 },*/}
    {/*  ]}*/}
    {/*/>*/}
    <ZonesHits />
    <Pagination
      classNames={{ root: "flex justify-center", list: "flex flex-row gap-8" }}
    />
  </InstantSearch>
);

export default Search;
