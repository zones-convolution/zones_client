import { Home, Loader, Settings, Target } from "lucide-react";
import React, { FC, ReactNode } from "react";
import { Outlet } from "react-router-dom";

import { Button } from "@/components/ui/button";
import { Separator } from "@/components/ui/separator";
import { Toaster } from "@/components/ui/toaster";

import ZonesLight from "@/assets/zones_light.svg";
import { Meter } from "@/components/meter";
import { ResizeHandle } from "@/components/resize_handle";
import { LoadProvider, useLoadContext } from "@/context/load_context";
import { Tabs, TabsProvider, useTabsContext } from "@/context/tabs_context";
import { useEngineLoading } from "@/hooks/use_engine";
import { useMetering } from "@/hooks/use_metering";
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
    <div className="bg-card flex flex-col gap-0.5">
      <img src={ZonesLight} className="w-[80px] ml-4" alt="Zones Logo Dark" />
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

      <div className="px-2 flex flex-col gap-4 mt-auto overflow-hidden">
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
      <div className="h-72 w-full p-2">
        <Meter {...metering} />
      </div>
    </div>
  );
};

const Root = () => {
  const { tab } = useTabsContext();

  return (
    <div
      className="flex flex-row h-screen bg-background relative"
      data-theme="dark"
    >
      <LoadProvider>
        <Sidebar />
        <div className="w-full ml-0.5">
          {tab == Tabs.Browser ? (
            <Outlet />
          ) : tab == Tabs.Editor ? (
            <Editor />
          ) : (
            <Preferences />
          )}
        </div>
      </LoadProvider>
      <ResizeHandle />
      <Toaster />
    </div>
  );
};

export default () => (
  <TabsProvider>
    <Root />
  </TabsProvider>
);
