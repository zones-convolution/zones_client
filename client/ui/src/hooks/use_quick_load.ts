import { useCallback } from "react";

import { useLoadContext } from "@/context/load_context";
import { useValidTargetFormats } from "@/hooks/use_valid_target_formats";
import {
  ChannelFormat,
  IrMetadata,
  TargetFormat,
  ZoneMetadata,
} from "@/hooks/zone_metadata";
import { juce } from "@/lib/juce";

const chooseIrPathNative = juce.getNativeFunction("choose_ir_path_native");
const getChannelFormat = (targetFormat: TargetFormat): ChannelFormat => {
  //Future high order target formats similar true stereo should be resolved here to ensure a single file channel format is used
  return targetFormat === "trueStereo" ? "stereo" : targetFormat;
};
export const useQuickLoad = () => {
  const { load } = useLoadContext();
  const { validTargetFormats } = useValidTargetFormats();

  const targetFormat = validTargetFormats[0];

  return useCallback(async () => {
    if (!targetFormat) {
      return;
    }

    const path = await chooseIrPathNative();

    if (!path) {
      return;
    }

    const file = path.split("/").pop() ?? "";
    const fileName = file.substring(0, file.lastIndexOf("."));
    const folderName = path.split("/").slice(-2, -1)[0];

    const channelFormat = getChannelFormat(targetFormat);

    const ir: IrMetadata = {
      title: fileName,
      relativePath: "",
      channelFormat,
      positionMap: {
        centre: "",
      },
    };

    const zoneMetadata: ZoneMetadata = {
      zoneType: "user",
      title: folderName,
      pathAttribute: path,
      images: [],
      irs: [ir],
    };

    await load({
      zone: zoneMetadata,
      ir,
      targetFormat,
    });
  }, [load, targetFormat]);
};
