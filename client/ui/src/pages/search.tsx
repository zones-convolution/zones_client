import { instantMeiliSearch } from "@meilisearch/instant-meilisearch";
import { InstantSearch } from "react-instantsearch";

import { ZonesHits } from "@/components/zones_hits";
import { ZonesSearchBox } from "@/components/zones_search_box";

const { searchClient } = instantMeiliSearch(
  "https://search.zonesconvolution.com",
  "ccb867a586459db3056e2f92ab53ee33a69340ddc626b38243c3aa1b756f66e1",
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
