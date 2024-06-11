import { FC, ReactNode } from "react";

import { Knob } from "@/components/knob";
import { useControlParameterIndexUpdater } from "@/hooks/use_control_parameter_index_updater";

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
        <Knob identifier="input_parameter" />
        <Knob identifier="dry_wet_mix_parameter" />
        <Knob identifier="output_parameter" />
      </PanelContent>
    </Panel>
  );
};

const EQPanel = () => {
  return (
    <Panel>
      <PanelHeading>EQ</PanelHeading>
      <PanelContent>
        <Knob identifier="bass_parameter" />
        <Knob identifier="treble_parameter" />
      </PanelContent>
    </Panel>
  );
};

const MainPanel = () => {
  return (
    <Panel>
      <PanelHeading>MAIN</PanelHeading>
      <PanelContent>
        <Knob identifier="reverb_time_parameter" />
        <Knob identifier="resample_parameter" />
        <Knob identifier="room_size_parameter" />
      </PanelContent>
    </Panel>
  );
};

const ListenPanel = () => {
  return (
    <Panel>
      <PanelHeading>LISTEN</PanelHeading>
      <PanelContent>
        <Knob identifier="reverb_time_parameter" />
        <Knob identifier="resample_parameter" />
        <Knob identifier="room_size_parameter" />
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
