import { ZonesHits } from "@/components/search/zones_hits";
import { ZonesPaginationWithLinks } from "@/components/search/zones_pagination";
import { ZonesSearchBox } from "@/components/search/zones_search_box";
import { SearchProvider, useWebZoneRepository } from "@/context/search_context";

const Browser = () => {
  return (
    <div className="flex flex-col gap-4 bg-card h-full p-4 overflow-scroll">
      <SearchProvider repository={useWebZoneRepository}>
        <ZonesSearchBox />
        <ZonesHits />
        <ZonesPaginationWithLinks pageSizeOptions={[10, 15, 20]} />
      </SearchProvider>
    </div>
  );
};

export default Browser;
