import { useEffect, useState } from "react";
import { z } from "zod";

import {
  addNativeEventListener,
  juce,
  removeNativeEventListener,
} from "@/lib/juce";

const Preferences = z
  .object({
    user_paths: z.array(z.string()),
  })
  .transform((preferences) => ({
    userPaths: preferences.user_paths,
  }));

type Preferences = z.infer<typeof Preferences>;

interface IUsePreferences {
  preferences: Preferences;
  addPath: () => Promise<void>;
  removePath: (path: string) => Promise<void>;
  revealPath: (path: string) => Promise<void>;
}

const addUserPathNative = juce.getNativeFunction("add_user_path_native");
const removeUserPathNative = juce.getNativeFunction("remove_user_path_native");
const revealUserPathNative = juce.getNativeFunction("reveal_user_path_native");
const getPreferencesNative = juce.getNativeFunction("get_preferences_native");
const onPreferencesUpdateNative = "on_preferences_updated_native";

const usePreferences = (): IUsePreferences => {
  const [preferences, setPreferences] = useState<Preferences>({
    userPaths: [],
  });

  const addPath = async () => {
    await addUserPathNative();
  };

  const removePath = async (path: string) => {
    await removeUserPathNative(path);
  };

  const revealPath = async (path: string) => {
    await revealUserPathNative(path);
  };

  const getPreferences = async () => {
    return getPreferencesNative();
  };

  const handleReceivePreferences = (data: any) => {
    try {
      setPreferences(Preferences.parse(JSON.parse(data)));
    } catch (err) {
      console.error("Failed to parse Preferences!", err);
    }
  };

  useEffect(() => {
    getPreferences().then(handleReceivePreferences);

    let registrationId = addNativeEventListener(
      onPreferencesUpdateNative,
      handleReceivePreferences,
    );

    return () => {
      removeNativeEventListener(registrationId);
    };
  }, []);

  return {
    preferences: preferences,
    addPath: addPath,
    removePath: removePath,
    revealPath: revealPath,
  };
};

export { usePreferences };
