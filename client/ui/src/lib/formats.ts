import { Target } from "@use-gesture/react";

import { IrMetadata, TargetFormat } from "@/hooks/zone_metadata";

const irSupportsMono = (ir: IrMetadata) => {
  return true;
};

const irSupportsStereo = (ir: IrMetadata) => {
  if (ir.channelFormat == "stereo" || ir.channelFormat == "mono") return true;
};

const irSupportsTrueStereo = (ir: IrMetadata) => {
  return (
    ir.channelFormat == "stereo" &&
    ir.positionMap?.right != undefined &&
    ir.positionMap.left != undefined
  );
};

const irSupportsFoa = (ir: IrMetadata) => {
  return (
    ir.channelFormat == "foa" ||
    ir.channelFormat == "quadraphonic" ||
    ir.channelFormat == "mono"
  );
};

const irSupportsQuadraphonic = (ir: IrMetadata) => {
  return (
    ir.channelFormat == "foa" ||
    ir.channelFormat == "quadraphonic" ||
    ir.channelFormat == "mono"
  );
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
