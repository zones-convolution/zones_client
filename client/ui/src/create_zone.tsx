import { zodResolver } from "@hookform/resolvers/zod";
import { Download, MinusCircle, PlusCircle, Settings } from "lucide-react";
import { useEffect } from "react";
import { useFieldArray, useForm, useFormContext } from "react-hook-form";
import { Link } from "react-router-dom";
import { z } from "zod";

import { Button } from "@/components/ui/button";
import {
  Form,
  FormControl,
  FormField,
  FormItem,
  FormLabel,
  FormMessage,
} from "@/components/ui/form";
import { Input } from "@/components/ui/input";
import {
  SelectContent,
  SelectItem,
  Select,
  SelectTrigger,
  SelectValue,
} from "@/components/ui/select";
import { Separator } from "@/components/ui/separator";
import { Textarea } from "@/components/ui/textarea";

import { usePreferences } from "@/hooks/use_preferences";
import { ZoneMetadata } from "@/hooks/zone_metadata";

const CreateZoneSchema = z.object({
  userPath: z.string(),
  metadata: ZoneMetadata,
});

type CreateZoneSchema = z.infer<typeof CreateZoneSchema>;

const ZoneMetadataForm = () => {
  const { control, setValue } = useFormContext<CreateZoneSchema>();
  const { preferences } = usePreferences();
  useEffect(() => {
    if (preferences.userPaths[0])
      setValue("userPath", preferences.userPaths[0]);
  }, [preferences]);

  const noUserPaths = preferences.userPaths.length == 0;

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
        name="userPath"
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

                <Button asChild>
                  <Link to="/preferences">
                    Preferences
                    <Settings className="w-4 h-4 ml-2" />
                  </Link>
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

const IrForm = () => {
  const { control } = useFormContext<CreateZoneSchema>();
  const { fields, append, remove } = useFieldArray({
    control,
    name: "metadata.irs",
  });

  return (
    <div className="flex flex-col gap-2">
      {fields.map((field, index) => (
        <div className="flex flex-col gap-2">
          <FormField
            control={control}
            name={`metadata.irs.${index}.title`}
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
            name={`metadata.irs.${index}.description`}
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
            name={`metadata.irs.${index}.channelFormat`}
            render={({ field }) => (
              <FormItem>
                <FormLabel>Channel Format</FormLabel>
                <FormControl>
                  <Select onValueChange={field.onChange} value={field.value}>
                    <FormControl>
                      <SelectTrigger>
                        <SelectValue defaultValue="stereo" />
                      </SelectTrigger>
                    </FormControl>
                    <SelectContent>
                      <SelectItem value="foa">FOA</SelectItem>
                      <SelectItem value="mono">Mono</SelectItem>
                      <SelectItem value="stereo">Stereo</SelectItem>
                      <SelectItem value="quadraphonic">Quadraphonic</SelectItem>
                    </SelectContent>
                  </Select>
                </FormControl>
                <FormMessage />
              </FormItem>
            )}
          />
          <Button
            className="w-full"
            variant="destructive"
            onClick={() => remove(index)}
          >
            Remove Ir <MinusCircle className="w-4 h-4 ml-2" />
          </Button>
        </div>
      ))}
      <Button
        className="w-full"
        variant="secondary"
        onClick={() => append({ relativePath: "" })}
      >
        Add Ir <PlusCircle className="w-4 h-4 ml-2" />
      </Button>
    </div>
  );
};
const CreateZone = () => {
  const form = useForm<CreateZoneSchema>({
    resolver: zodResolver(CreateZoneSchema),
    defaultValues: {},
  });

  return (
    <div className="p-2 bg-card">
      <Form {...form}>
        <form
          onSubmit={form.handleSubmit((data) => {
            console.log(data);
          })}
          className="space-y-8"
        >
          <ZoneMetadataForm />
          <Separator />
          <IrForm />
          <Separator />
          <Button type="submit" className="w-full" variant="secondary">
            Create Zone <Download className="w-4 h-4 ml-2" />
          </Button>
        </form>
      </Form>
    </div>
  );
};

export default CreateZone;
