import { Knob } from "@/components/knob";
import { useControlParameterIndexUpdater } from "@/hooks/use_control_parameter_index_updater";

const IOPanel = () => {
  return (
    <div className="flex flex-col flex-grow gap-2 bg-card p-4 rounded-md">
      IO
      <div className="flex flex-row justify-around gap-2">
        <Knob identifier="input_parameter" />
        <Knob identifier="dry_wet_mix_parameter" />
        <Knob identifier="output_parameter" />
      </div>
    </div>
  );
};

const EQPanel = () => {
  return (
    <div className="flex flex-col flex-grow gap-2 bg-card p-4 rounded-md">
      EQ
      <div className="flex flex-row justify-around gap-2">
        <Knob identifier="bass_parameter" />
        <Knob identifier="treble_parameter" />
      </div>
    </div>
  );
};

const MainPanel = () => {
  return (
    <div className="flex flex-col flex-grow gap-2 bg-card p-4 rounded-md">
      MAIN
      <div className="flex flex-row justify-around gap-2">
        <Knob identifier="reverb_time_parameter" />
        <Knob identifier="resample_parameter" />
        <Knob identifier="room_size_parameter" />
      </div>
    </div>
  );
};

const ListenPanel = () => {
  return (
    <div className="flex flex-col flex-grow gap-2 bg-card p-4 rounded-md">
      LISTEN
      <div className="flex flex-row justify-around gap-2">
        <Knob identifier="reverb_time_parameter" />
        <Knob identifier="resample_parameter" />
        <Knob identifier="room_size_parameter" />
      </div>
    </div>
  );
};

const TimePanel = () => {
  return (
    <div className="flex flex-col flex-grow gap-2 bg-card p-4 rounded-md">
      TIME
      <div className="flex flex-row justify-around gap-2">
        <Knob identifier="IDENTIFIER" />
        <Knob identifier="IDENTIFIER" />
      </div>
    </div>
  );
};
const Editor = () => {
  useControlParameterIndexUpdater();

  return (
    <div className="flex flex-col gap-0.5">
      <div className="flex flex-row gap-0.5">
        <MainPanel />
        <div className="bg-amber-200 flex-grow" />
      </div>
      <div className="flex flex-row gap-0.5">
        <TimePanel />
        <div className="bg-amber-200 flex-grow" />
      </div>
      <div className="flex flex-row gap-0.5">
        <IOPanel />
        <EQPanel />
        <ListenPanel />
      </div>
    </div>
  );
};

export default Editor;
