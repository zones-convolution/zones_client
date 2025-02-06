import { z } from "zod";

import { defaultVersionData, VersionData } from "@/ipc/preferences_ipc";
import {
  addNativeEventListener,
  juce,
  removeNativeEventListener,
} from "@/lib/juce";

const VisualiserMetadata = z.object({
  sampleRate: z.number(),
  baseIrLengthSamples: z.number(),
});

export type VisualiserMetadata = z.infer<typeof VisualiserMetadata>;

const onVisualiserRenderNative = "on_visualiser_render_native";
const getVisualiserMetadataNative = juce.getNativeFunction(
  "get_visualiser_metadata_native",
);

export const getVisualiserRender = async () => {
  const res = await fetch(juce.getBackendResourceAddress("visualiser.bin"));
  return new Uint8Array(await res.arrayBuffer());
};

export const defaultVisualiserMetadata: VisualiserMetadata = {
  sampleRate: 48000,
  baseIrLengthSamples: 48000,
};

const handleReceiveVisualiserMetadata = (data: any) => {
  try {
    return VisualiserMetadata.parse(JSON.parse(data));
  } catch (err) {
    console.error("Failed to parse VisualiserMetadataIPC!", err);
  }

  return defaultVisualiserMetadata;
};

export const getVisualiserMetadata = async () => {
  return handleReceiveVisualiserMetadata(await getVisualiserMetadataNative());
};
export const visualiserRenderListener = (
  onRenderUpdate: (state: Uint8Array) => void,
  onVisualiserMetadataUpdate: (state: VisualiserMetadata) => void,
) => {
  const listener = addNativeEventListener(
    onVisualiserRenderNative,
    async (data: any) => {
      onRenderUpdate(await getVisualiserRender());
      onVisualiserMetadataUpdate(await getVisualiserMetadata());
    },
  );

  return () => {
    removeNativeEventListener(listener);
  };
};
