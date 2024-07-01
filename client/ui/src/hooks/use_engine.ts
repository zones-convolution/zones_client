import { useEffect, useState } from "react";
import { z } from "zod";

import {
  addNativeEventListener,
  juce,
  removeNativeEventListener,
} from "@/lib/juce";

const EngineLoading = z.object({
  convolutionEngineLoading: z.boolean(),
  irEngineLoading: z.boolean(),
});

type EngineLoading = z.infer<typeof EngineLoading>;

const getEngineLoadingNative = juce.getNativeFunction(
  "get_engine_loading_native",
);
const onEngineLoadingUpdated = "on_engine_loading_updated";

const useEngineLoading = (): EngineLoading => {
  const [engineLoading, setEngineLoading] = useState<EngineLoading>({
    convolutionEngineLoading: false,
    irEngineLoading: false,
  });

  const getEngineLoading = async () => {
    return await getEngineLoadingNative();
  };

  const handleReceiveEngineLoading = (data: any) => {
    console.log("Engine loading state updated");
    try {
      setEngineLoading(EngineLoading.parse(JSON.parse(data)));
    } catch (err) {
      console.error("Failed to parse EngineLoading!", err);
    }
  };

  useEffect(() => {
    getEngineLoading().then(handleReceiveEngineLoading);

    let registrationId = addNativeEventListener(
      onEngineLoadingUpdated,
      handleReceiveEngineLoading,
    );

    return () => {
      removeNativeEventListener(registrationId);
    };
  }, []);

  return engineLoading;
};

export { useEngineLoading };
