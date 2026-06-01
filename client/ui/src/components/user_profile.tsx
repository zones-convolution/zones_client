import { Eye, Loader, Play } from "lucide-react";
import { FC } from "react";

import { Avatar, AvatarImage, AvatarFallback } from "@/components/ui/avatar";

import { getProfileImageUrl } from "@/lib/s3_resources";
import { IUser } from "@/lib/zones";

export const getInitials = (fullName: string) => {
  const allNames = fullName.trim().split(" ");
  return allNames.reduce((acc, curr, index) => {
    if (index === 0 || index === allNames.length - 1) {
      acc = `${acc}${curr.charAt(0).toUpperCase()}`;
    }
    return acc;
  }, "");
};

export const UserProfile: FC<IUser> = ({ name, id }) => {
  const imageUrl = getProfileImageUrl(id);

  return (
    <div className="flex h-10 w-fit flex-row items-center rounded-md bg-card/40 px-1.5 backdrop-blur">
      <Avatar className="h-7 w-7 rounded-lg">
        <AvatarImage className="rounded-lg" src={imageUrl} alt={name ?? ""} />
        {name && (
          <AvatarFallback className="rounded-lg">
            {getInitials(name)}
          </AvatarFallback>
        )}
      </Avatar>
      <div className="mx-2 h-fit w-fit text-sm font-thin leading-none">{name}</div>
    </div>
  );
};
