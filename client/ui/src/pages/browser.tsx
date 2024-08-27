import { ZoneCard } from "@/components/cards/zone_card";
import { useNavigation } from "@/context/browser_context";
import { useLoadContext } from "@/context/load_context";
import { useUserZones } from "@/hooks/use_user_zones";
import { useValidTargetFormats } from "@/hooks/use_valid_target_formats";
import { doesZoneMatchSelection, getDefaultIrSelection } from "@/lib/irs";
import Search from "@/pages/search";

const UserIRs = () => {
  const { userZones } = useUserZones();
  const { load, loadingIr, currentIr } = useLoadContext();
  const { navigateToUserZone } = useNavigation();
  const { validTargetFormats } = useValidTargetFormats();

  return (
    <div className="flex flex-col w-full bg-card p-4 gap-4">
      User IRs
      <div className="h-32 overflow-x-auto relative">
        <div className="absolute flex gap-4 h-full">
          {userZones.map((userZone, index) => {
            let isLoadingZone = doesZoneMatchSelection(userZone, loadingIr);
            let isCurrentZone = doesZoneMatchSelection(userZone, currentIr);
            let defaultIrSelection = getDefaultIrSelection(
              userZone,
              validTargetFormats,
            );

            return (
              <div className="w-64" key={index}>
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
      <div className="flex flex-col gap-4 bg-card h-full p-4 overflow-scroll">
        <Search />
      </div>
      <UserIRs />
    </>
  );
};

export default Browser;
