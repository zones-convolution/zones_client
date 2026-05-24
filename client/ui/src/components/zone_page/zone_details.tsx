import { FC } from "react";

import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";

import { UserProfile } from "@/components/user_profile";
import { ZoneMetadata } from "@/hooks/zone_metadata";
import { openExternalUrl } from "@/ipc/browser_ipc";
import { getFilterOptionByValue, ZoneLicenseOptions } from "@/lib/zone_options";
import { IUser } from "@/lib/zones";

const ZoneDetails: FC<{ zone: ZoneMetadata; user?: IUser }> = ({
  zone,
  user,
}) => {
  const license = zone.license
    ? getFilterOptionByValue(ZoneLicenseOptions, zone.license)
    : undefined;

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
          {zone.description && (
            <div className="grid flex-1 auto-rows-min gap-0.5">
              <div className="text-sm text-muted-foreground">
                Zone description
              </div>
              <div className="flex items-baseline gap-1 text-xl font-bold tabular-nums leading-none">
                {zone.description}
              </div>
            </div>
          )}
          {zone.attribution && (
            <div className="grid flex-1 auto-rows-min gap-0.5">
              <div className="text-sm text-muted-foreground">Attribution</div>
              <div className="flex items-baseline gap-1 text-xl font-bold tabular-nums leading-none">
                {zone.attribution}
              </div>
            </div>
          )}
          {license && (
            <div className="grid flex-1 auto-rows-min gap-0.5">
              <div className="text-sm text-muted-foreground">License</div>
              <div className="mt-1 rounded-xl border border-border/70 bg-muted/30 p-4">
                <div className="space-y-3">
                  <div className="space-y-1">
                    <button
                      type="button"
                      className="text-left text-base font-semibold leading-snug text-primary underline underline-offset-4"
                      onClick={() =>
                        license.href && openExternalUrl(license.href)
                      }
                    >
                      {license.label}
                    </button>
                  </div>
                  {license.description && (
                    <p className="text-sm text-muted-foreground">
                      {license.description}
                    </p>
                  )}
                </div>
              </div>
            </div>
          )}
          {zone.captureDate && (
            <div className="grid flex-1 auto-rows-min gap-0.5">
              <div className="text-sm text-muted-foreground">Capture Date</div>
              <div className="flex items-baseline gap-1 text-xl font-bold tabular-nums leading-none">
                {zone.captureDate &&
                  new Date(zone.captureDate).toLocaleDateString()}
              </div>
            </div>
          )}
          {user && (
            <div className="grid flex-1 auto-rows-min gap-0.5">
              <div className="text-sm text-muted-foreground">Profile</div>
              <div className="flex items-baseline gap-1 text-xl font-bold tabular-nums leading-none">
                <UserProfile {...user} />
              </div>
            </div>
          )}
        </div>
      </CardContent>
    </Card>
  );
};

export { ZoneDetails };
