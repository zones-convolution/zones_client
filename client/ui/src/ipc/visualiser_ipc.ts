import { defaultVersionData, VersionData } from "@/ipc/preferences_ipc";
import {
  addNativeEventListener,
  juce,
  removeNativeEventListener,
} from "@/lib/juce";

const onVisualiserRenderNative = "on_visualiser_render_native";
const getSampleRateNative = juce.getNativeFunction("get_sample_rate_native");

export const getVisualiserRender = async () => {
  const res = await fetch(juce.getBackendResourceAddress("visualiser.bin"));
  return new Uint8Array(await res.arrayBuffer());
};

const handleReceiveSampleRate = (data: any) => {
  try {
    return JSON.parse(data);
  } catch (err) {
    console.error("Failed to parse SampleRateIPC!", err);
  }

  return 48000;
};

export const getSampleRate = async () => {
  return handleReceiveSampleRate(await getSampleRateNative());
};
export const visualiserRenderListener = (
  onRenderUpdate: (state: Uint8Array) => void,
  onSampleRateUpdate: (state: number) => void,
) => {
  const listener = addNativeEventListener(
    onVisualiserRenderNative,
    async (data: any) => {
      onRenderUpdate(await getVisualiserRender());
      onSampleRateUpdate(await getSampleRate());
    },
  );

  return () => {
    removeNativeEventListener(listener);
  };
};
