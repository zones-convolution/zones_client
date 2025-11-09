import { FC } from "react";

import { Badge } from "@/components/ui/badge";
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";

import { ZoneMetadata } from "@/hooks/zone_metadata";
import {
  GenerationTypeOptions,
  SpaceCategoryOptions,
  TagOptions,
} from "@/lib/zone_options";

const ZoneTags: FC<{ zone: ZoneMetadata }> = ({ zone }) => {
  return (
    <Card>
      <CardHeader>
        <CardTitle>Tagging</CardTitle>
      </CardHeader>
      <CardContent className="space-y-8">
        <div className="grid items-center gap-4">
          <div className="grid flex-1 auto-rows-min gap-0.5">
            <div className="text-sm text-muted-foreground">Generation type</div>
            <div className="flex items-baseline gap-1 text-xl font-bold tabular-nums leading-none">
              {
                GenerationTypeOptions.find(
                  (gen) => gen.value == zone.generationType,
                )?.label
              }
            </div>
          </div>

          <div className="grid flex-1 auto-rows-min gap-0.5">
            <div className="text-sm text-muted-foreground">Space category</div>
            <div className="flex items-baseline gap-1 text-xl font-bold tabular-nums leading-none">
              {
                SpaceCategoryOptions.find(
                  (space) => space.value == zone.spaceCategory,
                )?.label
              }
            </div>
          </div>
          <div className="grid flex-1 auto-rows-min gap-0.5">
            <div className="text-sm text-muted-foreground">Tags</div>
            <div className="flex items-baseline gap-2 text-xl font-bold tabular-nums leading-none">
              {zone.tags?.map((tag, index) => {
                const tagLabel = TagOptions.find(
                  (opt) => opt.value == tag,
                )?.label;
                if (!tagLabel) return undefined;
                return <Badge key={index}>{tagLabel}</Badge>;
              })}
            </div>
          </div>
        </div>
      </CardContent>
    </Card>
  );
};

export { ZoneTags };
