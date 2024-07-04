import { Home, Loader, Settings, Target } from "lucide-react";
import { FC, ReactNode } from "react";
import { NavLink, Outlet } from "react-router-dom";

import { buttonVariants } from "@/components/ui/button";
import { Separator } from "@/components/ui/separator";
import { Toaster } from "@/components/ui/toaster";

import ZonesLight from "@/assets/zones_light.svg";
import { DiscreteLevelLayout } from "@/components/discrete_level_layout";
import { Meter } from "@/components/meter";
import { ResizeHandle } from "@/components/resize_handle";
import { LoadProvider, useLoadContext } from "@/context/load_context";
import { useEngineLoading } from "@/hooks/use_engine";
import { cn } from "@/lib/utils";

const NavButton: FC<{
  children: ReactNode;
  to: string;
}> = ({ children, to }) => {
  return (
    <NavLink
      to={to}
      className={(nav) =>
        cn(
          buttonVariants({
            variant: "ghost",
            className: `justify-start w-full ${nav.isActive && "bg-accent"}`,
          }),
        )
      }
    >
      {children}
    </NavLink>
  );
};

const Sidebar = () => {
  const { irEngineLoading, convolutionEngineLoading } = useEngineLoading();
  const { currentIr } = useLoadContext();

  return (
    <div className="bg-card flex flex-col gap-0.5">
      <img src={ZonesLight} className="w-[80px] ml-4" alt="Zones Logo Dark" />
      <NavButton to="/browser">
        <Home className="w-4 h-4 mr-4" />
        Browse
      </NavButton>
      <NavButton to="/">
        <Target className="w-4 h-4 mr-4" />
        Edit
      </NavButton>
      <NavButton to="/preferences">
        <Settings className="w-4 h-4 mr-4" />
        Preferences
      </NavButton>

      <div className="px-2 flex flex-col gap-4 mt-auto overflow-hidden">
        {irEngineLoading && (
          <div className="flex items-center">
            <Loader className="w-4 h-4 animate-spin mr-2 shrink-0" /> Impulse
          </div>
        )}

        {convolutionEngineLoading && (
          <div className="flex items-center">
            <Loader className="w-4 h-4 animate-spin mr-2 shrink-0" />
            <p className="line-clamp-1">Convolver</p>
          </div>
        )}

        {currentIr.irSelection && (
          <div className="flex flex-col gap-2">
            <Separator />
            <span className="text-base">{currentIr.irSelection.ir.title}</span>
            <span className="text-sm font-thin">
              {currentIr.irSelection.zone.title}
            </span>
          </div>
        )}
      </div>
      <div className="h-60 w-full p-2">
        <Meter />
        {/*<DiscreteLevelLayout levels={[0, -3, -6, -18, -30, -60]} />*/}
      </div>
    </div>
  );
};

const Root = () => {
  return (
    <div
      className="flex flex-row h-screen bg-background relative"
      data-theme="dark"
    >
      <LoadProvider>
        <Sidebar />
        <div className="w-full ml-0.5">
          <Outlet />
        </div>
      </LoadProvider>
      <ResizeHandle />
      <Toaster />
    </div>
  );
};

export default Root;
