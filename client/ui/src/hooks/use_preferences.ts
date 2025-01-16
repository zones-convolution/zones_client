import { useEffect, useState } from "react";

import {
  Preferences,
  getPreferences,
  defaultPreferences,
  addPreferencesPath,
  removePreferencesPath,
  revealPreferencesPath,
  getVersionData,
  VersionData,
  defaultVersionData,
} from "@/ipc/preferences_ipc";

interface IUsePreferences {
  preferences: Preferences;
  versionData: VersionData;
  addPath: () => Promise<void>;
  removePath: (path: string) => Promise<void>;
  revealPath: (path: string) => Promise<void>;
}

const usePreferences = (): IUsePreferences => {
  const [preferences, setPreferences] =
    useState<Preferences>(defaultPreferences);
  const [versionData, setVersionData] =
    useState<VersionData>(defaultVersionData);

  useEffect(() => {
    getPreferences().then(setPreferences);
    getVersionData().then(setVersionData);
  }, []);

  const addPath = async () => {
    setPreferences(await addPreferencesPath());
  };

  const removePath = async (path: string) => {
    setPreferences(await removePreferencesPath(path));
  };

  return {
    preferences: preferences,
    versionData: versionData,
    addPath: addPath,
    removePath: removePath,
    revealPath: revealPreferencesPath,
  };
};

export { usePreferences };
