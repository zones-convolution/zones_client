import { Settings } from "lucide-react";
import { useEffect } from "react";
import { useFormContext } from "react-hook-form";

import { Button } from "@/components/ui/button";
import {
  FormControl,
  FormField,
  FormItem,
  FormLabel,
  FormMessage,
} from "@/components/ui/form";
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
      <FormField
        control={control}
        name="metadata.title"
        render={({ field }) => (
          <FormItem>
            <FormLabel>Title</FormLabel>
            <FormControl>
              <Input placeholder="Title" {...field} />
            </FormControl>
            <FormMessage />
          </FormItem>
        )}
      />
      <FormField
        control={control}
        name="metadata.description"
        render={({ field }) => (
          <FormItem>
            <FormLabel>Description</FormLabel>
            <FormControl>
              <Textarea placeholder="Description" {...field} />
            </FormControl>
            <FormMessage />
          </FormItem>
        )}
      />
      <FormField
        control={control}
        name="path"
        render={({ field }) => (
          <FormItem>
            <FormLabel>User Path</FormLabel>
            <FormControl>
              <div className="flex flex-row gap-2 items-center justify-between">
                {noUserPaths ? (
                  <span className="italic font-thin">
                    No user paths found, add a path in preferences.
                  </span>
                ) : (
                  <Select onValueChange={field.onChange} value={field.value}>
                    <FormControl>
                      <SelectTrigger>
                        <SelectValue />
                      </SelectTrigger>
                    </FormControl>
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
            </FormControl>
            <FormMessage />
          </FormItem>
        )}
      />
    </>
  );
};

export { ZoneMetadataForm };
