import { PlusCircle, Trash, FolderOpen } from "lucide-react";

import { Button } from "@/components/ui/button";

import { usePreferences } from "@/hooks/use_preferences";

const Preferences = () => {
  const { preferences, addPath, revealPath, removePath } = usePreferences();

  return (
    <div className="flex flex-col bg-card m-0.5 p-4 gap-2">
      <div className="flex flex-row">
        <Button onClick={addPath}>
          Add User Path
          <PlusCircle className="w-4 h-4 ml-2" />
        </Button>
      </div>
      {preferences.userPaths.map((path) => {
        return (
          <div className="flex flex-row justify-between">
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
    </div>
  );
};

export default Preferences;
