import { createContext, FC, ReactNode, useContext, useState } from "react";

import { ZoneMetadata } from "@/hooks/zone_metadata";

export enum Route {
  Home,
  CreateZone,
  Zone,
  Search,
}

export type Routes =
  | {
      target: Route.Zone;
      state: ZoneMetadata;
    }
  | {
      target: Route.CreateZone;
    }
  | {
      target: Route.Home;
    }
  | { target: Route.Search };

interface INavigation {
  stack: Routes[];
  pointer: number;
}

interface IBrowserContext {
  canNavigateForward: boolean;
  canNavigateBack: boolean;
  navigation: INavigation;
  push: (route: Routes) => void;
  forward: () => void;
  back: (erase?: boolean) => void;
  route: Routes;
}

const BrowserContext = createContext<IBrowserContext | null>(null);

export const BrowserProvider: FC<{
  children: ReactNode;
}> = ({ children }) => {
  const [navigation, setNavigation] = useState<INavigation>({
    pointer: 0,
    stack: [{ target: Route.Home }],
  });

  const push = (route: Routes) => {
    setNavigation((current) => {
      const slicedStack = current.stack.slice(0, current.pointer + 1);
      const newStack = [...slicedStack, route];
      return {
        pointer: newStack.length - 1,
        stack: newStack,
      };
    });
  };

  const forward = () => {
    setNavigation((current) => {
      return {
        pointer: Math.min(current.pointer + 1, current.stack.length - 1),
        stack: current.stack,
      };
    });
  };

  const back = (erase: boolean = false) => {
    setNavigation((current) => {
      return {
        pointer: Math.max(current.pointer - 1, 0),
        stack: erase
          ? [...current.stack.slice(0, current.pointer)]
          : current.stack,
      };
    });
  };

  const canNavigateForward = navigation.pointer < navigation.stack.length - 1;
  const canNavigateBack = navigation.pointer > 0;
  const route = navigation.stack[navigation.pointer];

  return (
    <BrowserContext.Provider
      value={{
        navigation,
        push,
        forward,
        back,
        canNavigateForward,
        canNavigateBack,
        route,
      }}
    >
      {children}
    </BrowserContext.Provider>
  );
};

export const useBrowserContext = () => {
  const context = useContext(BrowserContext);
  if (!context)
    throw new Error(
      "No BrowserContext.Provider found when calling useBrowserContext.",
    );
  return context;
};

export const useNavigation = () => {
  const { push } = useBrowserContext();

  return {
    navigateToCreateZone: () => push({ target: Route.CreateZone }),
    navigateToZone: (zone: ZoneMetadata) =>
      push({ target: Route.Zone, state: zone }),
    navigateToSearch: () => push({ target: Route.Search }),
  };
};
