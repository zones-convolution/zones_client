import { Canvas, useThree } from "@react-three/fiber";
import { useMeasure } from "@uidotdev/usehooks";
import {
  select,
  line,
  curveCardinal,
  scaleLinear,
  axisBottom,
  axisLeft,
} from "d3";
import { FC, useEffect, useMemo, useRef, useState } from "react";
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

const data = [
  { x: 0, y: 10 },
  { x: 1, y: 20 },
  { x: 2, y: 15 },
  { x: 3, y: 25 },
  { x: 4, y: 30 },
];

const Axis = () => {
  const svgRef = useRef<SVGSVGElement | null>(null);
  const [measureRef, { width, height }] = useMeasure<SVGSVGElement>();

  useEffect(() => {
    if (svgRef.current) {
      const svg = select(svgRef.current);

      const xScale = scaleLinear()
        .domain([0, 60])
        .range([0, width ?? 0]);
      const xAxis = axisBottom(xScale).ticks(data.length);
      svg.call(xAxis);
    }
  }, [width]);

  return (
    <svg
      className="w-full h-full p-2"
      viewBox={`0 0 ${width} ${height}`}
      preserveAspectRatio="none"
      ref={(el) => {
        measureRef(el);
        svgRef.current = el;
      }}
    />
  );
};

const Visualiser2D: FC<{ context: IVisualiserContext }> = ({ context }) => {
  return (
    <div className="relative flex-1">
      <div className="absolute w-full h-full p-2">
        <Canvas className="min-w-0 min-h-0 flex-1 shrink" orthographic>
          <Graph2D context={context} />
        </Canvas>
      </div>
      <div className="absolute w-full h-full">
        <Axis />
      </div>
    </div>
  );
};

export { Visualiser2D };
