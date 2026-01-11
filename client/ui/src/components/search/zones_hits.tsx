import { Search } from "lucide-react";
import React, { FC } from "react";

import { Skeleton } from "@/components/ui/skeleton";

import { ZoneCard } from "@/components/cards/zone_card";
import { useNavigation } from "@/context/browser_context";
import { useLoadContext } from "@/context/load_context";
import { useSearchContext } from "@/context/search_context";
import { useCachedWebZone } from "@/hooks/use_cached_web_zones";
import { useValidTargetFormats } from "@/hooks/use_valid_target_formats";
import { doesZoneMatchSelection, getDefaultIrSelection } from "@/lib/irs";
import { getImageUrl } from "@/lib/s3_resources";
import { IZone, resolveWebZoneImageUrl, toZoneMetadata } from "@/lib/zones";

const ZonesSearchHit: FC<{
  zone: IZone;
}> = ({ zone }) => {
  const zoneMetadata = toZoneMetadata(zone);
  const { navigateToZone } = useNavigation();
  const { load, loadingIr, currentIr } = useLoadContext();
  const { validTargetFormats } = useValidTargetFormats();
  let isLoadingZone = doesZoneMatchSelection(zoneMetadata, loadingIr);
  let isCurrentZone = doesZoneMatchSelection(zoneMetadata, currentIr);
  let defaultIrSelection = getDefaultIrSelection(
    zoneMetadata,
    validTargetFormats,
  );
  const { isCached } = useCachedWebZone(zone.zoneId);
  const imageUrl = resolveWebZoneImageUrl(
    zone.zoneId,
    zone.coverImageId!,
    isCached,
  );

  return (
    <ZoneCard
      user={zone.user}
      category={zone.title}
      imageUrl={imageUrl}
      rt60={1.2}
      loading={isLoadingZone}
      disabled={isCurrentZone}
      canLoad={defaultIrSelection != undefined}
      onView={() => navigateToZone(zone)}
      onLoad={async () => {
        if (defaultIrSelection) await load(defaultIrSelection);
      }}
      isCached={isCached}
    />
  );
};

const ZonesNoHits = () => {
  return (
    <div className="py-10 px-6 text-center">
      <div className="inline-block">
        <div className="flex justify-center items-center rounded-full h-20 w-20 bg-background p-2 text-foreground">
          <Search className="w-8 h-8 text-white" />
        </div>
      </div>
      <h2 className="mt-6 mb-2 text-2xl">No Zones Found</h2>
      <span className="text-secondary">
        This search had no matching Zones. Please try modifying your search
        term.
      </span>
    </div>
  );
};

const ZoneCardSkeleton = () => {
  return (
    <div className="flex items-center space-x-4">
      <Skeleton className="h-12 w-12 rounded-full" />
      <div className="space-y-2 flex-grow">
        <Skeleton className="h-4 w-[80%]" />
        <Skeleton className="h-4 w-full" />
      </div>
    </div>
  );
};
const ZonesLoading = () => {
  return (
    <div className="py-10 px-6 grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-8">
      <ZoneCardSkeleton />
      <ZoneCardSkeleton />
      <ZoneCardSkeleton />
      <ZoneCardSkeleton />
      <ZoneCardSkeleton />
      <ZoneCardSkeleton />
    </div>
  );
};

const ZonesHits = () => {
  const { search, isLoading } = useSearchContext();

  if (isLoading) return <ZonesLoading />;
  if (search.count === 0) return <ZonesNoHits />;

  return (
    <div className="flex flex-col gap-4">
      <div className="grid grid-cols-1 md:grid-cols-3 xl:grid-cols-4 gap-4">
        {search.data.map((zone) => {
          return (
            <div className="h-[200px]" key={zone.zoneId}>
              <ZonesSearchHit zone={zone} />
            </div>
          );
        })}
      </div>
    </div>
  );
};

export { ZonesHits };
