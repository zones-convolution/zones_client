import { ChevronLeft, ChevronRight } from "lucide-react";

import { Button } from "@/components/ui/button";

import {
  Route,
  useBrowserContext,
  useNavigation,
} from "@/context/browser_context";
import Browser from "@/pages/browser";
import CreateZone from "@/pages/create_zone";
import Zone from "@/pages/zone";

const Outlet = () => {
  const { route } = useBrowserContext();

  switch (route.target) {
    case Route.CreateZone:
      return <CreateZone />;
    case Route.Home:
      return <Browser />;
    case Route.Zone:
      return <Zone zone={route.state} />;
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
      title = route.state.title;
      break;
  }

  return <h2 className="font-thin text-lg">{title}</h2>;
};

const BrowserRoot = () => {
  const { canNavigateBack, canNavigateForward, forward, back } =
    useBrowserContext();
  const { navigateToCreateZone } = useNavigation();

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
        <Button className="ml-auto" onClick={navigateToCreateZone}>
          Create Zone
        </Button>
      </div>
      {/*<div className="flex flex-col">*/}
      {/*  {validTargetFormats.map((format) => (*/}
      {/*    <span>{format}</span>*/}
      {/*  ))}*/}
      {/*</div>*/}
      <Outlet />
    </div>
  );
};

export default BrowserRoot;
