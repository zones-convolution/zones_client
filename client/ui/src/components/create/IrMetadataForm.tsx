import { Folder, MinusCircle } from "lucide-react";
import { FC, useState } from "react";
import { useFormContext, useFormState, useWatch } from "react-hook-form";

import { Button } from "@/components/ui/button";
import {
  FormControl,
  FormField,
  FormItem,
  FormLabel,
  FormMessage,
  useFormField,
} from "@/components/ui/form";
import { Input } from "@/components/ui/input";
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from "@/components/ui/select";
import { Textarea } from "@/components/ui/textarea";

import { CreateZoneSchema } from "@/create_zone";
import { IrMetadata } from "@/hooks/zone_metadata";
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
      <Button className="ml-auto" onClick={onChoose}>
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
      <FormField
        control={control}
        name={`metadata.irs.${index}.title`}
        render={({ field }) => (
          <FormItem>
            <FormLabel>Title</FormLabel>
            <FormControl>
              <Input placeholder="Title" {...field} />
            </FormControl>
            <FormMessage />
          </FormItem>
        )}
      />
      <FormField
        control={control}
        name={`metadata.irs.${index}.description`}
        render={({ field }) => (
          <FormItem>
            <FormLabel>Description</FormLabel>
            <FormControl>
              <Textarea placeholder="Description" {...field} />
            </FormControl>
            <FormMessage />
          </FormItem>
        )}
      />

      <FormField
        control={control}
        name={`metadata.irs.${index}.channelFormat`}
        render={({ field }) => (
          <FormItem>
            <FormLabel>Channel Format</FormLabel>
            <FormControl>
              <Select onValueChange={field.onChange} value={field.value}>
                <FormControl>
                  <SelectTrigger>
                    <SelectValue />
                  </SelectTrigger>
                </FormControl>
                <SelectContent>
                  <SelectItem value="foa">FOA</SelectItem>
                  <SelectItem value="mono">Mono</SelectItem>
                  <SelectItem value="stereo">Stereo</SelectItem>
                  <SelectItem value="quadraphonic">Quadraphonic</SelectItem>
                </SelectContent>
              </Select>
            </FormControl>
            <FormMessage />
          </FormItem>
        )}
      />

      <FormItem>
        <FormLabel>Speaker Positions</FormLabel>
        <FormControl>
          <Select
            value={speakerPosition}
            onValueChange={(value) => {
              setSpeakerPosition(value as SpeakerPosition);
            }}
          >
            <FormControl>
              <SelectTrigger>
                <SelectValue />
              </SelectTrigger>
            </FormControl>
            <SelectContent>
              <SelectItem value="C">C</SelectItem>
              <SelectItem value="LR">LR</SelectItem>
              <SelectItem value="CLR">CLR</SelectItem>
            </SelectContent>
          </Select>
        </FormControl>
        <FormMessage />
      </FormItem>

      {speakerPositionsPicker}

      <Button className="w-full" variant="destructive" onClick={remove}>
        Remove Ir <MinusCircle className="w-4 h-4 ml-2" />
      </Button>
    </div>
  );
};

export { IrMetadataForm };
