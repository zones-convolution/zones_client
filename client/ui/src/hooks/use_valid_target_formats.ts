import { useEffect, useState } from "react";

import { TargetFormat } from "@/hooks/zone_metadata";
import {
  getValidTargetFormats,
  validTargetFormatsListener,
} from "@/ipc/valid_target_formats_ipc";

interface IUseValidTargetFormats {
  validTargetFormats: TargetFormat[];
}

const useValidTargetFormats = (): IUseValidTargetFormats => {
  const [validTargetFormats, setValidTargetFormats] = useState<TargetFormat[]>(
    [],
  );

  useEffect(() => {
    getValidTargetFormats().then(setValidTargetFormats);
    return validTargetFormatsListener(setValidTargetFormats);
  }, []);

  return {
    validTargetFormats: validTargetFormats,
  };
};

export { useValidTargetFormats };
