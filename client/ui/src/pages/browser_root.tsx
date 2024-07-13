import { ChevronLeft, ChevronRight } from "lucide-react";
import { Link, Outlet, useLocation, useNavigate } from "react-router-dom";

import { Button } from "@/components/ui/button";

import { useValidTargetFormats } from "@/hooks/use_valid_target_formats";

const BrowserRoot = () => {
  const { validTargetFormats } = useValidTargetFormats();
  const location = useLocation();
  const navigate = useNavigate();

  const doesAnyHistoryEntryExist = location.key !== "default";

  console.log(window.history);

  return (
    <div className="flex flex-col gap-0.5 h-full">
      <div className="flex w-full bg-card p-2 items-center">
        <Button
          variant="ghost"
          onClick={() => navigate(-1)}
          disabled={!doesAnyHistoryEntryExist}
        >
          <ChevronLeft className="w-4 h-4" />
        </Button>
        <Button variant="ghost" className="mr-4" onClick={() => navigate(1)}>
          <ChevronRight className="w-4 h-4" />
        </Button>
        Browser
        <Button asChild className="ml-auto">
          <Link to="/create">Create Zone</Link>
        </Button>
      </div>
      <div className="flex flex-col">
        {validTargetFormats.map((format) => (
          <span>{format}</span>
        ))}
      </div>
      <Outlet />
    </div>
  );
};

export default BrowserRoot;
