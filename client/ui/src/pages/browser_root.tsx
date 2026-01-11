import { ChevronLeft, ChevronRight, LucideLibrary } from "lucide-react";

import { Button } from "@/components/ui/button";

import {
  Route,
  useBrowserContext,
  useNavigation,
} from "@/context/browser_context";
import Browser from "@/pages/browser";
import CreateZone from "@/pages/create_zone";
import User from "@/pages/user";
import Zone from "@/pages/zone";

const Outlet = () => {
  const { route } = useBrowserContext();

  switch (route.target) {
    case Route.CreateZone:
      return <CreateZone />;
    case Route.Home:
      return <Browser />;
    case Route.Zone:
      return <Zone zoneMetadata={route.state.zone} user={route.state.user} />;
    case Route.User:
      return <User />;
  }
};

const NavigationTitle = () => {
  const { route } = useBrowserContext();
  let title: string;

  switch (route.target) {
    case Route.CreateZone:
      title = "Create Zone";
      break;
    case Route.Home:
      title = "Browse";
      break;
    case Route.Zone:
      title = route.state.zone.title;
      break;
    case Route.User:
      title = "User";
      break;
  }

  return <h2 className="font-thin text-lg">{title}</h2>;
};

const BrowserRoot = () => {
  const { canNavigateBack, canNavigateForward, forward, back, route } =
    useBrowserContext();
  const { navigateToCreateZone, navigateToUser } = useNavigation();

  return (
    <div className="flex flex-col gap-0.5 h-full">
      <div className="flex w-full bg-card p-2 items-center">
        <Button
          variant="ghost"
          onClick={() => back()}
          disabled={!canNavigateBack}
        >
          <ChevronLeft className="w-4 h-4" />
        </Button>
        <Button
          variant="ghost"
          className="mr-4"
          onClick={() => forward()}
          disabled={!canNavigateForward}
        >
          <ChevronRight className="w-4 h-4" />
        </Button>
        <NavigationTitle />
        {route.target == Route.User && (
          <Button className="ml-auto" onClick={navigateToCreateZone}>
            Create Zone
          </Button>
        )}
        {route.target == Route.Home && (
          <Button className="ml-auto" onClick={navigateToUser}>
            User
            <LucideLibrary className="h-4 w-4 ml-2" />
          </Button>
        )}
      </div>
      <Outlet />
    </div>
  );
};

export default BrowserRoot;
