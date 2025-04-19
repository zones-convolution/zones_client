import { ZonesHits } from "@/components/zones_hits";
import { SearchProvider } from "@/context/search_context";

const Search = () => (
  <SearchProvider>
    <ZonesHits />
  </SearchProvider>
);

export default Search;
