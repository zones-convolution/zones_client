import { FC, useEffect, useState } from "react";

import { IrTable } from "@/components/zone_page/ir_table";
import { ZoneMetadata } from "@/hooks/zone_metadata";

const UserZone: FC<{ zone: ZoneMetadata }> = ({ zone }) => {
  return (
    <div className="flex flex-col gap-4 bg-card h-full p-4">
      <h1 className="text-xl">{zone.title}</h1>
      {zone.description && (
        <span className="font-thin">{zone.description}</span>
      )}
      <IrTable zone={zone} />
    </div>
  );
};

export default UserZone;
