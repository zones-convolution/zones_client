import { LucideCircleCheck } from "lucide-react";
import { FC } from "react";

import { BannerImageGallery } from "@/components/banner_image_gallery";
import { IrTable } from "@/components/zone_page/ir_table";
import { ZoneDetails } from "@/components/zone_page/zone_details";
import { ZoneTags } from "@/components/zone_page/zone_tags";
import { useCachedWebZone } from "@/hooks/use_cached_web_zones";
import { ZoneMetadata } from "@/hooks/zone_metadata";
import { IUser, resolveWebZoneImageUrl } from "@/lib/zones";

const Zone: FC<{ zoneMetadata: ZoneMetadata; user?: IUser }> = ({
  zoneMetadata,
  user,
}) => {
  const { cachedWebZone, isCached } = useCachedWebZone(zoneMetadata.zoneId);

  const imageUrls = zoneMetadata.images.map((image) =>
    resolveWebZoneImageUrl(zoneMetadata.zoneId!, image.imageId, isCached),
  );

  const coverImageUrl =
    zoneMetadata.coverImageId &&
    resolveWebZoneImageUrl(
      zoneMetadata.zoneId!,
      zoneMetadata.coverImageId,
      isCached,
    );

  return (
    <div className="h-full min-w-0 overflow-x-hidden overflow-y-auto bg-card">
      <div className="flex min-h-full min-w-0 flex-col gap-4 p-4">
        <div className="flex flex-row justify-between">
          <h1 className="text-xl">{zoneMetadata.title}</h1>
          {isCached && (
            <LucideCircleCheck className=" text-green-500  rounded-full h-6 w-6" />
          )}
        </div>
        {zoneMetadata.images.length > 0 && (
          <div className="h-[300px] min-w-0 max-w-full overflow-hidden">
            <BannerImageGallery
              imageUrls={imageUrls}
              coverImageUrl={coverImageUrl}
            />
          </div>
        )}
        <IrTable
          zone={zoneMetadata}
          cachedZoneMetadata={cachedWebZone.zoneMetadata}
        />
        <ZoneDetails zone={zoneMetadata} user={user} />
        {zoneMetadata.tags && <ZoneTags zone={zoneMetadata} />}
      </div>
    </div>
  );
};

export default Zone;
