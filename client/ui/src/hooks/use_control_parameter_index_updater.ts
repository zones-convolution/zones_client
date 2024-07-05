import { useEffect } from "react";

import { juce } from "@/lib/juce";

const controlParameterIndexAnnotation = "controlparameterindex";

const useControlParameterIndexUpdater = () => {
  useEffect(() => {
    const controlParameterIndexUpdater = new juce.ControlParameterIndexUpdater(
      controlParameterIndexAnnotation,
    );

    const handleMouseMove = (event: MouseEvent) => {
      controlParameterIndexUpdater.handleMouseMove(event);
    };

    document.addEventListener("mousemove", handleMouseMove);

    return () => {
      document.removeEventListener("mousemove", handleMouseMove);
    };
  }, []);
};

export { useControlParameterIndexUpdater, controlParameterIndexAnnotation };
