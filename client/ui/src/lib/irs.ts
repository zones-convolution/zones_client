import {
  IrMetadata,
  IrSelection,
  IrSelectionOptional,
  TargetFormat,
  ZoneMetadata,
} from "@/hooks/zone_metadata";
import { irSupportsTarget } from "@/lib/formats";

const doesZoneMatchSelection = (
  zone: ZoneMetadata,
  selection: IrSelectionOptional,
) => {
  // Should write real equality operator here... fine-ish for now xD
  return selection.irSelection?.zone.title === zone.title;
};

const doesIrMatchSelection = (
  zone: ZoneMetadata,
  ir: IrMetadata,
  selection: IrSelectionOptional,
) => {
  return (
    doesZoneMatchSelection(zone, selection) &&
    selection.irSelection?.ir.title === ir.title
  );
};

const getDefaultIrSelection = (
  zone: ZoneMetadata,
  targetFormats: TargetFormat[],
): IrSelection | undefined => {
  for (const ir of zone.irs) {
    for (const target of targetFormats) {
      if (irSupportsTarget(ir, target))
        return {
          zone: zone,
          ir: ir,
          targetFormat: target,
        };
    }
  }
};

export { doesZoneMatchSelection, doesIrMatchSelection, getDefaultIrSelection };
