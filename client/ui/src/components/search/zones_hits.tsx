import { Search } from "lucide-react";
import React, { FC } from "react";

import { Skeleton } from "@/components/ui/skeleton";

import { ZoneCard } from "@/components/cards/zone_card";
import { useNavigation } from "@/context/browser_context";
import { useLoadContext } from "@/context/load_context";
import { useSearchContext } from "@/context/search_context";
import { useCachedWebZone } from "@/hooks/use_cached_web_zones";
import { useValidTargetFormats } from "@/hooks/use_valid_target_formats";
import { ZoneMetadata } from "@/hooks/zone_metadata";
import { doesZoneMatchSelection, getDefaultIrSelection } from "@/lib/irs";
import { getImageUrl } from "@/lib/s3_resources";
import { getUserZoneGradient } from "@/lib/user_zones";
import {
  IUser,
  IZone,
  resolveWebZoneImageUrl,
  toZoneMetadata,
} from "@/lib/zones";

const ZonesSearchHit: FC<{
  zone: ZoneMetadata;
  user?: IUser;
}> = ({ zone, user }) => {
  const { navigateToZone } = useNavigation();
  const { load, loadingIr, currentIr } = useLoadContext();
  const { validTargetFormats } = useValidTargetFormats();
  let isLoadingZone = doesZoneMatchSelection(zone, loadingIr);
  let isCurrentZone = doesZoneMatchSelection(zone, currentIr);
  let defaultIrSelection = getDefaultIrSelection(zone, validTargetFormats);
  const { isCached } = useCachedWebZone(zone.zoneId);
  const isWebZone = zone.zoneType == "web";
  const imageUrl = zone.zoneId
    ? resolveWebZoneImageUrl(zone.zoneId, zone.coverImageId!, isCached)
    : undefined;

  return (
    <ZoneCard
      user={user}
      category={zone.title}
      gradient={isWebZone ? undefined : getUserZoneGradient(zone)}
      imageUrl={imageUrl}
      rt60={1.2}
      loading={isLoadingZone}
      disabled={isCurrentZone}
      canLoad={defaultIrSelection != undefined}
      onView={() => navigateToZone(zone, user)}
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
        {search.data.map(({ zone, user }) => {
          return (
            <div className="h-[200px]" key={zone.zoneId ?? zone.title}>
              <ZonesSearchHit zone={zone} user={user} />
            </div>
          );
        })}
      </div>
    </div>
  );
};

export { ZonesHits, ZonesSearchHit };
