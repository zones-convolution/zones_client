import { z } from "zod";

import { juce } from "@/lib/juce";

const Preferences = z.object({
  userPaths: z.array(z.string()),
});

const VersionData = z.object({
  versionNumber: z.string(),
  buildType: z.string(),
});

const BlockSizes = z.object({
  maximum: z.number(),
  current: z.number(),
});

export type Preferences = z.infer<typeof Preferences>;
export type VersionData = z.infer<typeof VersionData>;
export type BlockSizes = z.infer<typeof BlockSizes>;

const addUserPathNative = juce.getNativeFunction("add_user_path_native");
const removeUserPathNative = juce.getNativeFunction("remove_user_path_native");
const revealUserPathNative = juce.getNativeFunction("reveal_user_path_native");
const getPreferencesNative = juce.getNativeFunction("get_preferences_native");

const getVersionDataNative = juce.getNativeFunction("get_version_data_native");

const getBlockSizesNative = juce.getNativeFunction("get_block_sizes_native");
const setInternalBlockSizeNative = juce.getNativeFunction(
  "set_internal_block_size_native",
);

export const defaultPreferences: Preferences = {
  userPaths: [],
};
export const defaultVersionData: VersionData = {
  versionNumber: "",
  buildType: "",
};
export const defaultBlockSizes: BlockSizes = {
  maximum: 0,
  current: 0,
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

const handleReceiveBlockSizes = (data: any) => {
  try {
    return BlockSizes.parse(JSON.parse(data));
  } catch (err) {
    console.error("Failed to parse BlockSizesIPC!", err);
  }

  return defaultBlockSizes;
};

export const getBlockSizes = async () => {
  return handleReceiveBlockSizes(await getBlockSizesNative());
};
export const setInternalBlockSize = async (blockSize: number) => {
  await setInternalBlockSizeNative(JSON.stringify(blockSize.toString()));
};
