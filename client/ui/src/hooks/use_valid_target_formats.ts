import { useEffect, useState } from "react";

import { TargetFormat } from "@/hooks/zone_metadata";
import {
  getValidTargetFormats,
  validTargetFormatsListener,
} from "@/ipc/valid_target_formats_ipc";
import { isAbortError } from "@/lib/abortable";

interface IUseValidTargetFormats {
  validTargetFormats: TargetFormat[];
}

const useValidTargetFormats = (): IUseValidTargetFormats => {
  const [validTargetFormats, setValidTargetFormats] = useState<TargetFormat[]>(
    [],
  );

  useEffect(() => {
    const controller = new AbortController();
    const unsubscribe = validTargetFormatsListener(setValidTargetFormats);

    getValidTargetFormats({ signal: controller.signal })
      .then(setValidTargetFormats)
      .catch((error) => {
        if (!isAbortError(error)) console.error(error);
      });

    return () => {
      controller.abort();
      unsubscribe();
    };
  }, []);

  return {
    validTargetFormats: validTargetFormats,
  };
};

export { useValidTargetFormats };
