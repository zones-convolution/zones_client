import { Canvas, useFrame, useThree } from "@react-three/fiber";
import { FC, useEffect, useMemo, useRef } from "react";
import {
  DataTexture,
  FloatType,
  FrontSide,
  GLSL3,
  RedFormat,
  ShaderMaterial,
} from "three";

import {
  createColourMapTexture,
  generateColourMap,
} from "@/components/visualiser/3d";

import frag from "./visualiser.frag";
import vert from "./visualiser.vert";

const defaultWidth = 1024;
const defaultHeight = 512;

export const generateRenderTexture = (render: Uint8Array): DataTexture => {
  const size = defaultWidth * defaultHeight;
  const data = new Uint8Array(size);

  let numChannels = render.length / defaultHeight;

  for (let channel = 0; channel < numChannels; ++channel) {
    const channelOffset = channel * defaultHeight;
    const x = channel;

    for (let fftPoint = 0; fftPoint < defaultHeight; ++fftPoint) {
      const point = render[fftPoint + channelOffset];
      const y = fftPoint;
      const textureIndex = x + y * defaultWidth;
      data[textureIndex] = point;
    }
  }

  const texture = new DataTexture(data, defaultWidth, defaultHeight, RedFormat);
  texture.needsUpdate = true;
  return texture;
};

const createScaleTexture = (
  sampleRate: number,
  windowSize: number,
  scale: "linear" | "mel",
) => {
  const data = new Float32Array(defaultHeight);

  for (let i = 0; i < defaultHeight; i += 1) {
    switch (scale) {
      case "linear":
        data[i] = i / (defaultHeight - 1);
        break;
      case "mel": {
        const peakHz = (sampleRate * (windowSize - 2)) / (2 * windowSize);
        data[i] =
          (700 * ((1 + peakHz / 700) ** (i / (defaultHeight - 1)) - 1)) /
          peakHz;
        break;
      }
    }
  }

  const texture = new DataTexture(data, defaultHeight, 1, RedFormat);
  texture.type = FloatType;
  texture.needsUpdate = true;
  return texture;
};

const FullscreenMesh: FC<{ render: Uint8Array }> = ({ render }) => {
  const viewport = useThree((state) => state.viewport);

  const matRef = useRef<ShaderMaterial>(null);

  useFrame(() => {
    const mat = matRef.current;
    if (mat) {
      mat.uniforms.time.value += 0.1;
      // mat.uniforms.contrast.value =
      //   ((Math.sin(mat.uniforms.time.value * 0.1) + 1.0) / 2.0) * 100.0;
      // mat.uniforms.sensitivity.value =
      //   ((Math.sin(mat.uniforms.time.value * 0.2) + 1.0) / 2.0) * 2.0;
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
      scale: {
        value: createScaleTexture(44100, defaultHeight * 2, "mel"),
      },
      contrast: {
        value: 0.0,
      },
      sensitivity: {
        value: 0.0,
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

const Visualiser2D: FC<{ render: Uint8Array }> = ({ render }) => {
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
