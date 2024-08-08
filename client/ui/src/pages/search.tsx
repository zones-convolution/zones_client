import { instantMeiliSearch } from "@meilisearch/instant-meilisearch";
import { InstantSearch } from "react-instantsearch";

import { ZonesHits } from "@/components/zones_hits";
import { ZonesSearchBox } from "@/components/zones_search_box";
import { Config } from "@/lib/config";

const { searchClient } = instantMeiliSearch(
  Config.SEARCH_HOST,
  Config.SEARCH_PUBLIC_KEY,
);

const Search = () => (
  <div className="flex flex-col gap-4 bg-card h-full p-4">
    <InstantSearch indexName="zones" searchClient={searchClient}>
      <ZonesSearchBox />
      <ZonesHits />
    </InstantSearch>
  </div>
);

export default Search;
