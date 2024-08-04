import {
  addNativeEventListener,
  juce,
  removeNativeEventListener,
} from "@/lib/juce";

const onVisualiserRenderNative = "on_visualiser_render_native";

export const getVisualiserRender = async () => {
  const res = await fetch(juce.getBackendResourceAddress("visualiser.bin"));
  return new Uint8Array(await res.arrayBuffer());
};

export const visualiserRenderListener = (
  onUpdate: (state: Uint8Array) => void,
) => {
  const listener = addNativeEventListener(
    onVisualiserRenderNative,
    async (data: any) => {
      onUpdate(await getVisualiserRender());
    },
  );

  return () => {
    removeNativeEventListener(listener);
  };
};
