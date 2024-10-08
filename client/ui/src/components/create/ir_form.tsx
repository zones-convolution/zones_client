import { PlusCircle } from "lucide-react";
import { useFieldArray, useFormContext } from "react-hook-form";

import { Button } from "@/components/ui/button";

import { IrMetadataForm } from "@/components/create/ir_metadata_form";
import { CreateZoneSchema } from "@/ipc/create_zone_ipc";

const IrForm = () => {
  const { control } = useFormContext<CreateZoneSchema>();
  const { fields, append, remove } = useFieldArray({
    control,
    name: "metadata.irs",
  });

  return (
    <div className="flex flex-col gap-2">
      {fields.map((field, index) => (
        <IrMetadataForm
          key={field.id}
          index={index}
          remove={() => remove(index)}
        />
      ))}
      <Button
        type="button"
        className="w-full"
        variant="secondary"
        onClick={() => append({ relativePath: "", channelFormat: "stereo" })}
      >
        Add Ir <PlusCircle className="w-4 h-4 ml-2" />
      </Button>
    </div>
  );
};

export { IrForm };
