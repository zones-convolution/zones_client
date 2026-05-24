import { juce } from "@/lib/juce";

const openExternalUrlNative = juce.getNativeFunction(
  "open_external_url_native",
);

export const openExternalUrl = async (url: string) => {
  await openExternalUrlNative(JSON.stringify(url));
};
