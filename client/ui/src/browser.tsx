import { ChevronLeft, ChevronRight, Eye, Play } from "lucide-react";
import { FC } from "react";

import { Button } from "@/components/ui/button";

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

const Categories = () => {
  return (
    <div className="flex-grow bg-card p-2 grid grid-cols-3 grid-rows-3 gap-0.5">
      <div>
        <CategoryCard
          category="CATHEDRALS"
          imageUrl="https://picsum.photos/600"
          rt60={1.2}
        />
      </div>
      <div>
        <CategoryCard
          category="STADIUMS"
          imageUrl="https://picsum.photos/601"
          rt60={1.2}
        />
      </div>
      <div>
        <CategoryCard
          category="TUNNELS"
          imageUrl="https://picsum.photos/602"
          rt60={1.2}
        />
      </div>
      <div className="col-span-1 row-span-2">
        <CategoryCard
          category="CANYONS"
          imageUrl="https://picsum.photos/603"
          rt60={1.2}
        />
      </div>
      <div>
        <CategoryCard
          category="TRAINS"
          imageUrl="https://picsum.photos/604"
          rt60={1.2}
        />
      </div>
      <div>
        <CategoryCard
          category="FORESTS"
          imageUrl="https://picsum.photos/605"
          rt60={1.2}
        />
      </div>
      <div className="col-span-2">
        <CategoryCard
          category="CAVES"
          imageUrl="https://picsum.photos/606"
          rt60={1.2}
        />
      </div>
    </div>
  );
};

const ZoneCard: FC<{
  category: string;
  imageUrl: string;
  rt60: number;
}> = ({ category, imageUrl, rt60 }) => {
  return (
    <div className="w-full h-full relative flex justify-between items-end">
      <img
        className="absolute object-cover object-center w-full h-full max-h-full rounded-lg"
        src={imageUrl}
        alt="gallery-photo"
      />
      <div className="backdrop-blur bg-card/40 p-2 m-2 w-fit h-fit rounded-md">
        {category}
        <em className="text-xs font-thin ml-1">{rt60}s</em>
      </div>
      <div className="flex gap-2 p-2 w-fit h-fit rounded-md">
        <Button variant="blur">
          <Play className="w-4 h-4" />
        </Button>
        <Button variant="blur">
          <Eye className="w-4 h-4" />
        </Button>
      </div>
    </div>
  );
};

const UserIRs = () => {
  return (
    <div className="flex flex-col w-full bg-card p-2 gap-4">
      User IRs
      <div className="h-40 overflow-x-auto relative">
        <div className="absolute flex gap-0.5 h-full">
          <div className="w-80">
            <ZoneCard
              category="FORESTS"
              imageUrl="https://picsum.photos/607"
              rt60={1.2}
            />
          </div>
          <div className="w-80">
            <ZoneCard
              category="FORESTS"
              imageUrl="https://picsum.photos/608"
              rt60={1.2}
            />
          </div>
          <div className="w-80">
            <ZoneCard
              category="FORESTS"
              imageUrl="https://picsum.photos/609"
              rt60={1.2}
            />
          </div>
          <div className="w-80">
            <ZoneCard
              category="FORESTS"
              imageUrl="https://picsum.photos/610"
              rt60={1.2}
            />
          </div>
          <div className="w-80">
            <ZoneCard
              category="FORESTS"
              imageUrl="https://picsum.photos/611"
              rt60={1.2}
            />
          </div>
          <div className="w-80">
            <ZoneCard
              category="FORESTS"
              imageUrl="https://picsum.photos/612"
              rt60={1.2}
            />
          </div>
          <div className="w-80">
            <ZoneCard
              category="FORESTS"
              imageUrl="https://picsum.photos/613"
              rt60={1.2}
            />
          </div>
          <div className="w-80">
            <ZoneCard
              category="FORESTS"
              imageUrl="https://picsum.photos/614"
              rt60={1.2}
            />
          </div>
        </div>
      </div>
    </div>
  );
};

const Browser = () => {
  return (
    <div className="flex flex-col gap-0.5 h-full">
      <div className="flex w-full bg-card p-2 items-center">
        <Button variant="ghost">
          <ChevronLeft className="w-4 h-4" />
        </Button>
        <Button variant="ghost" className="mr-4">
          <ChevronRight className="w-4 h-4" />
        </Button>
        Browser
      </div>
      <Categories />
      <UserIRs />
    </div>
  );
};

export default Browser;
