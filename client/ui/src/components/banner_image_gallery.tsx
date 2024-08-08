import { Image } from "lucide-react";
import React, { FC } from "react";

import { cn } from "@/lib/utils";

interface IBannerImageGalleryItem {
  imageUrl: string;
}

const BannerImageGalleryItem: FC<IBannerImageGalleryItem> = ({ imageUrl }) => {
  return (
    <div className="overflow-hidden relative w-full h-full">
      <img className="object-cover w-full h-full absolute" src={imageUrl} />
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
      <div className="gap-2 grid grid-cols-4 rounded-md overflow-hidden h-full w-full">
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
