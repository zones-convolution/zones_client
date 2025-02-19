import { zodResolver } from "@hookform/resolvers/zod";
import { Download } from "lucide-react";
import { useForm } from "react-hook-form";

import { Button } from "@/components/ui/button";
import { Form } from "@/components/ui/form";
import { Separator } from "@/components/ui/separator";
import { useToast } from "@/components/ui/use-toast";

import { IrForm } from "@/components/create/ir_form";
import { ZoneMetadataForm } from "@/components/create/zone_metadata_form";
import { useBrowserContext } from "@/context/browser_context";
import { createZone, CreateZoneSchema } from "@/ipc/create_zone_ipc";

const CreateZone = () => {
  const form = useForm<CreateZoneSchema>({
    resolver: zodResolver(CreateZoneSchema),
    defaultValues: {
      metadata: {
        zoneType: "user",
        irs: [],
        images: [],
      },
    },
  });
  const { back } = useBrowserContext();
  const { toast } = useToast();

  return (
    <div className="h-full bg-card overflow-y-auto p-2">
      <div className="w-full">
        <Form {...form}>
          <form
            onSubmit={form.handleSubmit(async (data) => {
              const result = await createZone(data);
              if (result) {
                toast({
                  title: `${data.metadata.title} Created!`,
                });
                back(true);
              } else {
                toast({
                  title: "Error Creating Zone",
                  description: `Maybe the form is incomplete?`,
                });
              }
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
