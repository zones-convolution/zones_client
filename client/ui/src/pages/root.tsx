import { Home, Loader, Settings, Target } from "lucide-react";
import React, { FC, ReactNode } from "react";
import { SWRConfig } from "swr";

import { Button } from "@/components/ui/button";
import { Card, CardContent } from "@/components/ui/card";
import { Separator } from "@/components/ui/separator";
import { Toaster } from "@/components/ui/sonner";

import ZonesLight from "@/assets/zones_light.svg";
import { Meter } from "@/components/meter";
import { ResizeHandle } from "@/components/resize_handle";
import { BrowserProvider } from "@/context/browser_context";
import { LoadProvider, useLoadContext } from "@/context/load_context";
import { Tabs, TabsProvider, useTabsContext } from "@/context/tabs_context";
import { VisualiserProvider } from "@/context/visualiser_context";
import { useEngineLoading } from "@/hooks/use_engine";
import { useMetering } from "@/hooks/use_metering";
import { fetcher } from "@/lib/api";
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
    <div className="h-full w-44 min-w-44 max-w-44 p-page-gutter pr-0">
      <Card className="flex h-full rounded-md">
        <CardContent className="flex h-full flex-col p-page-gutter">
          <img
            src={ZonesLight}
            className="mb-2 ml-4 w-[80px]"
            alt="Zones Logo Dark"
          />
          <div className="flex flex-col gap-0.5">
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
          </div>

          <div className="mt-auto flex flex-col gap-4 overflow-hidden">
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
                <span className="text-base line-clamp-2 break-words">
                  {currentIr.irSelection.ir.title}
                </span>
                <span className="text-sm font-thin line-clamp-2 break-words">
                  {currentIr.irSelection.zone.title}
                </span>
              </div>
            )}
          </div>
          <div className="mt-2 h-64 w-full">
            <Meter {...metering} />
          </div>
        </CardContent>
      </Card>
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
      <div className="min-w-0 flex-1 overflow-hidden">
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
  <SWRConfig value={{ fetcher: fetcher }}>
    <LoadProvider>
      <TabsProvider>
        <BrowserProvider>
          <VisualiserProvider>
            <Root />
          </VisualiserProvider>
        </BrowserProvider>
      </TabsProvider>
    </LoadProvider>
  </SWRConfig>
);
