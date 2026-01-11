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
import { number } from "zod";

import {
  createColourMapTexture,
  createScaleTexture,
  defaultHeight,
  generateColourMap,
  generateRenderTexture,
} from "@/components/visualiser/visualiser_utils";
import {
  IVisualiserContext,
  VisualiserScale,
} from "@/context/visualiser_context";
import { VisualiserMetadata } from "@/ipc/visualiser_ipc";

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
        context.visualiserMetadata.sampleRate,
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

const XAxis: FC<{ visualiserMetadata: VisualiserMetadata }> = ({
  visualiserMetadata,
}) => {
  const maxIrRatio = 2.0;
  const svgRef = useRef<SVGSVGElement | null>(null);
  const [measureRef, { width, height }] = useMeasure<SVGSVGElement>();

  useEffect(() => {
    if (svgRef.current) {
      const svg = select(svgRef.current);

      const timeSeconds =
        visualiserMetadata.baseIrLengthSamples / visualiserMetadata.sampleRate;

      const xScale = scaleLinear()
        .domain([0, maxIrRatio * timeSeconds + 0.5])
        .range([0, width ?? 0]);
      const xAxis = axisBottom(xScale).ticks(
        Math.floor((width ?? 0) / 100.0) * 2,
      );
      svg.call(xAxis);
    }
  }, [width, visualiserMetadata]);

  return (
    <div>
      <svg
        className="w-full absolute h-6 pr-6 pl-12 bottom-0 ml-[-1px]"
        viewBox={`0 0 ${width} ${height}`}
        preserveAspectRatio="none"
        ref={(el) => {
          measureRef(el);
          svgRef.current = el;
        }}
      />
      <svg className=" absolute h-6 right-0 bottom-0 w-6 pr-2 ">
        <text
          x="8"
          y="22"
          transform="rotate(-90, 5, 12)"
          textAnchor="middle"
          fill="hsl(var(--foreground))"
          fontSize="10"
        >
          sec
        </text>
      </svg>
    </div>
  );
};

const YAxis: FC<{ scale: VisualiserScale }> = ({ scale }) => {
  const svgRef = useRef<SVGSVGElement | null>(null);
  const [measureRef, { width, height }] = useMeasure<SVGSVGElement>();

  function melToHz(mel: number): number {
    return 700 * (Math.pow(10, mel / 2595) - 1);
  }

  function hzToMel(frequency: number): number {
    return 2595 * Math.log10(1 + frequency / 700);
  }

  useEffect(() => {
    if (svgRef.current) {
      const svg = select(svgRef.current);
      svg.selectAll(".y-axis").remove();

      const yAxisGroup = svg.append("g").attr("class", "y-axis");
      switch (scale) {
        case "linear": {
          const yScale = scaleLinear()
            .domain([20, 20000])
            .range([height ?? 0, 0]);
          const yAxis = axisLeft(yScale);

          let tickValues = ticks(
            20,
            20000,
            Math.floor((height ?? 0) / 100.0) * 2,
          );

          if (!tickValues.includes(20)) {
            tickValues.unshift(20);
          }
          yAxis.tickValues(tickValues);

          yAxisGroup.call(yAxis);
          yAxisGroup.attr("transform", "translate(47, 0)").call(yAxis);

          break;
        }
        case "mel": {
          const yScale = scaleLinear()
            .domain([hzToMel(20), hzToMel(20000)])
            .range([height ?? 0, 0]);
          const yAxis = axisLeft(yScale);

          let tickValuesHz = [20, 512];
          let value = 1024;
          while (
            value <= 20000 &&
            tickValuesHz.length < Math.floor((height ?? 0) / 100.0) * 2 + 1
          ) {
            tickValuesHz.push(value);
            value *= 2;
          }
          tickValuesHz.push(20000);

          let melVals = tickValuesHz.map((hz) => hzToMel(hz));

          yAxis.tickValues(melVals).tickFormat(function (d, i) {
            return Math.round(melToHz(d.valueOf())).toString();
          });
          yAxisGroup.call(yAxis);

          yAxisGroup.attr("transform", "translate(47, 0)").call(yAxis);
          break;
        }
      }
    }
  }, [height, scale]);

  return (
    <div>
      <svg className=" absolute w-12 pr-2 pt-2 ">
        <text
          x="32"
          y="8"
          textAnchor="middle"
          fill="hsl(var(--foreground))"
          fontSize="10"
        >
          Hz
        </text>
      </svg>

      <svg
        className="h-full absolute w-12 pb-6 pt-6 left-0 "
        viewBox={`0 0 ${width} ${height}`}
        preserveAspectRatio="none"
        ref={(el) => {
          measureRef(el);
          svgRef.current = el;
        }}
      />
    </div>
  );
};

const Visualiser2D: FC<{ context: IVisualiserContext }> = ({ context }) => {
  const [measureRef, { width, height }] = useMeasure();

  return (
    <div className=" relative flex flex-1  " ref={measureRef}>
      <div
        className="absolute w-full h-full"
        style={{
          width: Math.floor(width ?? 0),
          height: Math.floor(height ?? 0),
        }}
      >
        <div className="absolute w-full h-full pt-6 pl-12 pr-6 pb-6">
          <Canvas className="min-w-0 min-h-0 flex-1 shrink" orthographic>
            <Graph2D context={context} />
          </Canvas>
        </div>
        <div className="w-full h-full absolute">
          <XAxis visualiserMetadata={context.visualiserMetadata} />
          <YAxis scale={context.scale} />
        </div>
      </div>
    </div>
  );
};

export { Visualiser2D };
