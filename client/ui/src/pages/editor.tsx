import { Pause, Play, Repeat, Search, Settings } from "lucide-react";
import React, { FC, ReactNode, useState } from "react";
import { ErrorBoundary, FallbackProps } from "react-error-boundary";

import { Button } from "@/components/ui/button";
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from "@/components/ui/select";
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs";
import { Toggle } from "@/components/ui/toggle";

import { BannerImageGalleryItem } from "@/components/banner_image_gallery";
import { Knob } from "@/components/knob";
import { Visualiser2D } from "@/components/visualiser/2d";
import { Visualiser3D } from "@/components/visualiser/3d";
import { VisualiserControls } from "@/components/visualiser/visualiser_controls";
import { useLoadContext } from "@/context/load_context";
import { useTabsContext, Tabs as ZoneTab } from "@/context/tabs_context";
import { useVisualiserContext } from "@/context/visualiser_context";
import { useControlParameterIndexUpdater } from "@/hooks/use_control_parameter_index_updater";
import { usePlayer } from "@/hooks/use_player";
import { Parameters } from "@/lib/parameters";
import { getImageUrl } from "@/lib/s3_resources";
import { getUserZoneGradient } from "@/lib/user_zones";
import { getCachedWebZoneImageUrl } from "@/lib/zones";

const Panel: FC<{ children: ReactNode }> = ({ children }) => (
  <Card className="flex w-full flex-1 overflow-hidden rounded-md">
    {children}
  </Card>
);

const PanelHeading: FC<{ children: ReactNode }> = ({ children }) => {
  return <CardTitle className="uppercase tracking-wide">{children}</CardTitle>;
};

const PanelContent: FC<{ children: ReactNode }> = ({ children }) => {
  return <CardContent className="flex h-full flex-1 flex-row justify-around">{children}</CardContent>;
};

const IOPanel = () => {
  return (
    <Panel>
      <CardHeader>
        <PanelHeading>IO</PanelHeading>
      </CardHeader>
      <PanelContent>
        <Knob identifier={Parameters.Input} />
        <Knob identifier={Parameters.DryWetMix} />
        <Knob identifier={Parameters.Output} />
      </PanelContent>
    </Panel>
  );
};

const EQPanel = () => {
  return (
    <Panel>
      <CardHeader>
        <PanelHeading>EQ</PanelHeading>
      </CardHeader>
      <PanelContent>
        <Knob identifier={Parameters.Bass} />
        <Knob identifier={Parameters.Treble} />
      </PanelContent>
    </Panel>
  );
};

const MainPanel = () => {
  return (
    <Panel>
      <CardHeader>
        <PanelHeading>MAIN</PanelHeading>
      </CardHeader>
      <PanelContent>
        <Knob
          identifier={Parameters.ReverbTime}
          showMidpointIndicator={false}
        />
        <Knob identifier={Parameters.Resample} trackFromMidpoint />
      </PanelContent>
    </Panel>
  );
};

const ListenPanel = () => {
  const { togglePlaying, toggleLooping, playerState, selectResource } =
    usePlayer();

  const { playing, looping, resource } = playerState;

  return (
    <Panel>
      <CardHeader>
        <PanelHeading>LISTEN</PanelHeading>
      </CardHeader>
      <PanelContent>
        <div className="flex gap-2 items-center grow">
          <Button
            variant={playing ? "default" : "outline"}
            onClick={async () => {
              await togglePlaying();
            }}
          >
            {playing ? (
              <Pause className="w-4 h-4" />
            ) : (
              <Play className="w-4 h-4" />
            )}
          </Button>
          <Button
            onClick={async () => {
              await toggleLooping();
            }}
            variant={looping ? "default" : "outline"}
          >
            <Repeat className="w-4 h-4" />
          </Button>
          <Select value={resource} onValueChange={selectResource}>
            <SelectTrigger>
              <SelectValue />
            </SelectTrigger>
            <SelectContent>
              <SelectItem value="snare">Snare</SelectItem>
              <SelectItem value="voice">Voice</SelectItem>
            </SelectContent>
          </Select>
        </div>
      </PanelContent>
    </Panel>
  );
};

const TimePanel = () => {
  return (
    <Panel>
      <CardHeader>
        <PanelHeading>TIME</PanelHeading>
      </CardHeader>
      <PanelContent>
        <Knob identifier={Parameters.PreDelay} showMidpointIndicator={false} />
        <Knob identifier={Parameters.Trim} showMidpointIndicator={false} />
        <Knob identifier={Parameters.Attack} showMidpointIndicator={false} />
      </PanelContent>
    </Panel>
  );
};

