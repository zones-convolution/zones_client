import { Check, Eye, Loader, Play } from "lucide-react";
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

      <div className="flex items-center justify-between p-2">
        {user && <UserProfile {...user} />}
        {isCached && (
          <div className="flex size-10 items-center justify-center rounded-md bg-card/40 backdrop-blur">
            <Check className="h-4 w-4 text-green-500" />
          </div>
        )}
      </div>

      <div className="flex flex-row justify-between ">
        <div className="m-2 flex min-h-10 max-w-[70%] items-center rounded-md bg-card/40 px-3 py-2 backdrop-blur line-clamp-2 break-words">
          {category}
        </div>
        <div className="flex h-full w-fit flex-row items-end gap-2 p-2">
          {canLoad && (
            <Button
              variant="outline"
              size="icon"
              className="size-10 rounded-md border-border/60 bg-background/70 shadow-sm backdrop-blur-md"
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
          <Button
            variant="outline"
            size="icon"
            className="size-10 rounded-md border-border/60 bg-background/70 shadow-sm backdrop-blur-md"
            onClick={onView}
          >
            <Eye className="w-4 h-4" />
          </Button>
        </div>
      </div>
    </div>
  );
};
