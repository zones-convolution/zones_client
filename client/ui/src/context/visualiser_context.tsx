import {
  createContext,
  Dispatch,
  FC,
  ReactNode,
  SetStateAction,
  useContext,
  useEffect,
  useState,
} from "react";

import {
  getVisualiserRender,
  visualiserRenderListener,
} from "@/ipc/visualiser_ipc";

export const visualiserScales = ["linear", "mel"];
export type VisualiserScale = (typeof visualiserScales)[number];

export const visualiserColourMaps = [
  "jet",
  "hsv",
  "hot",
  "spring",
  "summer",
  "autumn",
  "winter",
  "bone",
  "copper",
  "greys",
  "yignbu",
  "greens",
  "yiorrd",
  "bluered",
  "rdbu",
  "picnic",
  "rainbow",
  "portland",
  "blackbody",
  "earth",
  "electric",
  "alpha",
  "viridis",
  "inferno",
  "magma",
  "plasma",
  "warm",
  "cool",
  "rainbow-soft",
  "bathymetry",
  "cdom",
  "chlorophyll",
  "density",
  "freesurface-blue",
  "freesurface-red",
  "oxygen",
  "par",
  "phase",
  "salinity",
  "temperature",
  "turbidity",
  "velocity-blue",
  "velocity-green",
  "cubehelix",
];
export type VisualiserColourMap = (typeof visualiserColourMaps)[number];

export interface IVisualiserContext {
  render?: Uint8Array;
  sensitivity: number;
  setSensitivity: Dispatch<SetStateAction<number>>;
  contrast: number;
  setContrast: Dispatch<SetStateAction<number>>;
  scale: VisualiserScale;
  setScale: Dispatch<SetStateAction<VisualiserScale>>;
  colourMap: VisualiserColourMap;
  setColourMap: Dispatch<SetStateAction<VisualiserColourMap>>;
}

const VisualiserContext = createContext<IVisualiserContext | null>(null);

export const VisualiserProvider: FC<{
  children: ReactNode;
}> = ({ children }) => {
  const [render, setRender] = useState<Uint8Array | undefined>();

  useEffect(() => {
    getVisualiserRender().then(setRender);
    return visualiserRenderListener(setRender);
  }, []);

  const [sensitivity, setSensitivity] = useState<number>(1.0);
  const [contrast, setContrast] = useState<number>(1.0);
  const [scale, setScale] = useState<VisualiserScale>("mel");
  const [colourMap, setColourMap] = useState<VisualiserColourMap>("viridis");

  return (
    <VisualiserContext.Provider
      value={{
        render: render,
        sensitivity: sensitivity,
        contrast: contrast,
        setSensitivity: setSensitivity,
        setContrast: setContrast,
        scale: scale,
        setScale: setScale,
        colourMap: colourMap,
        setColourMap: setColourMap,
      }}
    >
      {children}
    </VisualiserContext.Provider>
  );
};

export const useVisualiserContext = () => {
  const context = useContext(VisualiserContext);
  if (!context)
    throw new Error(
      "No VisualiserContext.Provider found when calling useVisualiserContext.",
    );

  return context;
};
