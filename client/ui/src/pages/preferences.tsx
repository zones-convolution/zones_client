import { PlusCircle, Trash, FolderOpen, MoreHorizontal } from "lucide-react";
import { number } from "zod";

import { Button } from "@/components/ui/button";
import {
  DropdownMenu,
  DropdownMenuCheckboxItem,
  DropdownMenuContent,
  DropdownMenuLabel,
  DropdownMenuSeparator,
  DropdownMenuTrigger,
} from "@/components/ui/dropdown-menu";
import { Separator } from "@/components/ui/separator";

import { usePreferences } from "@/hooks/use_preferences";

function getValidBlockSizes(maximum: number): number[] {
  console.log(maximum);
  const result: number[] = [];
  let currentpo2 = 32;

  while (currentpo2 <= maximum) {
    result.push(currentpo2);
    currentpo2 *= 2;
  }
  console.log(result);
  return result;
}

const Preferences = () => {
  const {
    preferences,
    versionData,
    blockSizes,
    setBlockSize,
    addPath,
    revealPath,
    removePath,
  } = usePreferences();

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
      <div>{blockSizes.current}</div>
      <div>{blockSizes.maximum}</div>
      <DropdownMenu>
        <DropdownMenuTrigger asChild>
          {
            <Button aria-haspopup="true" size="icon" variant="ghost">
              <MoreHorizontal className="h-4 w-4" />
              <span className="sr-only">Toggle menu</span>
            </Button>
          }
        </DropdownMenuTrigger>
        <DropdownMenuContent>
          <DropdownMenuLabel>Internal Block Size</DropdownMenuLabel>
          <DropdownMenuSeparator />
          {getValidBlockSizes(blockSizes.maximum).map((validBlockSize) => {
            return (
              <DropdownMenuCheckboxItem
                className="uppercase"
                checked={validBlockSize == blockSizes.current}
                onCheckedChange={(checked) => {
                  if (checked) setBlockSize(validBlockSize);
                }}
              >
                {validBlockSize}
              </DropdownMenuCheckboxItem>
            );
          })}
        </DropdownMenuContent>
      </DropdownMenu>

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
