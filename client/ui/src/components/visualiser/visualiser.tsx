import { OrbitControls } from "@react-three/drei";
import { Canvas, useFrame } from "@react-three/fiber";
import colormap from "colormap";
import { useMemo, useRef } from "react";
import {
  DataTexture,
  DoubleSide,
  GLSL3,
  RedFormat,
  RGBAFormat,
  ShaderMaterial,
} from "three";

import frag from "./visualiser.frag";
import vert from "./visualiser.vert";

const generateColourMap = (): number[][] => {
  return colormap({
    colormap: "viridis",
    nshades: 256,
    format: "rgba",
    alpha: 1,
  });
};

const createColourMapTexture = (colourMapArray: number[][]): DataTexture => {
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

const generateDefaultTexture = (): DataTexture => {
  const width = 1024;
  const height = 1024;
  const size = width * height;
  const data = new Uint8Array(size);

  for (let i = 0; i < width; ++i) {
    for (let j = 0; j < height; ++j) {
      const index = i + j * width;
      const redValue = (Math.sin(j * 0.02) * Math.sin(i * 0.02) + 1) * 0.5;
      data[index] = Math.floor(redValue * 255);
    }
  }

  const texture = new DataTexture(data, width, height, RedFormat);
  texture.needsUpdate = true;

  return texture;
};

const Graph = () => {
  const matRef = useRef<ShaderMaterial>(null);

  useFrame(() => {
    const mat = matRef.current;
    if (mat) {
      mat.uniforms.time.value += 0.1;
    }
  });

  const uniforms = useMemo(
    () => ({
      time: {
        value: 0.0,
      },
      colourMap: {
        value: createColourMapTexture(generateColourMap()),
      },
      graphTexture: {
        value: generateDefaultTexture(),
      },
    }),
    [],
  );

  return (
    <mesh position={[0, 0, 0]} rotation={[-Math.PI / 2, 0, 0]}>
      <planeGeometry args={[1, 1.6, 100, 200]} />
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

const Visualiser = () => {
  return (
    <div className="relative flex-1">
      <div className="absolute w-full h-full">
        <Canvas
          camera={{ position: [1.0, 1.0, 1.0] }}
          className="min-w-0 min-h-0 flex-1 shrink"
        >
          <Graph />
          <axesHelper />
          <OrbitControls />
        </Canvas>
      </div>
    </div>
  );
};

export { Visualiser };
