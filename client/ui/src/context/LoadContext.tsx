import { createContext, FC, ReactNode, useContext } from "react";

import { IrSelection } from "@/hooks/zone_metadata";
import { juce } from "@/lib/juce";

interface ILoadContext {
  load: (irSelection: IrSelection) => Promise<void>;
  irSelection?: IrSelection;
}

const LoadContext = createContext<ILoadContext | null>(null);

const loadZoneNative = juce.getNativeFunction("load_zone_native");

export const LoadProvider: FC<{
  children: ReactNode;
}> = ({ children }) => {
  const load = async (irSelection: IrSelection) => {
    await loadZoneNative(JSON.stringify(irSelection));
  };

  return (
    <LoadContext.Provider value={{ load }}>{children}</LoadContext.Provider>
  );
};

export const useLoadContext = () => {
  const loadContext = useContext(LoadContext);
  if (!loadContext)
    throw new Error(
      "No LoadContext.Provider found when calling useLoadContext.",
    );

  return loadContext;
};
