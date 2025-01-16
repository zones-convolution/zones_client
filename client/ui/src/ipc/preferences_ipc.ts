import { z } from "zod";

import { juce } from "@/lib/juce";

const Preferences = z.object({
  userPaths: z.array(z.string()),
});

const VersionData = z.object({
  versionNumber: z.string(),
  buildType: z.string(),
});

export type Preferences = z.infer<typeof Preferences>;
export type VersionData = z.infer<typeof VersionData>;

const addUserPathNative = juce.getNativeFunction("add_user_path_native");
const removeUserPathNative = juce.getNativeFunction("remove_user_path_native");
const revealUserPathNative = juce.getNativeFunction("reveal_user_path_native");
const getPreferencesNative = juce.getNativeFunction("get_preferences_native");

const getVersionDataNative = juce.getNativeFunction("get_version_data_native");

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

export const defaultVersionData: VersionData = {
  versionNumber: "",
  buildType: "",
};
const handleReceiveVersionData = (data: any) => {
  try {
    return VersionData.parse(JSON.parse(data));
  } catch (err) {
    console.error("Failed to parse VersionIPC!", err);
  }

  return defaultVersionData;
};

export const getVersionData = async () => {
  return handleReceiveVersionData(await getVersionDataNative());
};
