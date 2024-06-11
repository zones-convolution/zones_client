import { Knob } from "@/components/knob";
import { useControlParameterIndexUpdater } from "@/hooks/use_control_parameter_index_updater";

const Editor = () => {
  useControlParameterIndexUpdater();

  return (
    <div className="flex flex-row justify-around p-4 bg-card">
      <Knob identifier="dry_wet_mix_parameter" />
    </div>
  );
};

export default Editor;
