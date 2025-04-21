import { FC } from "react";

import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";

import { UserProfile } from "@/components/user_profile";
import { IZone } from "@/lib/zones";

const ZoneDetails: FC<{ zone: IZone }> = ({ zone }) => {
  const { user } = zone;
  return (
    <Card>
      <CardHeader>
        <CardTitle>Details</CardTitle>
      </CardHeader>
      <CardContent className="space-y-8">
        <div className="grid items-center gap-4">
          <div className="grid flex-1 auto-rows-min gap-0.5">
            <div className="text-sm text-muted-foreground">Zone name</div>
            <div className="flex items-baseline gap-1 text-xl font-bold tabular-nums leading-none">
              {zone.title}
            </div>
          </div>
          <div className="grid flex-1 auto-rows-min gap-0.5">
            <div className="text-sm text-muted-foreground">
              Zone description
            </div>
            <div className="flex items-baseline gap-1 text-xl font-bold tabular-nums leading-none">
              {zone.description}
            </div>
          </div>
          {zone.attribution && (
            <div className="grid flex-1 auto-rows-min gap-0.5">
              <div className="text-sm text-muted-foreground">Attribution</div>
              <div className="flex items-baseline gap-1 text-xl font-bold tabular-nums leading-none">
                {zone.attribution}
              </div>
            </div>
          )}
          <div className="grid flex-1 auto-rows-min gap-0.5">
            <div className="text-sm text-muted-foreground">Capture Date</div>
            <div className="flex items-baseline gap-1 text-xl font-bold tabular-nums leading-none">
              {zone.captureDate &&
                new Date(zone.captureDate).toLocaleDateString()}
            </div>
          </div>
          <div className="grid flex-1 auto-rows-min gap-0.5">
            <div className="text-sm text-muted-foreground">Profile</div>
            <div className="flex items-baseline gap-1 text-xl font-bold tabular-nums leading-none">
              <UserProfile {...user} />
            </div>
          </div>
        </div>
      </CardContent>
    </Card>
  );
};

export { ZoneDetails };
