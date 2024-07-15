import { Play, Pause, Repeat } from "lucide-react";
import { FC, ReactNode } from "react";

import { Button } from "@/components/ui/button";
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from "@/components/ui/select";

import { CategoryCard } from "@/components/cards/category_card";
import { Knob } from "@/components/knob";
import { Visualiser } from "@/components/visualiser/visualiser";
import { useControlParameterIndexUpdater } from "@/hooks/use_control_parameter_index_updater";
import { usePlayer } from "@/hooks/use_player";
import { Parameters } from "@/lib/parameters";

const Panel: FC<{ children: ReactNode }> = ({ children }) => (
  <div className="flex flex-col flex-1 gap-4 bg-card p-4 rounded-md w-full">
    {children}
  </div>
);

const PanelHeading: FC<{ children: ReactNode }> = ({ children }) => {
  return <div className="text-primary uppercase">{children}</div>;
};

const PanelContent: FC<{ children: ReactNode }> = ({ children }) => {
  return <div className="flex flex-row justify-around">{children}</div>;
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
        <Knob identifier={Parameters.ReverbTime} />
        <Knob identifier={Parameters.Resample} />
        <Knob identifier={Parameters.RoomSize} />
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
        <div className="flex gap-1">
          <Button
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
            variant={looping ? "default" : "ghost"}
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
        <Knob identifier={Parameters.PreDelay} />
        <Knob identifier={Parameters.PreDelay} />
      </PanelContent>
    </Panel>
  );
};
const Editor = () => {
  useControlParameterIndexUpdater();

  return (
    <div className="flex flex-col gap-0.5 h-full">
      <div className="flex flex-row gap-0.5 h-full">
        <MainPanel />
        <CategoryCard
          category="Canyon"
          imageUrl="https://picsum.photos/600"
          rt60={12.6}
        />
      </div>
      <div className="flex flex-row gap-0.5 h-full">
        <TimePanel />
        <Visualiser />
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
