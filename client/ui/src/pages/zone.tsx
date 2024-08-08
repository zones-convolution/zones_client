import { FC } from "react";

import { Skeleton } from "@/components/ui/skeleton";

import { BannerImageGallery } from "@/components/banner_image_gallery";
import { IrTable } from "@/components/zone_page/ir_table";
import { useZone } from "@/hooks/use_zone";
import { getImageUrl } from "@/lib/s3_resources";

const Zone: FC<{ zoneId: string }> = ({ zoneId }) => {
  const { data, isLoading } = useZone(zoneId);

  if (data == undefined || isLoading)
    return (
      <>
        <Skeleton />
      </>
    );

  const { zone, images, irs } = data;

  return (
    <div className="flex flex-col gap-4 bg-card h-full p-4">
      <h1 className="text-xl">{zone.title}</h1>
      <div className="h-[300px]">
        <BannerImageGallery
          imageUrls={images.map((image) =>
            getImageUrl(zone.zoneId, image.imageId),
          )}
          coverImageUrl={
            zone.coverImageId
              ? getImageUrl(zone.zoneId, zone.coverImageId)
              : undefined
          }
        />
      </div>
      <IrTable zone={data.zone} />
    </div>
  );
};

export default Zone;
