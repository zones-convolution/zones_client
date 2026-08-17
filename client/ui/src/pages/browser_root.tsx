import {
  ChevronLeft,
  ChevronRight,
  LucideLibrary,
  LucideZap,
} from "lucide-react";

import { Button } from "@/components/ui/button";
import { Card, CardContent } from "@/components/ui/card";

import {
  Route,
  useBrowserContext,
  useNavigation,
} from "@/context/browser_context";
import { useQuickLoad } from "@/hooks/use_quick_load";
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
  const handleQuickLoad = useQuickLoad();

  return (
    <div className="flex h-full min-h-0 flex-col gap-page-gutter p-page-gutter">
      <Card className="rounded-md py-0" size="sm">
        <CardContent className="flex items-center px-2 py-1">
          <Button
            variant="ghost"
            size="icon-sm"
            onClick={() => back()}
            disabled={!canNavigateBack}
          >
            <ChevronLeft className="w-4 h-4" />
          </Button>
          <Button
            variant="ghost"
            size="icon-sm"
            className="mr-3"
            onClick={() => forward()}
            disabled={!canNavigateForward}
          >
            <ChevronRight className="w-4 h-4" />
          </Button>
          <NavigationTitle />
          <div className="ml-auto flex items-center gap-2">
            {route.target != Route.CreateZone && (
              <Button className="ml-auto" size="sm" onClick={handleQuickLoad}>
                Quick Load
                <LucideZap className="h-4 w-4 ml-2" />
              </Button>
            )}
            {route.target == Route.User && (
              <Button
                className="ml-auto"
                size="sm"
                onClick={navigateToCreateZone}
              >
                Create Zone
              </Button>
            )}
            {route.target == Route.Home && (
              <Button className="ml-auto" size="sm" onClick={navigateToUser}>
                User
                <LucideLibrary className="h-4 w-4 ml-2" />
              </Button>
            )}
          </div>
        </CardContent>
      </Card>
      <Card className="min-h-0 min-w-0 flex-1 rounded-md">
        <CardContent className="h-full p-0">
          <div className="h-full min-h-0 min-w-0 overflow-x-hidden overflow-y-auto p-4">
            <Outlet />
          </div>
        </CardContent>
      </Card>
    </div>
  );
};

export default BrowserRoot;
