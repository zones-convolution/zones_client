import { useEffect, useState } from "react";

import {
  defaultEngineLoading,
  EngineLoading,
  engineUpdateListener,
  getEngineLoading,
} from "@/ipc/engine_ipc";
import { isAbortError } from "@/lib/abortable";

const useEngineLoading = (): EngineLoading => {
  const [engineLoading, setEngineLoading] =
    useState<EngineLoading>(defaultEngineLoading);

  useEffect(() => {
    const controller = new AbortController();
    const unsubscribe = engineUpdateListener(setEngineLoading);

    getEngineLoading({ signal: controller.signal })
      .then(setEngineLoading)
      .catch((error) => {
        if (!isAbortError(error)) console.error(error);
      });

    return () => {
      controller.abort();
      unsubscribe();
    };
  }, []);

  return engineLoading;
};

export { useEngineLoading };
