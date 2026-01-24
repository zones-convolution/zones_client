import { OrbitControls } from "@react-three/drei";
import { Canvas, useThree } from "@react-three/fiber";
import { FC, useEffect, useMemo, useRef } from "react";
import { DoubleSide, GLSL3, ShaderMaterial } from "three";

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

const Graph3D: FC<{ context: IVisualiserContext }> = ({ context }) => {
  const viewport = useThree((state) => state.viewport);

  const matRef = useRef<ShaderMaterial>(null);

  useEffect(() => {
    const mat = matRef.current;
    if (mat && context.render) {
      mat.uniforms.render.value = generateRenderTexture(
        context.render,
        context.visualiserMetadata.sampleRate,
      );
    }
  }, [context.render, context.visualiserMetadata]);

  useEffect(() => {
    const mat = matRef.current;
    if (mat) {
      mat.uniforms.scale.value = createScaleTexture(
        context.visualiserMetadata.sampleRate,
        defaultHeight * 2,
        context.scale,
      );
    }
  }, [context.scale, context.visualiserMetadata]);

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
      render: {
        value: null,
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
    }),
    [],
  );

  return (
    <mesh
      position={[0, 0, 0]}
      rotation={[-Math.PI / 2, 0, 0]}
      scale={[viewport.width * 0.8, viewport.height * 0.8, 1]}
    >
      <planeGeometry args={[1, 1, 1024, 512]} />
      <shaderMaterial
        fragmentShader={frag}
        vertexShader={vert}
        ref={matRef}
        uniforms={uniforms}
        side={DoubleSide}
        glslVersion={GLSL3}
      />
    </mesh>
  );
};

const Visualiser3D: FC<{ context: IVisualiserContext }> = ({ context }) => {
  return (
    <div className="relative flex-1">
      <div className="absolute w-full h-full">
        <Canvas
          camera={{ position: [1.0, 1.0, 1.0] }}
          className="min-w-0 min-h-0 flex-1 shrink"
        >
          <Graph3D context={context} />
          <axesHelper />
          <OrbitControls />
        </Canvas>
      </div>
    </div>
  );
};

export { Visualiser3D };
