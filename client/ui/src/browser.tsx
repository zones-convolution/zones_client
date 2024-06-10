import { ChevronLeft, ChevronRight, Eye, Play } from "lucide-react";
import { FC } from "react";

import { Button } from "@/components/ui/button";

const CategoryCard: FC<{
  category: string;
  imageUrl: string;
  rt60: number;
}> = ({ category, imageUrl, rt60 }) => {
  return (
    <div className="w-full h-full relative flex items-end">
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
          imageUrl="https://source.unsplash.com/X48hkTT1qQc"
          rt60={1.2}
        />
      </div>
      <div>
        <CategoryCard
          category="STADIUMS"
          imageUrl="https://source.unsplash.com/70YxSTWa2Zw"
          rt60={1.2}
        />
      </div>
      <div>
        <CategoryCard
          category="TUNNELS"
          imageUrl="https://source.unsplash.com/13MzGKuJYSw"
          rt60={1.2}
        />
      </div>
      <div className="col-span-1 row-span-2">
        <CategoryCard
          category="CANYONS"
          imageUrl="https://source.unsplash.com/WeYamle9fDM"
          rt60={1.2}
        />
      </div>
      <div>
        <CategoryCard
          category="TRAINS"
          imageUrl="https://source.unsplash.com/pfkzBUh4KyA"
          rt60={1.2}
        />
      </div>
      <div>
        <CategoryCard
          category="FORESTS"
          imageUrl="https://source.unsplash.com/ESkw2ayO2As"
          rt60={1.2}
        />
      </div>
      <div className="col-span-2">
        <CategoryCard
          category="CAVES"
          imageUrl="https://source.unsplash.com/FZgheUfh4fA"
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
      <div className="h-60 overflow-x-auto relative">
        <div className="absolute flex gap-0.5 h-full">
          <div className="w-80">
            <ZoneCard
              category="FORESTS"
              imageUrl="https://source.unsplash.com/ESkw2ayO2As"
              rt60={1.2}
            />
          </div>
          <div className="w-80">
            <ZoneCard
              category="FORESTS"
              imageUrl="https://source.unsplash.com/ESkw2ayO2As"
              rt60={1.2}
            />
          </div>
          <div className="w-80">
            <ZoneCard
              category="FORESTS"
              imageUrl="https://source.unsplash.com/ESkw2ayO2As"
              rt60={1.2}
            />
          </div>
          <div className="w-80">
            <ZoneCard
              category="FORESTS"
              imageUrl="https://source.unsplash.com/ESkw2ayO2As"
              rt60={1.2}
            />
          </div>
          <div className="w-80">
            <ZoneCard
              category="FORESTS"
              imageUrl="https://source.unsplash.com/ESkw2ayO2As"
              rt60={1.2}
            />
          </div>
          <div className="w-80">
            <ZoneCard
              category="FORESTS"
              imageUrl="https://source.unsplash.com/ESkw2ayO2As"
              rt60={1.2}
            />
          </div>
          <div className="w-80">
            <ZoneCard
              category="FORESTS"
              imageUrl="https://source.unsplash.com/ESkw2ayO2As"
              rt60={1.2}
            />
          </div>
          <div className="w-80">
            <ZoneCard
              category="FORESTS"
              imageUrl="https://source.unsplash.com/ESkw2ayO2As"
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
