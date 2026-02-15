import { ZoneCard } from "@/components/cards/zone_card";
import { useNavigation } from "@/context/browser_context";
import { useLoadContext } from "@/context/load_context";
import { useCachedWebZones } from "@/hooks/use_cached_web_zones";
import { useUserZones } from "@/hooks/use_user_zones";
import { useValidTargetFormats } from "@/hooks/use_valid_target_formats";
import { doesZoneMatchSelection, getDefaultIrSelection } from "@/lib/irs";
import { getUserZoneGradient } from "@/lib/user_zones";

const User = () => {
  const { userZones } = useUserZones();
  const { load, loadingIr, currentIr } = useLoadContext();
  const { navigateToZone } = useNavigation();
  const { validTargetFormats } = useValidTargetFormats();
  const { cachedWebZones } = useCachedWebZones();

  const mergedZones = [...userZones, ...cachedWebZones].sort((a, b) =>
    a.title.localeCompare(b.title),
  );

  return (
    <div className="flex flex-col gap-4 bg-card h-full p-4 overflow-scroll">
      User IRs
      <div className="flex flex-col gap-4">
        <div className="grid grid-cols-1 md:grid-cols-3 xl:grid-cols-4 gap-4">
          {mergedZones.map((userZone, index) => {
            let isLoadingZone = doesZoneMatchSelection(userZone, loadingIr);
            let isCurrentZone = doesZoneMatchSelection(userZone, currentIr);
            let defaultIrSelection = getDefaultIrSelection(
              userZone,
              validTargetFormats,
            );

            return (
              <div className="h-[200px]" key={index}>
                <ZoneCard
                  category={userZone.title}
                  gradient={getUserZoneGradient(userZone)}
                  rt60={1.2}
                  loading={isLoadingZone}
                  disabled={isCurrentZone}
                  canLoad={defaultIrSelection != undefined}
                  onLoad={async () => {
                    if (defaultIrSelection) await load(defaultIrSelection);
                  }}
                  onView={() => {
                    navigateToZone(userZone);
                  }}
                  isCached={false}
                />
              </div>
            );
          })}
        </div>
      </div>
    </div>
  );
};

export default User;
