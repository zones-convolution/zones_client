import { ZoneCard } from "@/components/cards/zone_card";
import { ZonesHits, ZonesSearchHit } from "@/components/search/zones_hits";
import { ZonesPaginationWithLinks } from "@/components/search/zones_pagination";
import { ZonesSearchBox } from "@/components/search/zones_search_box";
import { useNavigation } from "@/context/browser_context";
import { useLoadContext } from "@/context/load_context";
import { SearchProvider } from "@/context/search_context";
import { useCachedWebZones } from "@/hooks/use_cached_web_zones";
import { useUserZones } from "@/hooks/use_user_zones";
import { useValidTargetFormats } from "@/hooks/use_valid_target_formats";
import { doesZoneMatchSelection, getDefaultIrSelection } from "@/lib/irs";
import { getUserZoneGradient } from "@/lib/user_zones";

const User = () => {
  const { userZones } = useUserZones();
  const { cachedWebZones } = useCachedWebZones();

  const mergedZones = [...userZones, ...cachedWebZones].sort((a, b) =>
    a.title.localeCompare(b.title),
  );

  return (
    <div className="flex flex-col gap-4 bg-card h-full p-4 overflow-scroll">
      <SearchProvider>
        <ZonesSearchBox />
        <ZonesHits />
        <ZonesPaginationWithLinks pageSizeOptions={[10, 15, 20]} />
      </SearchProvider>
    </div>
  );
};

export default User;
