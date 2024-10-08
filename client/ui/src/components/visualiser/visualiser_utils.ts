import colormap from "colormap";
import { DataTexture, FloatType, RedFormat, RGBAFormat } from "three";

import {
  VisualiserColourMap,
  VisualiserScale,
} from "@/context/visualiser_context";

export const defaultWidth = 1024;
export const defaultHeight = 512;

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

export const createScaleTexture = (
  sampleRate: number,
  windowSize: number,
  scale: VisualiserScale,
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

export const generateColourMap = (
  colourMap: VisualiserColourMap,
): number[][] => {
  return colormap({
    colormap: colourMap,
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
