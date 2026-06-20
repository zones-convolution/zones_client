import { Image } from "lucide-react";
import React, { FC } from "react";

import { cn } from "@/lib/utils";

interface IBannerImageGalleryItem {
  imageUrl: string;
}

const BannerImageGalleryItem: FC<IBannerImageGalleryItem> = ({ imageUrl }) => {
  return (
    <div className="relative h-full w-full overflow-hidden">
      <img className="absolute inset-0 h-full w-full object-cover" src={imageUrl} />
    </div>
  );
};

interface IBannerImageGallery {
  imageUrls: string[];
  coverImageUrl?: string;
}

const NoImages = () => {
  return (
    <div className="bg-muted rounded-md flex w-full h-full items-center justify-center">
      <Image className="w-8 h-8" />
    </div>
  );
};

const BannerImageGallery: FC<IBannerImageGallery> = ({
  imageUrls,
  coverImageUrl,
}) => {
  if (!coverImageUrl) coverImageUrl = imageUrls[0];

  const filteredImageUrls = imageUrls.filter(
    (imageUrl) => imageUrl !== coverImageUrl,
  );

  const filteredImageCount = filteredImageUrls.length;

  const canDisplayTwo = filteredImageCount >= 2;
  const canDisplayQuad = filteredImageCount >= 4;

  if (coverImageUrl)
    return (
      <div className="grid h-full w-full min-w-0 max-w-full grid-cols-4 gap-2 overflow-hidden rounded-md">
        <div
          className={cn("row-span-2 col-span-4", {
            "md:col-span-2": filteredImageCount >= 2,
          })}
        >
          <BannerImageGalleryItem imageUrl={coverImageUrl} />
        </div>
        {filteredImageUrls.map((imageUrl, index) => (
          <div
            key={imageUrl}
            className={cn("hidden col-span-2 row-span-1", {
              "md:block": index < 2 && canDisplayTwo,
              "lg:block": index < 4 && canDisplayQuad,
              "lg:col-span-1": canDisplayQuad,
            })}
          >
            <BannerImageGalleryItem imageUrl={imageUrl} />
          </div>
        ))}
      </div>
    );
  else return <NoImages />;
};

export { BannerImageGallery, BannerImageGalleryItem };
