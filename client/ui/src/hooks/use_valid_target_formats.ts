import { useEffect, useState } from "react";
import { z } from "zod";

import { TargetFormat } from "@/hooks/zone_metadata";
import {
  addNativeEventListener,
  juce,
  removeNativeEventListener,
} from "@/lib/juce";

interface IUseValidTargetFormats {
  validTargetFormats: TargetFormat[];
}

const getValidTargetFormatsNative = juce.getNativeFunction(
  "get_valid_target_formats_native",
);

const onValidTargetFormatsUpdatedNative =
  "on_valid_target_formats_updated_native";

const useValidTargetFormats = (): IUseValidTargetFormats => {
  const [validTargetFormats, setValidTargetFormats] = useState<TargetFormat[]>(
    [],
  );

  const getValidTargetFormats = async () => {
    return getValidTargetFormatsNative();
  };

  useEffect(() => {
    getValidTargetFormats().then(handleReceiveValidTargetFormats);

    let registrationId = addNativeEventListener(
      onValidTargetFormatsUpdatedNative,
      handleReceiveValidTargetFormats,
    );

    return () => {
      removeNativeEventListener(registrationId);
    };
  }, []);

  const handleReceiveValidTargetFormats = (data: any) => {
    try {
      setValidTargetFormats(z.array(TargetFormat).parse(JSON.parse(data)));
    } catch (err) {
      console.error("Failed to parse valid target formats!", err);
    }
  };

  return {
    validTargetFormats: validTargetFormats,
  };
};

export { useValidTargetFormats };
