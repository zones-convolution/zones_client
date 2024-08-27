import { Pause, Play, Repeat, Search, Settings } from "lucide-react";
import React, { FC, ReactNode, useState } from "react";

import { Button } from "@/components/ui/button";
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

const Panel: FC<{ children: ReactNode }> = ({ children }) => (
  <div className="flex flex-col flex-1 gap-4 bg-card p-4 rounded-md w-full">
    {children}
  </div>
);

const PanelHeading: FC<{ children: ReactNode }> = ({ children }) => {
  return <div className="uppercase absolute">{children}</div>;
};

const PanelContent: FC<{ children: ReactNode }> = ({ children }) => {
  return <div className="flex flex-row justify-around h-full">{children}</div>;
};

const IOPanel = () => {
  return (
    <Panel>
      <PanelHeading>IO</PanelHeading>
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
      <PanelHeading>EQ</PanelHeading>
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
      <PanelHeading>MAIN</PanelHeading>
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
      <PanelHeading>LISTEN</PanelHeading>
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
              <SelectItem value="numbers">Numbers</SelectItem>
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
      <PanelHeading>TIME</PanelHeading>
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
      <div className="flex-1 h-full w-full relative flex items-end">
        <div className="absolute w-full h-full">
          <BannerImageGalleryItem
            imageUrl={getImageUrl(zone.zoneId, zone.coverImageId)}
          />
        </div>
        <div className="backdrop-blur bg-card/40 p-2 m-2 w-fit rounded-md h-fit">
          {zone?.title}
        </div>
      </div>
    );

  if (zone?.title)
    return (
      <div className="flex-1 h-full w-full relative flex items-end">
        <div
          className="absolute w-full h-full"
          style={{ backgroundImage: getUserZoneGradient(zone) }}
        />
        <div className="backdrop-blur bg-card/40 p-2 m-2 w-fit rounded-md h-fit">
          {zone?.title}
        </div>
      </div>
    );

  return (
    <div className="flex-1 bg-card">
      <div className="py-10 px-6 text-center">
        <h2 className="mt-6 mb-2 text-2xl">No Zone Loaded</h2>
        <span className="text-secondary">
          Head over to browse to get started!
        </span>
      </div>

      <div className="flex justify-center">
        <Button
          onClick={() => {
            setTab(ZoneTab.Browser);
          }}
        >
          Browse <Search className="w-4 h-4 ml-2" />
        </Button>
      </div>
    </div>
  );
};

const Editor = () => {
  useControlParameterIndexUpdater();

  const context = useVisualiserContext();
  const [showVisualiserControls, setShowVisualiserControls] =
    useState<boolean>(true);

  return (
    <div className="flex flex-col gap-0.5 h-full">
      <div className="flex flex-row gap-0.5 h-full">
        <MainPanel />
        <CurrentIrPanel />
      </div>
      <div className="flex flex-row gap-0.5 h-full">
        <TimePanel />
        <Tabs defaultValue="2d" className="relative flex-1">
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
          <TabsList className="absolute top-0 right-0 m-2">
            <TabsTrigger value="2d">2D</TabsTrigger>
            <TabsTrigger value="3d">3D</TabsTrigger>
          </TabsList>

          <Toggle
            aria-label="Visualiser Controls"
            className="absolute top-0 left-0 m-2"
            pressed={showVisualiserControls}
            onPressedChange={setShowVisualiserControls}
          >
            <Settings className="h-4 w-4" />
          </Toggle>
          {showVisualiserControls && (
            <div className="absolute bottom-0 right-0 w-[50%] p-2">
              <VisualiserControls />
            </div>
          )}
        </Tabs>
      </div>
      <div className="flex gap-0.5 h-[600px]">
        <IOPanel />
        <EQPanel />
        <ListenPanel />
      </div>
    </div>
  );
};

export default Editor;
