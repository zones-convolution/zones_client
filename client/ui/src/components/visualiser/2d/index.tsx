import { Canvas, useThree } from "@react-three/fiber";
import { useMeasure } from "@uidotdev/usehooks";
import {
  select,
  scaleLinear,
  scaleLog,
  axisBottom,
  format,
  range,
  axisLeft,
  axisRight,
  ticks,
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
      mat.uniforms.render.value = generateRenderTexture(
        context.render,
        context.sampleRate,
      );
    }
  }, [context.render]);

  useEffect(() => {
    const mat = matRef.current;
    if (mat) {
      mat.uniforms.scale.value = createScaleTexture(
        48000,
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

const XAxis = () => {
  const timeSeconds = 1.4;
  const svgRef = useRef<SVGSVGElement | null>(null);
  const [measureRef, { width, height }] = useMeasure<SVGSVGElement>();

  useEffect(() => {
    if (svgRef.current) {
      const svg = select(svgRef.current);

      const xScale = scaleLinear()
        .domain([0, 2.0 * timeSeconds])
        .range([0, width ?? 0]);
      const xAxis = axisBottom(xScale).ticks(
        Math.floor((width ?? 0) / 100.0) * 2,
      );
      svg.call(xAxis);
    }
  }, [width]);

  return (
    <svg
      className="w-full absolute h-6 pr-10 pl-2 bottom-0"
      viewBox={`0 0 ${width} ${height}`}
      preserveAspectRatio="none"
      ref={(el) => {
        measureRef(el);
        svgRef.current = el;
      }}
    />
  );
};

const YAxis = () => {
  const svgRef = useRef<SVGSVGElement | null>(null);
  const [measureRef, { width, height }] = useMeasure<SVGSVGElement>();

  useEffect(() => {
    if (svgRef.current) {
      const svg = select(svgRef.current);

      const yScale = scaleLinear()
        .domain([20, 20000])
        .range([height ?? 0, 0]);
      const yAxis = axisRight(yScale);
      let tickValues = ticks(20, 20000, Math.floor((height ?? 0) / 100.0) * 2);
      if (!tickValues.includes(20)) {
        tickValues.unshift(20);
      }
      yAxis.tickValues(tickValues);

      svg.call(yAxis);
    }
  }, [height]);

  return (
    <svg
      className="h-full absolute w-10 pb-6 pt-2 right-0"
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
  const [measureRef, { width, height }] = useMeasure();

  return (
    <div className="flex flex-1 justify-center items-center" ref={measureRef}>
      <div
        className="relative"
        style={{
          width: Math.floor(width ?? 0),
          height: Math.floor(height ?? 0),
        }}
      >
        <div className="absolute w-full h-full pt-2 pl-2 pr-10 pb-6">
          <Canvas className="min-w-0 min-h-0 flex-1 shrink" orthographic>
            <Graph2D context={context} />
          </Canvas>
        </div>
        <div className="w-full h-full absolute">
          <XAxis />
          <YAxis />
        </div>
      </div>
      <div className="w-full h-full absolute">{context.sampleRate}</div>
    </div>
  );
};

export { Visualiser2D };
