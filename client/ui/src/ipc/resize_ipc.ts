import { juce } from "@/lib/juce";

const beginResizeNative = juce.getNativeFunction("begin_resize_native");
const endResizeNative = juce.getNativeFunction("end_resize_native");
const resizeNative = juce.getNativeFunction("resize_native");

export const beginResize = async (x: number, y: number): Promise<void> => {
  await beginResizeNative(JSON.stringify({ x: x, y: y }));
};

export const endResize = async (): Promise<void> => {
  await endResizeNative();
};

export const resize = async (x: number, y: number): Promise<void> => {
  await resizeNative(JSON.stringify({ x: x, y: y }));
};
