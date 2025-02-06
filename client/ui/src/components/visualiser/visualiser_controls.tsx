import colormap from "colormap";

import { Label } from "@/components/ui/label";
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from "@/components/ui/select";
import { Slider } from "@/components/ui/slider";

import {
  useVisualiserContext,
  VisualiserColourMap,
  visualiserColourMaps,
  VisualiserScale,
  visualiserScales,
} from "@/context/visualiser_context";

const createCssGradient = (colors: string[]) => {
  if (!Array.isArray(colors) || colors.length === 0) {
    throw new Error("Input must be a non-empty array of colors.");
  }

  return `linear-gradient(90deg, ${colors.join(", ")})`;
};

const VisualiserControls = () => {
  const context = useVisualiserContext();
  return (
    <div className="flex flex-1 flex-col gap-2 justify-items-end">
      <div className="flex-1 items-center">
        <Label htmlFor="sensitivity" className="w-28">
          Sensitivity
        </Label>
        <Slider
          onValueChange={(v) => context.setSensitivity(v[0])}
          value={[context.sensitivity]}
          min={0}
          max={5}
          step={0.01}
          id="sensitivity"
        />
      </div>

      <div className="flex-1 items-center">
        <Label htmlFor="contrast" className="w-28">
          Contrast
        </Label>
        <Slider
          onValueChange={(v) => context.setContrast(v[0])}
          value={[context.contrast]}
          min={0}
          max={500}
          step={0.01}
          id="contrast"
        />
      </div>
      <div className={"flex-1"}>
        <Select
          value={context.scale}
          onValueChange={(v) => context.setScale(v as VisualiserScale)}
        >
          <SelectTrigger className="capitalize">
            <SelectValue />
          </SelectTrigger>
          <SelectContent>
            {visualiserScales.map((scale) => (
              <SelectItem key={scale} value={scale} className="capitalize">
                {scale}
              </SelectItem>
            ))}
          </SelectContent>
        </Select>
      </div>
      <div className={"flex-1"}>
        <Select
          value={context.colourMap}
          onValueChange={(v) => context.setColourMap(v as VisualiserColourMap)}
        >
          <SelectTrigger className="capitalize">
            <SelectValue />
          </SelectTrigger>
          <SelectContent>
            {visualiserColourMaps.map((colourMap) => {
              const colours = colormap({
                colormap: colourMap,
                nshades: 20,
                format: "hex",
                alpha: 1,
              });

              return (
                <SelectItem key={colourMap} value={colourMap}>
                  <div className="flex items-center gap-2 capitalize">
                    <div
                      className="w-8 h-4"
                      style={{
                        background: createCssGradient(colours),
                      }}
                    />
                    {colourMap}
                  </div>
                </SelectItem>
              );
            })}
          </SelectContent>
        </Select>
      </div>
    </div>
  );
};

export { VisualiserControls };
