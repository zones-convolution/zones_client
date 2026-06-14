import { PlusCircle, Trash, FolderOpen } from "lucide-react";

import { Button } from "@/components/ui/button";
import {
  Card,
  CardContent,
  CardHeader,
  CardTitle,
} from "@/components/ui/card";
import {
  DropdownMenu,
  DropdownMenuCheckboxItem,
  DropdownMenuContent,
  DropdownMenuLabel,
  DropdownMenuSeparator,
  DropdownMenuTrigger,
} from "@/components/ui/dropdown-menu";

import { usePreferences } from "@/hooks/use_preferences";

function getValidBlockSizes(maximum: number): number[] {
  const result: number[] = [];
  let currentpo2 = 32;

  while (currentpo2 <= maximum) {
    result.push(currentpo2);
    currentpo2 *= 2;
  }

  if (!((maximum & (maximum - 1)) === 0)) result.push(maximum);

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
    revealInternalPath,
    removePath,
  } = usePreferences();

  return (
    <div className="flex h-full min-h-0 flex-col gap-page-gutter p-page-gutter">
      <Card className="min-h-0 flex-1 overflow-hidden rounded-md">
        <CardHeader>
          <CardTitle className="text-xl">Preferences</CardTitle>
        </CardHeader>
        <CardContent className="h-full overflow-y-auto space-y-6">
          <div className="flex flex-col gap-3 sm:flex-row sm:items-start sm:justify-between">
            <div className="space-y-1">
              <div className="text-sm font-medium">User zones</div>
              <div className="text-sm text-muted-foreground">
                Add local folders that should appear as zones.
              </div>
              {preferences.userPaths.length === 0 ? (
                <div className="text-sm text-muted-foreground">
                  No folders added.
                </div>
              ) : (
                <div className="space-y-2 pt-1">
                  {preferences.userPaths.map((path) => {
                    return (
                      <div
                        className="flex flex-col gap-2 rounded-md border p-3 sm:flex-row sm:items-center sm:justify-between"
                        key={path}
                      >
                        <div className="min-w-0 break-all text-sm text-muted-foreground">
                          {path}
                        </div>
                        <div className="flex gap-2">
                          <Button
                            onClick={async () => {
                              await revealPath(path);
                            }}
                            size="sm"
                            variant="outline"
                          >
                            Reveal
                            <FolderOpen className="ml-2 h-4 w-4" />
                          </Button>
                          <Button
                            onClick={async () => {
                              await removePath(path);
                            }}
                            size="sm"
                            variant="outline"
                          >
                            Remove
                            <Trash className="ml-2 h-4 w-4" />
                          </Button>
                        </div>
                      </div>
                    );
                  })}
                </div>
              )}
            </div>
            <Button onClick={addPath}>
              Add Folder
              <PlusCircle className="ml-2 h-4 w-4" />
            </Button>
          </div>

          <div className="flex flex-col gap-3 border-t pt-6 sm:flex-row sm:items-center sm:justify-between">
            <div className="space-y-1">
              <div className="text-sm font-medium">Downloaded web zones</div>
              <div className="text-sm text-muted-foreground">
                Open the internal download folder.
              </div>
            </div>
            <Button onClick={revealInternalPath} size="sm" variant="outline">
              Reveal Folder
              <FolderOpen className="ml-2 h-4 w-4" />
            </Button>
          </div>

          <div className="flex flex-col gap-3 border-t pt-6 sm:flex-row sm:items-center sm:justify-between">
            <div className="space-y-1">
              <div className="text-sm font-medium">Internal block size</div>
              <div className="text-sm text-muted-foreground">
                Higher values can reduce CPU spikes, but may add latency.
              </div>
            </div>
            <DropdownMenu>
              <DropdownMenuTrigger asChild>
                <Button aria-haspopup="true" size="sm" variant="outline">
                  {blockSizes.current}
                </Button>
              </DropdownMenuTrigger>
              <DropdownMenuContent>
                <DropdownMenuLabel>Internal Block Size</DropdownMenuLabel>
                <DropdownMenuSeparator />
                {getValidBlockSizes(blockSizes.maximum).map((validBlockSize) => {
                  return (
                    <DropdownMenuCheckboxItem
                      className="uppercase"
                      checked={validBlockSize == blockSizes.current}
                      key={validBlockSize}
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
          </div>

          <div className="flex flex-col gap-3 border-t pt-4 text-sm sm:flex-row sm:items-center sm:justify-between">
            <div className="font-medium text-foreground">Plugin version</div>
            <div className="flex flex-row items-center gap-1 text-muted-foreground">
              <div>{versionData.buildType == "DEBUG" && "DEBUG"}</div>
              <div>{versionData.versionNumber}</div>
              <div>({versionData.gitCommitHash})</div>
            </div>
          </div>
        </CardContent>
      </Card>
    </div>
  );
};

export default Preferences;
