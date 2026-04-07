import { z } from "zod";

import {
  addNativeEventListener,
  juce,
  removeNativeEventListener,
} from "@/lib/juce";
import { isAbortError, withAbort } from "@/lib/abortable";

const VisualiserMetadata = z.object({
  sampleRate: z.number(),
  baseIrLengthSamples: z.number(),
});

export type VisualiserMetadata = z.infer<typeof VisualiserMetadata>;

const onVisualiserRenderNative = "on_visualiser_render_native";
const getVisualiserMetadataNative = juce.getNativeFunction(
  "get_visualiser_metadata_native",
);

export const getVisualiserRender = async (options?: { signal?: AbortSignal }) => {
  const res = await fetch(juce.getBackendResourceAddress("visualiser.bin"), {
    signal: options?.signal,
  });
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

export const getVisualiserMetadata = async (options?: { signal?: AbortSignal }) => {
  return handleReceiveVisualiserMetadata(
    await withAbort(getVisualiserMetadataNative(), options?.signal),
  );
};
export const visualiserRenderListener = (
  onRenderUpdate: (state: Uint8Array) => void,
  onVisualiserMetadataUpdate: (state: VisualiserMetadata) => void,
  options?: { signal?: AbortSignal },
) => {
  const listener = addNativeEventListener(
    onVisualiserRenderNative,
    async () => {
      try {
        const render = await getVisualiserRender(options);
        if (options?.signal?.aborted) return;
        onRenderUpdate(render);

        const metadata = await getVisualiserMetadata(options);
        if (options?.signal?.aborted) return;
        onVisualiserMetadataUpdate(metadata);
      } catch (error) {
        if (!isAbortError(error)) console.error(error);
      }
    },
  );

  return () => {
    removeNativeEventListener(listener);
  };
};
