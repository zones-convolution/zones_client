import { Eye, Loader, LucideCircleCheck, Play } from "lucide-react";
import { FC } from "react";

import { Button } from "@/components/ui/button";

import { UserProfile } from "@/components/user_profile";
import { IUser } from "@/lib/zones";

export const ZoneCard: FC<{
  user?: IUser;
  category: string;
  imageUrl?: string;
  gradient?: string;
  rt60: number;
  onLoad?: () => void;
  onView?: () => void;
  loading: boolean;
  disabled: boolean;
  canLoad: boolean;
  isCached: boolean;
}> = ({
  user,
  category,
  imageUrl,
  gradient,
  rt60,
  onLoad,
  onView,
  loading,
  disabled,
  canLoad,
  isCached,
}) => {
  return (
    <div className="w-full h-full relative flex flex-col justify-between">
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

      <div className="p-2 flex justify-between items-center">
        {user && <UserProfile {...user} />}
        {isCached && (
          <div className="backdrop-blur bg-card/40 h-full aspect-square flex items-center justify-center">
            <LucideCircleCheck className="text-green-500 h-6 w-6 " />
          </div>
        )}
      </div>

      <div className="flex flex-row justify-between ">
        <div className="backdrop-blur bg-card/40 p-2 m-2 w-fit h-fit rounded-md line-clamp-2 break-words">
          {category}
        </div>
        <div className="flex flex-row gap-2 p-2 w-fit h-full rounded-md items-end">
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
    </div>
  );
};