const CurrentIrPanel = () => {
  const { currentIr } = useLoadContext();
  const { setTab } = useTabsContext();

  const zone = currentIr.irSelection?.zone;

  if (zone?.zoneId && zone?.coverImageId)
    return (
      <Card className="relative flex h-full w-full flex-1 items-end overflow-hidden rounded-md py-0">
        <div className="absolute w-full h-full">
          <BannerImageGalleryItem
            imageUrl={getCachedWebZoneImageUrl(zone.zoneId, zone.coverImageId)}
          />
        </div>
        <div className="m-2 w-fit rounded-md bg-card/40 p-2 backdrop-blur">
          {zone?.title}
        </div>
      </Card>
    );

  if (zone?.title)
    return (
      <Card className="relative flex h-full w-full flex-1 items-end overflow-hidden rounded-md py-0">
        <div
          className="absolute w-full h-full"
          style={{ backgroundImage: getUserZoneGradient(zone) }}
        />
        <div className="m-2 w-fit rounded-md bg-card/40 p-2 backdrop-blur">
          {zone?.title}
        </div>
      </Card>
    );

  return (
    <Card className="flex-1 rounded-md">
      <CardContent className="flex h-full flex-col items-center justify-center gap-6 py-10 text-center">
        <h2 className="mt-6 mb-2 text-2xl">No Zone Loaded</h2>
        <span className="text-secondary">
          Head over to browse to get started!
        </span>
        <Button
          onClick={() => {
            setTab(ZoneTab.Browser);
          }}
        >
          Browse <Search className="w-4 h-4 ml-2" />
        </Button>
      </CardContent>
    </Card>
  );
};

const FailedToLoadVisualiser = ({
  error,
  resetErrorBoundary,
}: FallbackProps): React.JSX.Element => {
  return (
    <div className="flex flex-col gap-2 p-4 h-full items-center justify-center">
      <h1 className="text-2xl font-bold">Oops!</h1>
      <p>Sorry, an unexpected error has occurred.</p>
      <p className="text-destructive">
        <i>{error.message}</i>
      </p>
      <Button onClick={resetErrorBoundary}>Try again</Button>
    </div>
  );
};

const Editor = () => {
  useControlParameterIndexUpdater();

  const context = useVisualiserContext();
  const [showVisualiserControls, setShowVisualiserControls] =
    useState<boolean>(false);

  return (
    <div className="flex h-full flex-col gap-page-gutter p-page-gutter">
      <div className="flex h-full flex-row gap-page-gutter">
        <MainPanel />
        <CurrentIrPanel />
      </div>
      <div className="flex h-full flex-row gap-page-gutter">
        <TimePanel />
        <Card className="relative flex-1 overflow-hidden rounded-md">
          <ErrorBoundary fallbackRender={FailedToLoadVisualiser}>
            <Tabs defaultValue="2d" className="h-full">
              <TabsContent
                value="2d"
                className="absolute top-0 w-full h-full flex mt-0"
              >
                <Visualiser2D context={context} />
              </TabsContent>
              <TabsContent
                value="3d"
                className="absolute top-0 w-full h-full flex mt-0"
              >
                <Visualiser3D context={context} />
              </TabsContent>

              <div className="absolute top-0 right-0 m-3 flex flex-row justify-around">
                <Toggle
                  aria-label="Visualiser Controls"
                  className="flex-1 "
                  pressed={showVisualiserControls}
                  onPressedChange={setShowVisualiserControls}
                >
                  <Settings className="h-4 w-4" />
                </Toggle>
                <TabsList className="flex-1 ml-2">
                  <TabsTrigger value="2d">2D</TabsTrigger>
                  <TabsTrigger value="3d">3D</TabsTrigger>
                </TabsList>
              </div>

              {showVisualiserControls && (
                <div className=" flex flex-col  h-[80%] absolute bottom-0 right-0 w-[50%]  p-2 overflow-hidden">
                  <VisualiserControls />
                </div>
              )}
            </Tabs>
          </ErrorBoundary>
        </Card>
      </div>
      <div className="flex h-[600px] gap-page-gutter">
        <IOPanel />
        <EQPanel />
        <ListenPanel />
      </div>
    </div>
  );
};

export default Editor;
