import { Canvas, useFrame, useThree } from "@react-three/fiber";
import { FC, useEffect, useMemo, useRef } from "react";
import {
  DataTexture,
  FrontSide,
  GLSL3,
  RedFormat,
  ShaderMaterial,
} from "three";

import {
  createColourMapTexture,
  generateColourMap,
} from "@/components/visualiser/3d";
import { VisualiserRender } from "@/ipc/visualiser_ipc";

import frag from "./visualiser.frag";
import vert from "./visualiser.vert";

const defaultWidth = 512;
const defaultHeight = 256;

const generateRenderTexture = (render: VisualiserRender): DataTexture => {
  const size = defaultWidth * defaultHeight;
  const data = new Uint8Array(size);

  for (let i = 0; i < defaultWidth; ++i) {
    for (let j = 0; j < defaultHeight; ++j) {
      const index = i + j * defaultHeight;
      data[index] = 0;
    }
  }

  let width = render.length;

  for (let i = 0; i < width; ++i) {
    for (let j = 0; j < defaultHeight; ++j) {
      const index = i + j * defaultWidth;
      data[index] = Math.floor(render[i][j] * 255);
    }
  }

  const texture = new DataTexture(data, defaultWidth, defaultHeight, RedFormat);
  texture.needsUpdate = true;
  return texture;
};

const FullscreenMesh: FC<{ render: VisualiserRender }> = ({ render }) => {
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
      render: {
        value: generateRenderTexture(render),
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

const Visualiser2D: FC<{ render: VisualiserRender }> = ({ render }) => {
  return (
    <div className="relative flex-1">
      <div className="absolute w-full h-full">
        <Canvas className="min-w-0 min-h-0 flex-1 shrink" orthographic>
          <FullscreenMesh render={render} />
        </Canvas>
      </div>
    </div>
  );
};

export { Visualiser2D };
