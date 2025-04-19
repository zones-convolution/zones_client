import { ZonesHits } from "@/components/search/zones_hits";
import { ZonesPaginationWithLinks } from "@/components/search/zones_pagination";
import { ZonesSearchBox } from "@/components/search/zones_search_box";
import { SearchProvider } from "@/context/search_context";

const Search = () => (
  <SearchProvider>
    <ZonesSearchBox />
    <ZonesHits />
    <ZonesPaginationWithLinks pageSizeOptions={[10, 15, 20]} />
  </SearchProvider>
);

export default Search;
