import { z } from "zod";

import { IrSelection, IrSelectionOptional } from "@/hooks/zone_metadata";
import {
  addNativeEventListener,
  juce,
  removeNativeEventListener,
} from "@/lib/juce";

const loadIrNative = juce.getNativeFunction("load_ir_native");
const getLoadingIrNative = juce.getNativeFunction("get_loading_ir_native");
const getCurrentIrNative = juce.getNativeFunction("get_current_ir_native");
const onLoadingIrUpdatedNative = "on_loading_ir_updated_native";
const onCurrentIrUpdatedNative = "on_current_ir_updated_native";

const handleReceiveIrOptional = (data: any): IrSelectionOptional => {
  try {
    return IrSelectionOptional.parse(JSON.parse(data));
  } catch (err) {
    console.error("Failed to parse IrOptionalIPC!", err);
  }

  return {};
};

export const getLoadingIr = async () => {
  return handleReceiveIrOptional(await getLoadingIrNative());
};

export const getCurrentIr = async () => {
  return handleReceiveIrOptional(await getCurrentIrNative());
};

export const loadIr = async (irSelection: IrSelection): Promise<boolean> => {
  try {
    return z.boolean().parse(await loadIrNative(JSON.stringify(irSelection)));
  } catch (err) {
    console.error("Failed to parse loadIrIPC", err);
  }

  return false;
};

export const loadingIrUpdateListener = (
  onUpdate: (loadingIr: IrSelectionOptional) => void,
) => {
  const listener = addNativeEventListener(
    onLoadingIrUpdatedNative,
    (data: any) => onUpdate(handleReceiveIrOptional(data)),
  );

  return () => {
    removeNativeEventListener(listener);
  };
};

export const currentIrUpdateListener = (
  onUpdate: (loadingIr: IrSelectionOptional) => void,
) => {
  const listener = addNativeEventListener(
    onCurrentIrUpdatedNative,
    (data: any) => onUpdate(handleReceiveIrOptional(data)),
  );

  return () => {
    removeNativeEventListener(listener);
  };
};
