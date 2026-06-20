import {
  createContext,
  Dispatch,
  FC,
  ReactNode,
  useCallback,
  useRef,
  SetStateAction,
  useContext,
  useEffect,
  useMemo,
  useState,
} from "react";

import {
  getVisualiserMetadata,
  defaultVisualiserMetadata,
  getVisualiserRender,
  visualiserRenderListener,
  VisualiserMetadata,
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
  visualiserMetadata: VisualiserMetadata;
  getLatestRender: () => Uint8Array | undefined;
  subscribeToRender: (listener: (render: Uint8Array) => void) => () => void;
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
  const [visualiserMetadata, setVisualiserMetadata] =
    useState<VisualiserMetadata>(defaultVisualiserMetadata);
  const renderRef = useRef<Uint8Array | undefined>(undefined);
  const renderListenersRef = useRef(new Set<(render: Uint8Array) => void>());

  const publishRender = useCallback((nextRender: Uint8Array) => {
    renderRef.current = nextRender;

    for (const listener of renderListenersRef.current) {
      listener(nextRender);
    }
  }, []);

  useEffect(() => {
    getVisualiserMetadata().then(setVisualiserMetadata);
    getVisualiserRender().then(publishRender);
    return visualiserRenderListener(publishRender, (nextMetadata) => {
      setVisualiserMetadata((currentMetadata) => {
        if (
          currentMetadata.sampleRate === nextMetadata.sampleRate &&
          currentMetadata.baseIrLengthSamples ===
            nextMetadata.baseIrLengthSamples
        ) {
          return currentMetadata;
        }

        return nextMetadata;
      });
    });
  }, [publishRender]);

  const [sensitivity, setSensitivity] = useState<number>(1.0);
  const [contrast, setContrast] = useState<number>(200.0);
  const [scale, setScale] = useState<VisualiserScale>("mel");
  const [colourMap, setColourMap] = useState<VisualiserColourMap>("viridis");

  const value = useMemo(
    () => ({
      visualiserMetadata,
      getLatestRender: () => renderRef.current,
      subscribeToRender: (listener: (render: Uint8Array) => void) => {
        renderListenersRef.current.add(listener);
        return () => {
          renderListenersRef.current.delete(listener);
        };
      },
      sensitivity,
      contrast,
      setSensitivity,
      setContrast,
      scale,
      setScale,
      colourMap,
      setColourMap,
    }),
    [visualiserMetadata, sensitivity, contrast, scale, colourMap],
  );

  return <VisualiserContext.Provider value={value}>{children}</VisualiserContext.Provider>;
};

export const useVisualiserContext = () => {
  const context = useContext(VisualiserContext);
  if (!context)
    throw new Error(
      "No VisualiserContext.Provider found when calling useVisualiserContext.",
    );

  return context;
};
