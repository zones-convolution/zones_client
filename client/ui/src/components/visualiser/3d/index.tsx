import { OrbitControls } from "@react-three/drei";
import { Canvas, useFrame, useThree } from "@react-three/fiber";
import colormap from "colormap";
import { FC, useEffect, useMemo, useRef } from "react";
import {
  DataTexture,
  DoubleSide,
  GLSL3,
  RedFormat,
  RGBAFormat,
  ShaderMaterial,
} from "three";

import {
  createScaleTexture,
  defaultHeight,
  generateRenderTexture,
} from "@/components/visualiser/2d";

import frag from "./visualiser.frag";
import vert from "./visualiser.vert";

export const generateColourMap = (): number[][] => {
  return colormap({
    colormap: "viridis",
    nshades: 256,
    format: "rgba",
    alpha: 1,
  });
};

export const createColourMapTexture = (
  colourMapArray: number[][],
): DataTexture => {
  const size = colourMapArray.length;
  const data = new Uint8Array(size * 4);

  for (let i = 0; i < size; i++) {
    const color = colourMapArray[i];
    const index = i * 4;
    data[index] = color[0];
    data[index + 1] = color[1];
    data[index + 2] = color[2];
    data[index + 3] = color[3] * 255;
  }

  const texture = new DataTexture(data, size, 1, RGBAFormat);
  texture.needsUpdate = true;
  return texture;
};

const Graph: FC<{ render: Uint8Array }> = ({ render }) => {
  const viewport = useThree((state) => state.viewport);

  const matRef = useRef<ShaderMaterial>(null);

  useFrame(() => {
    const mat = matRef.current;
    if (mat) {
      mat.uniforms.time.value += 0.1;
    }
  });

  useEffect(() => {
    const mat = matRef.current;
    if (mat) {
      mat.uniforms.render.value = generateRenderTexture(render);
    }
  }, [render]);

  const uniforms = useMemo(
    () => ({
      time: {
        value: 0.0,
      },
      colourMap: {
        value: createColourMapTexture(generateColourMap()),
      },
      scale: {
        value: createScaleTexture(44100, defaultHeight * 2, "mel"),
      },
      render: {
        value: generateRenderTexture(render),
      },
      contrast: {
        value: 40.0,
      },
      sensitivity: {
        value: 40.0,
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

const Visualiser3D: FC<{ render: Uint8Array }> = ({ render }) => {
  return (
    <div className="relative flex-1">
      <div className="absolute w-full h-full">
        <Canvas
          camera={{ position: [1.0, 1.0, 1.0] }}
          className="min-w-0 min-h-0 flex-1 shrink"
        >
          <Graph render={render} />
          <axesHelper />
          <OrbitControls />
        </Canvas>
      </div>
    </div>
  );
};

export { Visualiser3D };
