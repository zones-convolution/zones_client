import { z } from "zod";

import { juce } from "@/lib/juce";

const Preferences = z.object({
  userPaths: z.array(z.string()),
});

export type Preferences = z.infer<typeof Preferences>;

const addUserPathNative = juce.getNativeFunction("add_user_path_native");
const removeUserPathNative = juce.getNativeFunction("remove_user_path_native");
const revealUserPathNative = juce.getNativeFunction("reveal_user_path_native");
const getPreferencesNative = juce.getNativeFunction("get_preferences_native");

export const defaultPreferences: Preferences = {
  userPaths: [],
};

const handleReceivePreferences = (data: any) => {
  try {
    return Preferences.parse(JSON.parse(data));
  } catch (err) {
    console.error("Failed to parse PreferencesIPC!", err);
  }

  return defaultPreferences;
};

export const getPreferences = async () => {
  return handleReceivePreferences(await getPreferencesNative());
};

export const addPreferencesPath = async () => {
  return handleReceivePreferences(await addUserPathNative());
};

export const removePreferencesPath = async (path: string) => {
  return handleReceivePreferences(await removeUserPathNative(path));
};

export const revealPreferencesPath = async (path: string) => {
  await revealUserPathNative(path);
};
