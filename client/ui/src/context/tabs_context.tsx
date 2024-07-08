import { createContext, FC, ReactNode, useContext, useState } from "react";

export enum Tabs {
  Browser,
  Editor,
  Preferences,
}

interface ITabsContext {
  setTab: (tab: Tabs) => void;
  tab: Tabs;
}

const TabsContext = createContext<ITabsContext | null>(null);

export const TabsProvider: FC<{
  children: ReactNode;
}> = ({ children }) => {
  const [tab, setTab] = useState<Tabs>(Tabs.Editor);

  return (
    <TabsContext.Provider value={{ tab, setTab }}>
      {children}
    </TabsContext.Provider>
  );
};

export const useTabsContext = () => {
  const tabsContext = useContext(TabsContext);
  if (!tabsContext)
    throw new Error(
      "No TabsContext.Provider found when calling useTabsContext.",
    );

  return tabsContext;
};
