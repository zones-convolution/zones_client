import {
  IrMetadata,
  IrSelectionOptional,
  ZoneMetadata,
} from "@/hooks/zone_metadata";

const doesZoneMatchSelection = (
  zone: ZoneMetadata,
  selection: IrSelectionOptional,
) => {
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

export { doesZoneMatchSelection, doesIrMatchSelection };
