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
  BlockSizes,
  defaultBlockSizes,
  setInternalBlockSize,
  getBlockSizes,
  blockSizeUpdateListener,
} from "@/ipc/preferences_ipc";
import { isAbortError } from "@/lib/abortable";

interface IUsePreferences {
  preferences: Preferences;
  versionData: VersionData;
  blockSizes: BlockSizes;
  setBlockSize: (blockSize: number) => Promise<void>;
  addPath: () => Promise<void>;
  removePath: (path: string) => Promise<void>;
  revealPath: (path: string) => Promise<void>;
}

const usePreferences = (): IUsePreferences => {
  const [preferences, setPreferences] =
    useState<Preferences>(defaultPreferences);
  const [versionData, setVersionData] =
    useState<VersionData>(defaultVersionData);
  const [blockSizes, setBlockSizes] = useState<BlockSizes>(defaultBlockSizes);

  useEffect(() => {
    const controller = new AbortController();
    const unsubscribe = blockSizeUpdateListener(setBlockSizes);

    getPreferences({ signal: controller.signal })
      .then(setPreferences)
      .catch((error) => {
        if (!isAbortError(error)) console.error(error);
      });

    getVersionData({ signal: controller.signal })
      .then(setVersionData)
      .catch((error) => {
        if (!isAbortError(error)) console.error(error);
      });

    getBlockSizes({ signal: controller.signal })
      .then(setBlockSizes)
      .catch((error) => {
        if (!isAbortError(error)) console.error(error);
      });

    return () => {
      controller.abort();
      unsubscribe();
    };
  }, []);

  const addPath = async () => {
    setPreferences(await addPreferencesPath());
  };

  const removePath = async (path: string) => {
    setPreferences(await removePreferencesPath(path));
  };

  const setBlockSize = async (blockSize: number) => {
    await setInternalBlockSize(blockSize);
  };

  return {
    preferences: preferences,
    versionData: versionData,
    blockSizes: blockSizes,
    setBlockSize: setBlockSize,
    addPath: addPath,
    removePath: removePath,
    revealPath: revealPreferencesPath,
  };
};

export { usePreferences };
