import {
  createContext,
  FC,
  ReactNode,
  useContext,
  useEffect,
  useState,
} from "react";
import { z } from "zod";

import { IrSelection, IrSelectionOptional } from "@/hooks/zone_metadata";
import {
  addNativeEventListener,
  juce,
  removeNativeEventListener,
} from "@/lib/juce";

interface ILoadContext {
  load: (irSelection: IrSelection) => Promise<void>;
  loadingIr?: IrSelection;
  currentIr?: IrSelection;
}

const LoadContext = createContext<ILoadContext | null>(null);

const loadIrNative = juce.getNativeFunction("load_ir_native");
const getLoadingIrNative = juce.getNativeFunction("get_loading_ir_native");
const getCurrentIrNative = juce.getNativeFunction("get_current_ir_native");
const onLoadingIrUpdatedNative = "on_loading_ir_updated_native";
const onCurrentIrUpdatedNative = "on_current_ir_updated_native";

export const LoadProvider: FC<{
  children: ReactNode;
}> = ({ children }) => {
  const [loadingIr, setLoadingIr] = useState<IrSelection | undefined>(
    undefined,
  );

  const [currentIr, setCurrentIr] = useState<IrSelection | undefined>(
    undefined,
  );

  const load = async (irSelection: IrSelection) => {
    try {
      const result = z
        .boolean()
        .parse(await loadIrNative(JSON.stringify(irSelection)));

      if (result) console.log("LOADED!!!");
      else console.log("FAILED TO LOAD");
    } catch (err) {
      console.error("Failed to parse success response", err);
    }
  };

  const getLoadingIr = async () => {
    return await getLoadingIrNative();
  };

  const getCurrentIr = async () => {
    return await getCurrentIrNative();
  };

  const handleReceiveLoadingIr = (data: any) => {
    try {
      setLoadingIr(IrSelectionOptional.parse(JSON.parse(data)).irSelection);
    } catch (err) {
      console.error("Failed to parse LoadingIr!", err);
    }
  };

  const handleReceiveCurrentIr = (data: any) => {
    try {
      setCurrentIr(IrSelectionOptional.parse(JSON.parse(data)).irSelection);
    } catch (err) {
      console.error("Failed to parse CurrentIr!", err);
    }
  };

  useEffect(() => {
    getLoadingIr().then(handleReceiveLoadingIr);

    let registrationId = addNativeEventListener(
      onLoadingIrUpdatedNative,
      handleReceiveLoadingIr,
    );

    return () => {
      removeNativeEventListener(registrationId);
    };
  }, []);

  useEffect(() => {
    getCurrentIr().then(handleReceiveCurrentIr);

    let registrationId = addNativeEventListener(
      onCurrentIrUpdatedNative,
      handleReceiveCurrentIr,
    );

    return () => {
      removeNativeEventListener(registrationId);
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
