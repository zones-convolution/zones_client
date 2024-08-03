import { z } from "zod";

import {
  addNativeEventListener,
  juce,
  removeNativeEventListener,
} from "@/lib/juce";

const VisualiserRender = z.array(z.array(z.number()));
export type VisualiserRender = z.infer<typeof VisualiserRender>;

const getVisualiserRenderNative = juce.getNativeFunction(
  "get_visualiser_render_native",
);
const onVisualiserRenderNative = "on_visualiser_render_native";

const handleReceiveVisualiserRender = (data: any) => {
  try {
    return VisualiserRender.parse(JSON.parse(data));
  } catch (err) {
    console.error("Failed to parse VisualiserIPC!", err);
  }

  return [];
};

export const getVisualiserRender = async () => {
  return handleReceiveVisualiserRender(await getVisualiserRenderNative());
};

export const visualiserRenderListener = (
  onUpdate: (state: VisualiserRender) => void,
) => {
  const listener = addNativeEventListener(
    onVisualiserRenderNative,
    (data: any) => onUpdate(handleReceiveVisualiserRender(data)),
  );

  return () => {
    removeNativeEventListener(listener);
  };
};
