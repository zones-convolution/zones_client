import { FC } from "react";
import { useErrorBoundary } from "react-error-boundary";

import { ZoneMetadata } from "@/hooks/zone_metadata";

const Zone: FC<{ zone: ZoneMetadata }> = ({ zone }) => {
  return (
    <div className="bg-card">
      <h1 className="text-xl">{zone.title}</h1>
    </div>
  );
};

export default Zone;
