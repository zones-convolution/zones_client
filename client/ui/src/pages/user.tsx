import { ZonesHits } from "@/components/search/zones_hits";
import { ZonesPaginationWithLinks } from "@/components/search/zones_pagination";
import { ZonesSearchBox } from "@/components/search/zones_search_box";
import {
  SearchProvider,
  useUserZoneRepository,
} from "@/context/search_context";

const User = () => {
  return (
    <div className="flex min-h-full flex-col gap-4">
      <SearchProvider repository={useUserZoneRepository}>
        <ZonesSearchBox />
        <ZonesHits />
        <ZonesPaginationWithLinks pageSizeOptions={[10, 15, 20]} />
      </SearchProvider>
    </div>
  );
};

export default User;
