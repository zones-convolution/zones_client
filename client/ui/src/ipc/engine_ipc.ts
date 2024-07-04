import { z } from "zod";

import {
  addNativeEventListener,
  juce,
  removeNativeEventListener,
} from "@/lib/juce";

const EngineLoading = z.object({
  convolutionEngineLoading: z.boolean(),
  irEngineLoading: z.boolean(),
});

export type EngineLoading = z.infer<typeof EngineLoading>;

const getEngineLoadingNative = juce.getNativeFunction(
  "get_engine_loading_native",
);
const onEngineLoadingUpdated = "on_engine_loading_updated";

export const defaultEngineLoading: EngineLoading = {
  convolutionEngineLoading: false,
  irEngineLoading: false,
};

const handleReceiveEngineLoading = (data: any) => {
  try {
    return EngineLoading.parse(JSON.parse(data));
  } catch (err) {
    console.error("Failed to parse EngineIPC!", err);
  }

  return defaultEngineLoading;
};

export const getEngineLoading = async () => {
  return handleReceiveEngineLoading(await getEngineLoadingNative());
};

export const engineUpdateListener = (
  onUpdate: (engineLoading: EngineLoading) => void,
) => {
  const listener = addNativeEventListener(onEngineLoadingUpdated, (data: any) =>
    onUpdate(handleReceiveEngineLoading(data)),
  );

  return () => {
    removeNativeEventListener(listener);
  };
};
