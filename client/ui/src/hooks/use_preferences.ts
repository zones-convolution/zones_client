import { useEffect, useState } from "react";

import {
  Preferences,
  getPreferences,
  defaultPreferences,
  addPreferencesPath,
  removePreferencesPath,
  revealPreferencesPath,
} from "@/ipc/preferences_ipc";

interface IUsePreferences {
  preferences: Preferences;
  addPath: () => Promise<void>;
  removePath: (path: string) => Promise<void>;
  revealPath: (path: string) => Promise<void>;
}

const usePreferences = (): IUsePreferences => {
  const [preferences, setPreferences] =
    useState<Preferences>(defaultPreferences);

  useEffect(() => {
    getPreferences().then(setPreferences);
  }, []);

  const addPath = async () => {
    setPreferences(await addPreferencesPath());
  };

  const removePath = async (path: string) => {
    setPreferences(await removePreferencesPath(path));
  };

  return {
    preferences: preferences,
    addPath: addPath,
    removePath: removePath,
    revealPath: revealPreferencesPath,
  };
};

export { usePreferences };
