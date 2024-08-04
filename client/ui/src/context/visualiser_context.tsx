import {
  createContext,
  FC,
  ReactNode,
  useContext,
  useEffect,
  useState,
} from "react";

import {
  getVisualiserRender,
  visualiserRenderListener,
} from "@/ipc/visualiser_ipc";

const VisualiserContext = createContext<Uint8Array>(new Uint8Array());

export const VisualiserProvider: FC<{
  children: ReactNode;
}> = ({ children }) => {
  const [render, setRender] = useState<Uint8Array>(new Uint8Array());

  useEffect(() => {
    getVisualiserRender().then(setRender);
    return visualiserRenderListener(setRender);
  }, []);

  return (
    <VisualiserContext.Provider value={render}>
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
