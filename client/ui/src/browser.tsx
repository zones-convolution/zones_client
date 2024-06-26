import { ChevronLeft, ChevronRight, Eye, Loader, Play } from "lucide-react";
import { FC } from "react";
import { Link } from "react-router-dom";

import { Button } from "@/components/ui/button";

import { LoadProvider, useLoadContext } from "@/context/LoadContext";
import { useUserZones } from "@/hooks/use_user_zones";

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
  onLoad?: () => void;
  onView?: () => void;
  loading: boolean;
}> = ({ category, imageUrl, rt60, onLoad, onView, loading }) => {
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
        <Button variant="blur" onClick={onLoad} disabled={loading}>
          {loading ? (
            <Loader className="w-4 h-4 animate-spin" />
          ) : (
            <Play className="w-4 h-4" />
          )}
        </Button>
        <Button variant="blur" onClick={onView}>
          <Eye className="w-4 h-4" />
        </Button>
      </div>
    </div>
  );
};

const UserIRs = () => {
  const { userZones } = useUserZones();
  const { load, loadingIr } = useLoadContext();

  return (
    <div className="flex flex-col w-full bg-card p-2 gap-4">
      User IRs
      <div className="h-40 overflow-x-auto relative">
        <div className="absolute flex gap-0.5 h-full">
          {userZones.map((userZone, index) => {
            let isLoadingZone = false;
            if (loadingIr)
              isLoadingZone = loadingIr.zone.title === userZone.title;

            return (
              <div className="w-80" key={index}>
                <ZoneCard
                  category={userZone.title}
                  imageUrl="https://picsum.photos/607"
                  rt60={1.2}
                  loading={isLoadingZone}
                  onLoad={async () => {
                    await load({
                      zone: userZone,
                      ir: userZone.irs[0]!,
                      targetFormat: "stereo",
                    });
                  }}
                />
              </div>
            );
          })}
        </div>
      </div>
    </div>
  );
};

const Browser = () => {
  return (
    <LoadProvider>
      <div className="flex flex-col gap-0.5 h-full">
        <div className="flex w-full bg-card p-2 items-center">
          <Button variant="ghost">
            <ChevronLeft className="w-4 h-4" />
          </Button>
          <Button variant="ghost" className="mr-4">
            <ChevronRight className="w-4 h-4" />
          </Button>
          Browser
          <Button asChild className="ml-auto">
            <Link to="/create">Create Zone</Link>
          </Button>
        </div>
        <Categories />
        <UserIRs />
      </div>
    </LoadProvider>
  );
};

export default Browser;
