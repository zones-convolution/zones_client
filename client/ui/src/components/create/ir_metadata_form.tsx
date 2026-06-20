import { Folder, MinusCircle } from "lucide-react";
import { FC, useState } from "react";
import { Controller, useFormContext, useWatch } from "react-hook-form";

import { Button } from "@/components/ui/button";
import {
  Field,
  FieldError,
  FieldLabel,
} from "@/components/ui/field";
import { Input } from "@/components/ui/input";
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from "@/components/ui/select";
import { Textarea } from "@/components/ui/textarea";

import { CreateZoneSchema } from "@/ipc/create_zone_ipc";
import { juce } from "@/lib/juce";

type SpeakerPosition = "C" | "LR" | "CLR";

const SpeakerPositionPicker: FC<{
  position: string;
  path?: string;
  onChoose: () => void;
}> = ({ position, path, onChoose }) => {
  return (
    <div className="flex flex-row items-center">
      {position}
      <span className="ml-2 font-thin">{path}</span>
      <Button className="ml-auto" onClick={onChoose} type="button">
        Choose <Folder className="w-4 h-4 ml-2" />
      </Button>
    </div>
  );
};

const chooseIrPathNative = juce.getNativeFunction("choose_ir_path_native");

const IrMetadataForm: FC<{
  index: number;
  remove: () => void;
}> = ({ index, remove }) => {
  const { control, setValue, getValues } = useFormContext<CreateZoneSchema>();
  const [speakerPosition, setSpeakerPosition] = useState<SpeakerPosition>("C");

  const positionMap = useWatch({
    control: control,
    name: `metadata.irs.${index}.positionMap`,
  });

  const lr = (
    <>
      <SpeakerPositionPicker
        position="Left"
        path={positionMap && positionMap.left}
        onChoose={async () => {
          setValue(
            `metadata.irs.${index}.positionMap.left`,
            await chooseIrPathNative(),
          );
        }}
      />
      <SpeakerPositionPicker
        position="Right"
        path={positionMap && positionMap.right}
        onChoose={async () => {
          setValue(
            `metadata.irs.${index}.positionMap.right`,
            await chooseIrPathNative(),
          );
        }}
      />
    </>
  );

  const c = (
    <SpeakerPositionPicker
      position="Centre"
      path={positionMap && positionMap.centre}
      onChoose={async () => {
        setValue(
          `metadata.irs.${index}.positionMap.centre`,
          await chooseIrPathNative(),
        );
      }}
    />
  );

  let speakerPositionsPicker;
  switch (speakerPosition) {
    case "C":
      speakerPositionsPicker = c;
      break;
    case "LR":
      speakerPositionsPicker = lr;
      break;
    case "CLR":
      speakerPositionsPicker = (
        <>
          {c}
          {lr}
        </>
      );
      break;
  }

  return (
    <div className="flex flex-col gap-2">
      <Controller
        name={`metadata.irs.${index}.title`}
        control={control}
        render={({ field, fieldState }) => (
          <Field data-invalid={fieldState.invalid}>
            <FieldLabel htmlFor={field.name}>Title</FieldLabel>
            <Input
              id={field.name}
              placeholder="Title"
              {...field}
              aria-invalid={fieldState.invalid}
            />
            <FieldError errors={[fieldState.error]} />
          </Field>
        )}
      />
      <Controller
        name={`metadata.irs.${index}.description`}
        control={control}
        render={({ field, fieldState }) => (
          <Field data-invalid={fieldState.invalid}>
            <FieldLabel htmlFor={field.name}>Description</FieldLabel>
            <Textarea
              id={field.name}
              placeholder="Description"
              {...field}
              aria-invalid={fieldState.invalid}
            />
            <FieldError errors={[fieldState.error]} />
          </Field>
        )}
      />

      <Controller
        name={`metadata.irs.${index}.channelFormat`}
        control={control}
        render={({ field, fieldState }) => (
          <Field data-invalid={fieldState.invalid}>
            <FieldLabel htmlFor={field.name}>Channel Format</FieldLabel>
              <Select onValueChange={field.onChange} value={field.value}>
                <SelectTrigger id={field.name} aria-invalid={fieldState.invalid}>
                  <SelectValue />
                </SelectTrigger>
                <SelectContent>
                  <SelectItem value="foa">FOA</SelectItem>
                  <SelectItem value="mono">Mono</SelectItem>
                  <SelectItem value="stereo">Stereo</SelectItem>
                  <SelectItem value="quadraphonic">Quadraphonic</SelectItem>
                </SelectContent>
              </Select>
            <FieldError errors={[fieldState.error]} />
          </Field>
        )}
      />

      <Field>
        <FieldLabel>Speaker Positions</FieldLabel>
          <Select
            value={speakerPosition}
            onValueChange={(value) => {
              setSpeakerPosition(value as SpeakerPosition);
            }}
          >
            <SelectTrigger>
              <SelectValue />
            </SelectTrigger>
            <SelectContent>
              <SelectItem value="C">C</SelectItem>
              <SelectItem value="LR">LR</SelectItem>
              <SelectItem value="CLR">CLR</SelectItem>
            </SelectContent>
          </Select>
      </Field>

      {speakerPositionsPicker}

      <Button
        className="w-full"
        variant="destructive"
        onClick={remove}
        type="button"
      >
        Remove Ir <MinusCircle className="w-4 h-4 ml-2" />
      </Button>
    </div>
  );
};

export { IrMetadataForm };
