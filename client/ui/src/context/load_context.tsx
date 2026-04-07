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
import { isAbortError } from "@/lib/abortable";

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
    const controller = new AbortController();
    const unsubscribe = loadingIrUpdateListener(setLoadingIr);

    getLoadingIr({ signal: controller.signal })
      .then(setLoadingIr)
      .catch((error) => {
        if (!isAbortError(error)) console.error(error);
      });

    return () => {
      controller.abort();
      unsubscribe();
    };
  }, []);

  useEffect(() => {
    const controller = new AbortController();
    const unsubscribe = currentIrUpdateListener(setCurrentIr);

    getCurrentIr({ signal: controller.signal })
      .then(setCurrentIr)
      .catch((error) => {
        if (!isAbortError(error)) console.error(error);
      });

    return () => {
      controller.abort();
      unsubscribe();
    };
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
