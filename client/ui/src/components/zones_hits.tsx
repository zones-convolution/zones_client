import { Hit } from "instantsearch.js";
import { Search } from "lucide-react";
import React, { FC } from "react";
import { useHits, useInstantSearch } from "react-instantsearch";

import { Skeleton } from "@/components/ui/skeleton";

import { ZoneCard } from "@/components/cards/zone_card";
import { useNavigation } from "@/context/browser_context";
import { useLoadContext } from "@/context/load_context";
import { useValidTargetFormats } from "@/hooks/use_valid_target_formats";
import { doesZoneMatchSelection, getDefaultIrSelection } from "@/lib/irs";
import { getImageUrl } from "@/lib/s3_resources";
import { toZoneMetadata, ZoneSearchHit } from "@/lib/zones";

const ZonesSearchHit: FC<{
  hit: Hit<ZoneSearchHit>;
}> = ({ hit }) => {
  const zoneMetadata = toZoneMetadata(hit, hit.images, hit.irs);
  const imageUrl = getImageUrl(hit.zoneId, hit.coverImageId!);
  const { navigateToZone } = useNavigation();
  const { load, loadingIr, currentIr } = useLoadContext();
  const { validTargetFormats } = useValidTargetFormats();
  let isLoadingZone = doesZoneMatchSelection(zoneMetadata, loadingIr);
  let isCurrentZone = doesZoneMatchSelection(zoneMetadata, currentIr);
  let defaultIrSelection = getDefaultIrSelection(
    zoneMetadata,
    validTargetFormats,
  );

  return (
    <ZoneCard
      category={hit.title!}
      imageUrl={imageUrl}
      rt60={1.2}
      loading={isLoadingZone}
      disabled={isCurrentZone}
      canLoad={defaultIrSelection != undefined}
      onView={() => navigateToZone(hit)}
      onLoad={async () => {
        if (defaultIrSelection) await load(defaultIrSelection);
      }}
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
  const { items } = useHits<ZoneSearchHit>();
  const { status } = useInstantSearch();

  if (status == "loading") return <ZonesLoading />;
  if (items.length === 0) return <ZonesNoHits />;

  return (
    <div className="flex flex-col gap-4">
      <div className="grid grid-cols-1 md:grid-cols-3 xl:grid-cols-4 gap-4">
        {items.map((item) => {
          return (
            <div className="h-[200px]" key={item.zoneId}>
              <ZonesSearchHit hit={item} />
            </div>
          );
        })}
      </div>
    </div>
  );
};

export { ZonesHits };
