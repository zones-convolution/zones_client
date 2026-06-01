import { LucideLibrary, LucideWifiOff } from "lucide-react";
import React from "react";

import { Button } from "@/components/ui/button";

import { ZonesHits } from "@/components/search/zones_hits";
import { ZonesPaginationWithLinks } from "@/components/search/zones_pagination";
import { ZonesSearchBox } from "@/components/search/zones_search_box";
import { useNavigation } from "@/context/browser_context";
import { SearchProvider, useWebZoneRepository } from "@/context/search_context";
import { useOnline } from "@/hooks/use_online";

const OfflineBrowser = () => {
  const { navigateToUser } = useNavigation();
  return (
    <div className="flex h-full flex-col items-center justify-center gap-4 text-center">
        <div className="inline-block">
          <div className="flex justify-center items-center rounded-full h-20 w-20 bg-background p-2 text-foreground">
            <LucideWifiOff className="w-8 h-8 text-white" />
          </div>
        </div>
        <h2 className="text-2xl">It Looks Like You're Offline...</h2>
        <span className="text-secondary">
          You can still access Zones you've downloaded on the User page.
        </span>
        <div className="flex justify-center">
          <Button onClick={navigateToUser}>
            Go To User Page <LucideLibrary className="w-4 h-4 ml-2" />
          </Button>
        </div>
    </div>
  );
};

const Browser = () => {
  const isOnline = useOnline();

  if (!isOnline) return <OfflineBrowser />;
  return (
    <div className="flex min-h-full flex-col gap-4">
      <SearchProvider repository={useWebZoneRepository}>
        <ZonesSearchBox />
        <ZonesHits />
        <ZonesPaginationWithLinks pageSizeOptions={[10, 15, 20]} />
      </SearchProvider>
    </div>
  );
};

export default Browser;
