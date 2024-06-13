import { Play, Pause, Repeat } from "lucide-react";
import { FC, ReactNode } from "react";

import { Button } from "@/components/ui/button";

import { Knob } from "@/components/knob";
import { useControlParameterIndexUpdater } from "@/hooks/use_control_parameter_index_updater";
import { usePlayer } from "@/hooks/use_player";
import { Parameters } from "@/lib/parameters";

const Panel: FC<{ children: ReactNode }> = ({ children }) => (
  <div className="flex flex-col flex-1 gap-8 bg-card p-4 rounded-md justify-center">
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
  const { togglePlaying, toggleLooping, playerState } = usePlayer();

  const { playing, looping } = playerState;

  return (
    <Panel>
      <PanelHeading>LISTEN</PanelHeading>
      <PanelContent>
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
      </PanelContent>
    </Panel>
  );
};

const TimePanel = () => {
  return (
    <Panel>
      <PanelHeading>TIME</PanelHeading>
      <PanelContent>
        <Knob identifier="IDENTIFIER" />
        <Knob identifier="IDENTIFIER" />
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
        <div className="bg-amber-200 flex-grow" />
      </div>
      <div className="flex flex-row gap-0.5 h-full">
        <TimePanel />
        <div className="bg-amber-200 flex-grow" />
      </div>
      <div className="flex flex-row gap-0.5 h-[1000px]">
        <IOPanel />
        <EQPanel />
        <ListenPanel />
      </div>
    </div>
  );
};

export default Editor;
