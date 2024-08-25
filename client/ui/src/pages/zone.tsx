import { FC } from "react";

import { BannerImageGallery } from "@/components/banner_image_gallery";
import { IrTable } from "@/components/zone_page/ir_table";
import { getImageUrl } from "@/lib/s3_resources";
import { toZoneMetadata, ZoneSearchHit } from "@/lib/zones";

const Zone: FC<{ zone: ZoneSearchHit }> = ({ zone }) => {
  const zoneMetadata = toZoneMetadata(zone, zone.images, zone.irs);

  return (
    <div className="flex flex-col gap-4 bg-card h-full p-4">
      <h1 className="text-xl">{zone.title}</h1>
      <div className="h-[300px]">
        <BannerImageGallery
          imageUrls={zone.images.map((image) =>
            getImageUrl(zone.zoneId, image.imageId),
          )}
          coverImageUrl={
            zone.coverImageId
              ? getImageUrl(zone.zoneId, zone.coverImageId)
              : undefined
          }
        />
      </div>
      <IrTable zone={zoneMetadata} />
    </div>
  );
};

export default Zone;
