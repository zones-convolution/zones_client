import { Home, Settings, Target } from "lucide-react";
import { FC, ReactNode } from "react";
import { NavLink, Outlet } from "react-router-dom";

import { buttonVariants } from "@/components/ui/button";

import ZonesLight from "@/assets/zones_light.svg";
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
    </div>
  );
};

const Root = () => {
  return (
    <div className="flex flex-row h-screen bg-background" data-theme="dark">
      <Sidebar />
      <div className="w-full ml-0.5">
        <Outlet />
      </div>
    </div>
  );
};

export default Root;
