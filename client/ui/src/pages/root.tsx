import { Home, Loader, Settings, Target } from "lucide-react";
import React, { FC, ReactNode } from "react";

import { Button } from "@/components/ui/button";
import { Separator } from "@/components/ui/separator";
import { Toaster } from "@/components/ui/toaster";

import ZonesLight from "@/assets/zones_light.svg";
import { Meter } from "@/components/meter";
import { ResizeHandle } from "@/components/resize_handle";
import { BrowserProvider } from "@/context/browser_context";
import { LoadProvider, useLoadContext } from "@/context/load_context";
import { Tabs, TabsProvider, useTabsContext } from "@/context/tabs_context";
import { useEngineLoading } from "@/hooks/use_engine";
import { useMetering } from "@/hooks/use_metering";
import BrowserRoot from "@/pages/browser_root";
import Editor from "@/pages/editor";
import Preferences from "@/pages/preferences";

const NavButton: FC<{
  children: ReactNode;
  to: Tabs;
}> = ({ children, to }) => {
  const { tab, setTab } = useTabsContext();

  return (
    <Button
      variant="ghost"
      className={`justify-start w-full ${to == tab && "bg-accent"}`}
      onClick={() => {
        setTab(to);
      }}
    >
      {children}
    </Button>
  );
};

const Sidebar = () => {
  const { irEngineLoading, convolutionEngineLoading } = useEngineLoading();
  const { currentIr } = useLoadContext();
  const metering = useMetering();

  return (
    <div className="bg-card flex flex-col gap-0.5 p-2">
      <img src={ZonesLight} className="w-[80px]" alt="Zones Logo Dark" />
      <NavButton to={Tabs.Browser}>
        <Home className="w-4 h-4 mr-4" />
        Browse
      </NavButton>
      <NavButton to={Tabs.Editor}>
        <Target className="w-4 h-4 mr-4" />
        Edit
      </NavButton>
      <NavButton to={Tabs.Preferences}>
        <Settings className="w-4 h-4 mr-4" />
        Preferences
      </NavButton>

      <div className="flex flex-col gap-4 mt-auto overflow-hidden">
        {irEngineLoading && (
          <div className="flex items-center">
            <Loader className="w-4 h-4 animate-spin mr-2 shrink-0" /> Impulse
          </div>
        )}

        {convolutionEngineLoading && (
          <div className="flex items-center">
            <Loader className="w-4 h-4 animate-spin mr-2 shrink-0" />
            <p className="line-clamp-1">Convolver</p>
          </div>
        )}

        {currentIr.irSelection && (
          <div className="flex flex-col gap-2">
            <Separator />
            <span className="text-base">{currentIr.irSelection.ir.title}</span>
            <span className="text-sm font-thin">
              {currentIr.irSelection.zone.title}
            </span>
          </div>
        )}
      </div>
      <div className="h-72 w-full mt-2">
        <Meter {...metering} />
      </div>
    </div>
  );
};

const Root = () => {
  const { tab } = useTabsContext();

  return (
    <div
      className="flex flex-row h-screen bg-background relative select-none"
      data-theme="dark"
    >
      <Sidebar />
      <div className="w-full ml-0.5">
        {tab == Tabs.Browser ? (
          <BrowserRoot />
        ) : tab == Tabs.Editor ? (
          <Editor />
        ) : (
          <Preferences />
        )}
      </div>
      <ResizeHandle />
      <Toaster />
    </div>
  );
};

export default () => (
  <LoadProvider>
    <TabsProvider>
      <BrowserProvider>
        <Root />
      </BrowserProvider>
    </TabsProvider>
  </LoadProvider>
);
