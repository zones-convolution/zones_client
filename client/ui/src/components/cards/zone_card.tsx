import { Eye, Loader, Play } from "lucide-react";
import { FC } from "react";

import { Button } from "@/components/ui/button";

export const ZoneCard: FC<{
  category: string;
  imageUrl?: string;
  gradient?: string;
  rt60: number;
  onLoad?: () => void;
  onView?: () => void;
  loading: boolean;
  disabled: boolean;
  canLoad: boolean;
}> = ({
  category,
  imageUrl,
  gradient,
  rt60,
  onLoad,
  onView,
  loading,
  disabled,
  canLoad,
}) => {
  return (
    <div className="w-full h-full relative flex justify-between items-end">
      {gradient && (
        <div
          className="absolute object-cover object-center w-full h-full max-h-full rounded-md"
          style={{ backgroundImage: gradient }}
        />
      )}
      {imageUrl && (
        <img
          className="absolute object-cover object-center w-full h-full max-h-full rounded-md"
          src={imageUrl}
          alt="gallery-photo"
        />
      )}
      <div className="backdrop-blur bg-card/40 p-2 m-2 w-fit h-fit rounded-md">
        {category}
        <em className="text-xs font-thin ml-1">{rt60}s</em>
      </div>
      <div className="flex gap-2 p-2 w-fit h-fit rounded-md">
        {canLoad && (
          <Button
            variant="blur"
            onClick={onLoad}
            disabled={loading || disabled}
          >
            {loading ? (
              <Loader className="w-4 h-4 animate-spin" />
            ) : (
              <Play className="w-4 h-4" />
            )}
          </Button>
        )}
        <Button variant="blur" onClick={onView}>
          <Eye className="w-4 h-4" />
        </Button>
      </div>
    </div>
  );
};
