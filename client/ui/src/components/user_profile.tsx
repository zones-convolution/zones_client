import { Eye, Loader, Play } from "lucide-react";
import { FC } from "react";

import { Avatar, AvatarImage, AvatarFallback } from "@/components/ui/avatar";

export const getInitials = (fullName: string) => {
  const allNames = fullName.trim().split(" ");
  return allNames.reduce((acc, curr, index) => {
    if (index === 0 || index === allNames.length - 1) {
      acc = `${acc}${curr.charAt(0).toUpperCase()}`;
    }
    return acc;
  }, "");
};

interface IUserProfile {
  name: string | null;
  imageUrl: string;
}

export const UserProfile: FC<IUserProfile> = ({ name, imageUrl }) => {
  return (
    <div className="flex flex-row items-center backdrop-blur bg-card/40 w-fit">
      <Avatar className="h-8 w-8 rounded-lg">
        <AvatarImage src={imageUrl} alt={name ?? ""} />
        {name && (
          <AvatarFallback className="rounded-lg">
            {getInitials(name)}
          </AvatarFallback>
        )}
      </Avatar>
      <div className="w-fit h-fit font-thin text-sm mx-2">{name}</div>
    </div>
  );
};
