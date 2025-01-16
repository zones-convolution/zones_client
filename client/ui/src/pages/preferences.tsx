import { PlusCircle, Trash, FolderOpen } from "lucide-react";

import { Button } from "@/components/ui/button";
import { Separator } from "@/components/ui/separator";

import { usePreferences } from "@/hooks/use_preferences";

const Preferences = () => {
  const { preferences, versionData, addPath, revealPath, removePath } =
    usePreferences();

  return (
    <div className="flex flex-col bg-card p-4 gap-4 h-full">
      <div className="flex flex-row">
        <Button onClick={addPath}>
          Add User Path
          <PlusCircle className="w-4 h-4 ml-2" />
        </Button>
      </div>
      <Separator />
      {preferences.userPaths.map((path) => {
        return (
          <div
            className="flex flex-row justify-between items-center"
            key={path}
          >
            {path}
            <div className="flex flex-row gap-2">
              <Button
                onClick={async () => {
                  await revealPath(path);
                }}
              >
                Reveal
                <FolderOpen className="w-4 h-4 ml-2" />
              </Button>
              <Button
                onClick={async () => {
                  await removePath(path);
                }}
              >
                Remove
                <Trash className="w-4 h-4 ml-2" />
              </Button>
            </div>
          </div>
        );
      })}
      <Separator />
      <div className={"flex flex-row gap-2"}>
        <div>Version:</div>
        <div>
          {versionData.versionNumber}
          {versionData.buildType == "DEBUG" ? " | DEBUG" : ""}
        </div>
      </div>
    </div>
  );
};

export default Preferences;
