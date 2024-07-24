import { Play } from "lucide-react";
import { FC } from "react";

import { Badge } from "@/components/ui/badge";
import { Button } from "@/components/ui/button";
import {
  Table,
  TableBody,
  TableCell,
  TableHead,
  TableHeader,
  TableRow,
} from "@/components/ui/table";

import { IrMetadata, PositionMap, ZoneMetadata } from "@/hooks/zone_metadata";

const SpeakerPositionBadge: FC<{ positionMap?: PositionMap }> = ({
  positionMap,
}) => {
  let positionTitle = "UNKNOWN";

  if (positionMap != undefined) {
    if (positionMap.centre) positionTitle = "C";
    if (positionMap.left && positionMap.right) positionTitle = "LR";
    if (positionMap.centre && positionMap.left && positionMap.right)
      positionTitle = "LCR";
  }

  return (
    <Badge className="uppercase" variant="secondary">
      {positionTitle}
    </Badge>
  );
};

const IrTable: FC<{ irs: IrMetadata[] }> = ({ irs }) => {
  return (
    <Table>
      <TableHeader>
        <TableRow>
          <TableHead className="w-[100px]">Title</TableHead>
          <TableHead>Channel Format</TableHead>
          <TableHead>Positions</TableHead>
          <TableHead>Description</TableHead>
          <TableHead></TableHead>
        </TableRow>
      </TableHeader>
      <TableBody>
        {irs.map((ir, index) => (
          <TableRow key={index}>
            <TableCell className="font-medium">{ir.title}</TableCell>
            <TableCell>
              <Badge className="uppercase">{ir.channelFormat}</Badge>
            </TableCell>
            <TableCell>
              <SpeakerPositionBadge positionMap={ir.positionMap} />
            </TableCell>
            <TableCell>{ir.description}</TableCell>
            <TableCell>
              <Button>
                Use <Play className="w-4 h-4 ml-2" />
              </Button>
            </TableCell>
          </TableRow>
        ))}
      </TableBody>
    </Table>
  );
};

const Zone: FC<{ zone: ZoneMetadata }> = ({ zone }) => {
  return (
    <div className="flex flex-col gap-4 bg-card h-full p-4">
      <h1 className="text-xl">{zone.title}</h1>
      {zone.description && (
        <span className="font-thin">{zone.description}</span>
      )}
      <IrTable irs={zone.irs} />
    </div>
  );
};

export default Zone;
