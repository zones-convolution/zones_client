import { Loader, MoreHorizontal, Play } from "lucide-react";
import { FC, useEffect, useState } from "react";

import { Badge } from "@/components/ui/badge";
import { Button } from "@/components/ui/button";
import {
  DropdownMenu,
  DropdownMenuCheckboxItem,
  DropdownMenuContent,
  DropdownMenuLabel,
  DropdownMenuSeparator,
  DropdownMenuTrigger,
} from "@/components/ui/dropdown-menu";
import {
  Table,
  TableBody,
  TableCell,
  TableHead,
  TableHeader,
  TableRow,
} from "@/components/ui/table";

import { useLoadContext } from "@/context/load_context";
import { useValidTargetFormats } from "@/hooks/use_valid_target_formats";
import {
  IrMetadata,
  PositionMap,
  TargetFormat,
  ZoneMetadata,
} from "@/hooks/zone_metadata";
import { getDefaultTarget, irSupportsTarget } from "@/lib/formats";
import { doesIrMatchSelection } from "@/lib/irs";
import { cn } from "@/lib/utils";

const SpeakerPositionBadge: FC<{
  positionMap?: PositionMap;
  disabled: boolean;
}> = ({ positionMap, disabled }) => {
  let positionTitle = "UNKNOWN";

  if (positionMap != undefined) {
    if (positionMap.centre) positionTitle = "C";
    if (positionMap.left && positionMap.right) positionTitle = "LR";
    if (positionMap.centre && positionMap.left && positionMap.right)
      positionTitle = "LCR";
  }

  return (
    <Badge
      variant="secondary"
      className={cn("uppercase", {
        "bg-muted-foreground": disabled,
      })}
    >
      {positionTitle}
    </Badge>
  );
};
const IrTableRow: FC<{ ir: IrMetadata; zone: ZoneMetadata }> = ({
  ir,
  zone,
}) => {
  const { load, currentIr, loadingIr } = useLoadContext();

  const { validTargetFormats } = useValidTargetFormats();

  const validTargets = validTargetFormats.filter((target) =>
    irSupportsTarget(ir, target),
  );

  const [target, setTarget] = useState<TargetFormat | undefined>();

  useEffect(() => {
    setTarget(getDefaultTarget(validTargets));
  }, [validTargetFormats]);

  let isLoadingIr = doesIrMatchSelection(zone, ir, loadingIr);
  let isCurrentIr =
    doesIrMatchSelection(zone, ir, currentIr) &&
    target == currentIr.irSelection?.targetFormat;

  const useIr = async () => {
    if (target)
      await load({
        zone: zone,
        ir: ir,
        targetFormat: target,
      });
  };

  const isDisabled = validTargets.length == 0;

  return (
    <TableRow>
      <TableCell
        className={cn("font-medium", {
          "text-muted-foreground": isDisabled,
        })}
      >
        {ir.title}
      </TableCell>
      <TableCell
        className={cn("font-medium", {
          "text-muted-foreground": isDisabled,
        })}
      >
        {ir.description}
      </TableCell>
      <TableCell>
        <Badge
          className={cn("uppercase", {
            "bg-muted-foreground": isDisabled,
          })}
        >
          {ir.channelFormat}
        </Badge>
      </TableCell>
      <TableCell>
        <SpeakerPositionBadge
          disabled={isDisabled}
          positionMap={ir.positionMap}
        />
      </TableCell>
      <TableCell>
        {!isDisabled && (
          <Button
            onClick={useIr}
            disabled={isLoadingIr || isCurrentIr}
            variant={"secondary"}
          >
            {isLoadingIr ? (
              <Loader className="w-4 h-4 animate-spin" />
            ) : (
              <>
                Use <Play className="w-4 h-4 ml-2" />
              </>
            )}
          </Button>
        )}
      </TableCell>
      <TableCell>
        <DropdownMenu>
          <DropdownMenuTrigger asChild>
            {!isDisabled && (
              <Button aria-haspopup="true" size="icon" variant="ghost">
                <MoreHorizontal className="h-4 w-4" />
                <span className="sr-only">Toggle menu</span>
              </Button>
            )}
          </DropdownMenuTrigger>
          <DropdownMenuContent>
            <DropdownMenuLabel>Target Format</DropdownMenuLabel>
            <DropdownMenuSeparator />
            {validTargets.map((validTarget) => {
              return (
                <DropdownMenuCheckboxItem
                  className="uppercase"
                  checked={validTarget == target}
                  onCheckedChange={(checked) => {
                    if (checked) setTarget(validTarget);
                  }}
                >
                  {validTarget}
                </DropdownMenuCheckboxItem>
              );
            })}
          </DropdownMenuContent>
        </DropdownMenu>
      </TableCell>
    </TableRow>
  );
};

const IrTable: FC<{ zone: ZoneMetadata }> = ({ zone }) => {
  const irs = zone.irs;

  return (
    <table className={cn("w-full caption-bottom text-sm")}>
      <TableHeader>
        <TableRow>
          <TableHead className="w-[100px]">Title</TableHead>
          <TableHead>Description</TableHead>
          <TableHead>Channel Format</TableHead>
          <TableHead>Positions</TableHead>
          <TableHead></TableHead>
          <TableHead></TableHead>
        </TableRow>
      </TableHeader>
      <TableBody>
        {irs.map((ir, index) => (
          <IrTableRow ir={ir} zone={zone} key={index} />
        ))}
      </TableBody>
    </table>
  );
};

export { IrTable };
