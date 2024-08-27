import { FC } from "react";

import { Separator } from "@/components/ui/separator";

import { BannerImageGallery } from "@/components/banner_image_gallery";
import { IrTable } from "@/components/zone_page/ir_table";
import { getImageUrl } from "@/lib/s3_resources";
import { toZoneMetadata, ZoneSearchHit } from "@/lib/zones";

const Zone: FC<{ zone: ZoneSearchHit }> = ({ zone }) => {
  const zoneMetadata = toZoneMetadata(zone, zone.images, zone.irs);

  return (
    <div className="h-full overflow-scroll bg-card">
      <div className="flex flex-col gap-4 h-full p-4">
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
        <p>{zone.description}</p>
        <Separator />
        <h1 className="text-xl">IRs</h1>
        <IrTable zone={zoneMetadata} />
      </div>
    </div>
  );
};

export default Zone;
