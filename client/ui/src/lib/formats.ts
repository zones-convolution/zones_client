import { Target } from "@use-gesture/react";

import { IrMetadata, TargetFormat } from "@/hooks/zone_metadata";
import { validTargetFormatsListener } from "@/ipc/valid_target_formats_ipc";

const irSupportsMono = (ir: IrMetadata) => {
  return true;
};

const irSupportsStereo = (ir: IrMetadata) => {
  if (ir.channelFormat == "stereo") return true;
  return (
    ir.channelFormat == "mono" &&
    ir.positionMap?.right != undefined &&
    ir.positionMap.left != undefined
  );
};

const irSupportsTrueStereo = (ir: IrMetadata) => {
  return (
    ir.channelFormat == "stereo" &&
    ir.positionMap?.right != undefined &&
    ir.positionMap.left != undefined
  );
};

const irSupportsFoa = (ir: IrMetadata) => {
  return ir.channelFormat == "foa" || ir.channelFormat == "quadraphonic";
};

const irSupportsQuadraphonic = (ir: IrMetadata) => {
  return ir.channelFormat == "foa" || ir.channelFormat == "quadraphonic";
};

const irSupportsTarget = (ir: IrMetadata, target: TargetFormat) => {
  switch (target) {
    case "stereo":
      return irSupportsStereo(ir);
    case "trueStereo":
      return irSupportsTrueStereo(ir);
    case "foa":
      return irSupportsFoa(ir);
    case "mono":
      return irSupportsMono(ir);
    case "quadraphonic":
      return irSupportsQuadraphonic(ir);
  }
};

const getDefaultTarget = (
  validTargets: TargetFormat[],
): TargetFormat | undefined => {
  if (validTargets.includes("trueStereo")) return "trueStereo";
  return validTargets.at(0);
};

export { irSupportsTarget, getDefaultTarget };
