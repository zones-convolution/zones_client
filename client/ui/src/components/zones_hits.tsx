import { Hit } from "instantsearch.js";
import { Salad } from "lucide-react";
import React, { FC } from "react";
import { InfiniteHits, useHits, useInstantSearch } from "react-instantsearch";

import { Skeleton } from "@/components/ui/skeleton";

import { ZoneCard } from "@/components/cards/zone_card";
import { useNavigation } from "@/context/browser_context";
import { getImageUrl } from "@/lib/s3_resources";
import { ZoneSearchHit } from "@/lib/zones";

const ZonesSearchHit: FC<{
  hit: Hit<ZoneSearchHit>;
}> = ({ hit }) => {
  const imageUrl = getImageUrl(hit.zoneId, hit.coverImageId!);
  const { navigateToZone } = useNavigation();
  return (
    <ZoneCard
      category={hit.title!}
      imageUrl={imageUrl}
      rt60={1.2}
      loading={false}
      disabled={false}
      canLoad={true}
      onView={() => navigateToZone(hit.zoneId)}
    />
  );
};

const ZonesNoHits = () => {
  return (
    <div className="py-10 px-6 text-center">
      <div className="inline-block">
        <div className="flex justify-center items-center rounded-full bg-red-500 w-28 h-28 p-2 text-foreground">
          <Salad className="w-16 h-16 text-white" />
        </div>
      </div>
      <h2 className="mt-6 mb-2 text-2xl">{"no_hits.heading"}</h2>
      <span className="text-secondary">{"no_hits.info"}</span>
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

  return items.length === 0 ? (
    <ZonesNoHits />
  ) : (
    <InfiniteHits
      hitComponent={ZonesSearchHit}
      classNames={{
        root: "flex flex-col gap-4",
        list: "grid grid-cols-1 md:grid-cols-3 xl:grid-cols-4 gap-4",
        item: "h-[200px]",
      }}
    />
  );
};

export { ZonesHits };
