import { useGesture } from "@use-gesture/react";
import { Grip } from "lucide-react";

import { beginResize, endResize, resize } from "@/ipc/resize_ipc";

export const ResizeHandle = () => {
  const bind = useGesture(
    {
      onDrag: async ({ xy: [x, y] }) => {
        await resize(x, y);
      },
      onDragStart: async ({ xy: [x, y] }) => {
        await beginResize(x, y);
      },
      onDragEnd: async () => {
        await endResize();
      },
    },
    {},
  );

  return (
    <div
      {...bind()}
      className="absolute bottom-0 right-0 m-4 cursor-all-scroll"
    >
      <Grip className="w-4 h-4 text-primary/80 hover:text-primary" />
    </div>
  );
};
