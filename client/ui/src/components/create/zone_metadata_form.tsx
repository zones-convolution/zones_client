import { Settings } from "lucide-react";
import { useEffect } from "react";
import { Controller, useFormContext } from "react-hook-form";

import { Button } from "@/components/ui/button";
import {
  Field,
  FieldError,
  FieldLabel,
} from "@/components/ui/field";
import { Input } from "@/components/ui/input";
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from "@/components/ui/select";
import { Textarea } from "@/components/ui/textarea";

import { Tabs, useTabsContext } from "@/context/tabs_context";
import { usePreferences } from "@/hooks/use_preferences";
import { CreateZoneSchema } from "@/ipc/create_zone_ipc";

const ZoneMetadataForm = () => {
  const { control, setValue } = useFormContext<CreateZoneSchema>();
  const { preferences } = usePreferences();
  useEffect(() => {
    if (preferences.userPaths[0]) setValue("path", preferences.userPaths[0]);
  }, [preferences]);

  const noUserPaths = preferences.userPaths.length == 0;
  const { setTab } = useTabsContext();
  return (
    <>
      <Controller
        name="metadata.title"
        control={control}
        render={({ field }) => (
          <Field>
            <FieldLabel htmlFor={field.name}>Title</FieldLabel>
            <Input id={field.name} placeholder="Title" {...field} />
          </Field>
        )}
      />
      <Controller
        name="metadata.description"
        control={control}
        render={({ field }) => (
          <Field>
            <FieldLabel htmlFor={field.name}>Description</FieldLabel>
            <Textarea id={field.name} placeholder="Description" {...field} />
          </Field>
        )}
      />
      <Controller
        name="path"
        control={control}
        render={({ field, fieldState }) => (
          <Field data-invalid={fieldState.invalid}>
            <FieldLabel htmlFor={field.name}>User Path</FieldLabel>
              <div className="flex flex-row gap-2 items-center justify-between">
                {noUserPaths ? (
                  <span className="italic font-thin">
                    No user paths found, add a path in preferences.
                  </span>
                ) : (
                  <Select onValueChange={field.onChange} value={field.value}>
                    <SelectTrigger id={field.name} aria-invalid={fieldState.invalid}>
                      <SelectValue />
                    </SelectTrigger>
                    <SelectContent>
                      {preferences.userPaths.map((path) => (
                        <SelectItem value={path} key={path}>
                          {path}
                        </SelectItem>
                      ))}
                    </SelectContent>
                  </Select>
                )}

                <Button
                  type="button"
                  onClick={() => {
                    setTab(Tabs.Preferences);
                  }}
                >
                  Preferences
                  <Settings className="w-4 h-4 ml-2" />
                </Button>
              </div>
            <FieldError errors={[fieldState.error]} />
          </Field>
        )}
      />
    </>
  );
};

export { ZoneMetadataForm };
