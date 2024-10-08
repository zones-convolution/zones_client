import { Canvas, useThree } from "@react-three/fiber";
import { FC, useEffect, useMemo, useRef } from "react";
import { FrontSide, GLSL3, ShaderMaterial } from "three";

import {
  createColourMapTexture,
  createScaleTexture,
  defaultHeight,
  generateColourMap,
  generateRenderTexture,
} from "@/components/visualiser/visualiser_utils";
import { IVisualiserContext } from "@/context/visualiser_context";

import frag from "./visualiser.frag";
import vert from "./visualiser.vert";

const Graph2D: FC<{ context: IVisualiserContext }> = ({ context }) => {
  const viewport = useThree((state) => state.viewport);

  const matRef = useRef<ShaderMaterial>(null);

  useEffect(() => {
    const mat = matRef.current;
    if (mat && context.render) {
      mat.uniforms.render.value = generateRenderTexture(context.render);
    }
  }, [context.render]);

  useEffect(() => {
    const mat = matRef.current;
    if (mat) {
      mat.uniforms.scale.value = createScaleTexture(
        44100,
        defaultHeight * 2,
        context.scale,
      );
    }
  }, [context.scale]);

  useEffect(() => {
    const mat = matRef.current;
    if (mat) {
      mat.uniforms.colourMap.value = createColourMapTexture(
        generateColourMap(context.colourMap),
      );
    }
  }, [context.colourMap]);

  useEffect(() => {
    const mat = matRef.current;
    if (mat) {
      mat.uniforms.sensitivity.value = context.sensitivity;
    }
  }, [context.sensitivity]);

  useEffect(() => {
    const mat = matRef.current;
    if (mat) {
      mat.uniforms.contrast.value = context.contrast;
    }
  }, [context.contrast]);

  const uniforms = useMemo(
    () => ({
      time: {
        value: 0.0,
      },
      colourMap: {
        value: null,
      },
      scale: {
        value: null,
      },
      contrast: {
        value: context.contrast,
      },
      sensitivity: {
        value: context.sensitivity,
      },
      render: {
        value: null,
      },
    }),
    [],
  );

  return (
    <mesh scale={[viewport.width, viewport.height, 1]}>
      <planeGeometry args={[1, 1]} />
      <shaderMaterial
        fragmentShader={frag}
        vertexShader={vert}
        ref={matRef}
        uniforms={uniforms}
        side={FrontSide}
        glslVersion={GLSL3}
      />
    </mesh>
  );
};

const Visualiser2D: FC<{ context: IVisualiserContext }> = ({ context }) => {
  return (
    <div className="relative flex-1">
      <div className="absolute w-full h-full">
        <Canvas className="min-w-0 min-h-0 flex-1 shrink" orthographic>
          <Graph2D context={context} />
        </Canvas>
      </div>
    </div>
  );
};

export { Visualiser2D };
