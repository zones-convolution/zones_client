import { z } from "zod";

import { TargetFormat } from "@/hooks/zone_metadata";
import {
  addNativeEventListener,
  juce,
  removeNativeEventListener,
} from "@/lib/juce";

const getValidTargetFormatsNative = juce.getNativeFunction(
  "get_valid_target_formats_native",
);

const onValidTargetFormatsUpdatedNative =
  "on_valid_target_formats_updated_native";

const handleReceiveValidTargetFormats = (data: any) => {
  try {
    return z.array(TargetFormat).parse(JSON.parse(data));
  } catch (err) {
    console.error("Failed to parse ValidTargetFormatsIPC!", err);
  }

  return [];
};

export const getValidTargetFormats = async () => {
  return handleReceiveValidTargetFormats(await getValidTargetFormatsNative());
};

export const validTargetFormatsListener = (
  onUpdate: (validTargetFormats: TargetFormat[]) => void,
) => {
  const listener = addNativeEventListener(
    onValidTargetFormatsUpdatedNative,
    (data: any) => onUpdate(handleReceiveValidTargetFormats(data)),
  );

  return () => {
    removeNativeEventListener(listener);
  };
};
