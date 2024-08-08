import { CategoryCard } from "@/components/cards/category_card";
import { ZoneCard } from "@/components/cards/zone_card";
import { useNavigation } from "@/context/browser_context";
import { useLoadContext } from "@/context/load_context";
import { useUserZones } from "@/hooks/use_user_zones";
import { useValidTargetFormats } from "@/hooks/use_valid_target_formats";
import { doesZoneMatchSelection, getDefaultIrSelection } from "@/lib/irs";

const Categories = () => {
  const { navigateToSearch } = useNavigation();

  return (
    <div
      className="flex-grow bg-card p-2 grid grid-cols-3 grid-rows-3 gap-0.5"
      onClick={navigateToSearch}
    >
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

const UserIRs = () => {
  const { userZones } = useUserZones();
  const { load, loadingIr, currentIr } = useLoadContext();
  const { navigateToUserZone } = useNavigation();
  const { validTargetFormats } = useValidTargetFormats();

  return (
    <div className="flex flex-col w-full bg-card p-2 gap-4">
      User IRs
      <div className="h-40 overflow-x-auto relative">
        <div className="absolute flex gap-0.5 h-full">
          {userZones.map((userZone, index) => {
            let isLoadingZone = doesZoneMatchSelection(userZone, loadingIr);
            let isCurrentZone = doesZoneMatchSelection(userZone, currentIr);
            let defaultIrSelection = getDefaultIrSelection(
              userZone,
              validTargetFormats,
            );

            return (
              <div className="w-80" key={index}>
                <ZoneCard
                  category={userZone.title}
                  imageUrl="https://picsum.photos/607"
                  rt60={1.2}
                  loading={isLoadingZone}
                  disabled={isCurrentZone}
                  canLoad={defaultIrSelection != undefined}
                  onLoad={async () => {
                    if (defaultIrSelection) await load(defaultIrSelection);
                  }}
                  onView={() => {
                    navigateToUserZone(userZone);
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
    <>
      <Categories />
      <UserIRs />
    </>
  );
};

export default Browser;
