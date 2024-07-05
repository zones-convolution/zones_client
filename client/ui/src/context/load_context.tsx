import {
  createContext,
  FC,
  ReactNode,
  useContext,
  useEffect,
  useState,
} from "react";

import { useToast } from "@/components/ui/use-toast";

import { IrSelection, IrSelectionOptional } from "@/hooks/zone_metadata";
import {
  currentIrUpdateListener,
  getCurrentIr,
  getLoadingIr,
  loadingIrUpdateListener,
  loadIr,
} from "@/ipc/load_ipc";

interface ILoadContext {
  load: (irSelection: IrSelection) => Promise<void>;
  loadingIr: IrSelectionOptional;
  currentIr: IrSelectionOptional;
}

const LoadContext = createContext<ILoadContext | null>(null);

export const LoadProvider: FC<{
  children: ReactNode;
}> = ({ children }) => {
  const { toast } = useToast();

  const [loadingIr, setLoadingIr] = useState<IrSelectionOptional>({});

  const [currentIr, setCurrentIr] = useState<IrSelectionOptional>({});

  const load = async (irSelection: IrSelection) => {
    const result = await loadIr(irSelection);
    if (result)
      toast({
        description: `Ir loaded, ${irSelection.ir.title}.`,
      });
    else
      toast({
        variant: "destructive",
        description: `Failed to load ir, ${irSelection.ir.title}.`,
      });
  };

  useEffect(() => {
    getLoadingIr().then(setLoadingIr);
    return loadingIrUpdateListener(setLoadingIr);
  }, []);

  useEffect(() => {
    getCurrentIr().then(setCurrentIr);
    return currentIrUpdateListener(setCurrentIr);
  }, []);

  return (
    <LoadContext.Provider value={{ load, loadingIr, currentIr }}>
      {children}
    </LoadContext.Provider>
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
