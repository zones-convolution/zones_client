import { zodResolver } from "@hookform/resolvers/zod";
import { Download } from "lucide-react";
import { useForm } from "react-hook-form";
import { useNavigate } from "react-router-dom";
import { z } from "zod";

import { Button } from "@/components/ui/button";
import { Form } from "@/components/ui/form";
import { Separator } from "@/components/ui/separator";

import { IrForm } from "@/components/create/IrForm";
import { ZoneMetadataForm } from "@/components/create/ZoneMetadataForm";
import { ZoneMetadata } from "@/hooks/zone_metadata";
import { juce } from "@/lib/juce";

const CreateZoneSchema = z.object({
  path: z.string(),
  metadata: ZoneMetadata,
});

export type CreateZoneSchema = z.infer<typeof CreateZoneSchema>;

const createZoneNative = juce.getNativeFunction("import_zone_native");

const CreateZone = () => {
  const form = useForm<CreateZoneSchema>({
    resolver: zodResolver(CreateZoneSchema),
    defaultValues: {
      metadata: {
        irs: [],
        images: [],
      },
    },
  });

  const navigate = useNavigate();

  return (
    <div className="bg-card h-full relative">
      <div className="max-h-full w-full p-2 overflow-y-auto">
        <Form {...form}>
          <form
            onSubmit={form.handleSubmit(async (data) => {
              await createZoneNative(JSON.stringify(data));
              navigate("/browser");
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
    </div>
  );
};

export default CreateZone;