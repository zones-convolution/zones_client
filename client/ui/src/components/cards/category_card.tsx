import { FC } from "react";

export const CategoryCard: FC<{
  category: string;
  imageUrl: string;
  rt60: number;
}> = ({ category, imageUrl, rt60 }) => {
  return (
    <div className="flex-1 h-full w-full relative flex items-end">
      <img
        className="absolute object-cover object-center w-full h-full max-h-full rounded-lg"
        src={imageUrl}
        alt="gallery-photo"
      />
      <div className="backdrop-blur bg-card/40 p-2 m-2 w-fit rounded-md h-fit">
        {category}
        <em className="text-xs font-thin ml-1">{rt60}s</em>
      </div>
    </div>
  );
};
