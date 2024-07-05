import { useEffect, useState } from "react";

import {
  defaultEngineLoading,
  EngineLoading,
  engineUpdateListener,
  getEngineLoading,
} from "@/ipc/engine_ipc";

const useEngineLoading = (): EngineLoading => {
  const [engineLoading, setEngineLoading] =
    useState<EngineLoading>(defaultEngineLoading);

  useEffect(() => {
    getEngineLoading().then(setEngineLoading);
    return engineUpdateListener(setEngineLoading);
  }, []);

  return engineLoading;
};

export { useEngineLoading };
