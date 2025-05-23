import { FC } from "react";

import { Separator } from "@/components/ui/separator";

import { BannerImageGallery } from "@/components/banner_image_gallery";
import { UserProfile } from "@/components/user_profile";
import { IrTable } from "@/components/zone_page/ir_table";
import { ZoneDetails } from "@/components/zone_page/zone_details";
import { ZoneTags } from "@/components/zone_page/zone_tags";
import { getImageUrl, getProfileImageUrl } from "@/lib/s3_resources";
import { toZoneMetadata, IZone } from "@/lib/zones";

const Zone: FC<{ zone: IZone }> = ({ zone }) => {
  const zoneMetadata = toZoneMetadata(zone);
  const profileImageUrl = getProfileImageUrl(zone.user.id);

  return (
    <div className="h-full overflow-scroll bg-card">
      <div className="flex flex-col gap-4 h-full p-4">
        <div className="flex flex-row justify-between">
          <h1 className="text-xl">{zone.title}</h1>
        </div>
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
        <ZoneDetails zone={zone} />
        <ZoneTags zone={zone} />
      </div>
    </div>
  );
};

export default Zone;
