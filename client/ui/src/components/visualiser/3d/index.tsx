import { OrbitControls } from "@react-three/drei";
import { Canvas, useThree } from "@react-three/fiber";
import { FC, useEffect, useMemo, useRef } from "react";
import { DataTexture, DoubleSide, GLSL3, ShaderMaterial } from "three";

import {
  createColourMapTexture,
  createScaleTexture,
  defaultHeight,
  generateColourMap,
  generateRenderTexture,
} from "@/components/visualiser/visualiser_utils";
import {
  IVisualiserContext,
} from "@/context/visualiser_context";

import frag from "./visualiser.frag";
import vert from "./visualiser.vert";

const Graph3D: FC<{
  context: IVisualiserContext;
}> = ({ context }) => {
  const viewport = useThree((state) => state.viewport);
  const invalidate = useThree((state) => state.invalidate);

  const matRef = useRef<ShaderMaterial>(null);
  const sampleRate = context.visualiserMetadata.sampleRate;

  useEffect(() => {
    const applyRender = (nextRender: Uint8Array) => {
      const mat = matRef.current;
      if (!mat) return;

      const texture = generateRenderTexture(nextRender, sampleRate);
      replaceTexture(mat, "render", texture);
      invalidate();
    };

    const initialRender = context.getLatestRender();
    if (initialRender) {
      applyRender(initialRender);
    }

    return context.subscribeToRender(applyRender);
  }, [context, sampleRate, invalidate]);

  useEffect(() => {
    const mat = matRef.current;
    if (mat) {
      const texture = createScaleTexture(sampleRate, defaultHeight * 2, context.scale);
      replaceTexture(mat, "scale", texture);
      invalidate();
    }
  }, [context.scale, sampleRate, invalidate]);

  useEffect(() => {
    const mat = matRef.current;
    if (mat) {
      const texture = createColourMapTexture(generateColourMap(context.colourMap));
      replaceTexture(mat, "colourMap", texture);
      invalidate();
    }
  }, [context.colourMap, invalidate]);

  useEffect(() => {
    const mat = matRef.current;
    if (mat) {
      mat.uniforms.sensitivity.value = context.sensitivity;
      invalidate();
    }
  }, [context.sensitivity, invalidate]);

  useEffect(() => {
    const mat = matRef.current;
    if (mat) {
      mat.uniforms.contrast.value = context.contrast;
      invalidate();
    }
  }, [context.contrast, invalidate]);

  useEffect(() => {
    return () => {
      const mat = matRef.current;
      if (!mat) return;

      disposeTexture(mat.uniforms.render.value as DataTexture | null);
      disposeTexture(mat.uniforms.scale.value as DataTexture | null);
      disposeTexture(mat.uniforms.colourMap.value as DataTexture | null);
    };
  }, []);

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
          frameloop="demand"
        >
          <Graph3D context={context} />
          <axesHelper />
          <OrbitControls />
        </Canvas>
      </div>
    </div>
  );
};

const replaceTexture = (
  material: ShaderMaterial,
  uniformKey: "render" | "scale" | "colourMap",
  texture: DataTexture,
) => {
  const previousTexture = material.uniforms[uniformKey].value as
    | DataTexture
    | null
    | undefined;

  if (previousTexture && previousTexture !== texture) {
    previousTexture.dispose();
  }

  material.uniforms[uniformKey].value = texture;
};

const disposeTexture = (texture: DataTexture | null | undefined) => {
  texture?.dispose();
};

export { Visualiser3D };
