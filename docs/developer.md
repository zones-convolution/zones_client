# Developer

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
![Tests](https://img.shields.io/github/actions/workflow/status/zones-convolution/zones_convolver/zones_convolver_tests.yml?style=for-the-badge&logo=github&label=TESTS)

![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![JUCE Badge](https://img.shields.io/badge/JUCE-8DC63F?logo=juce&logoColor=fff&style=for-the-badge)

---

<br />
<div align="center">
  <a href="https://github.com/zones-convolution/zones_client">
    <img src="resources/zones_icon.png" alt="Logo" width="80" height="80">
  </a>

<h3 align="center">Zones Client</h3>

  <p align="center">

# Introduction

Zones client is a convolution plugin for using Impulse Responses uploaded to ZonesConvolution.com. It
allows browsing of user uploaded IR's and editing directly in a DAW. This repository contains the
source code of the plugin and technical documentation about the plugin's architecture.

For more information about uploading and using Zones practically see *ZonesConvolution.com*.

For more information about the underlying convolution algorithm used, see *ZonesConvolver*.

To get involved with developing the plugin, uploading Zones or simply the using the plugin, please
join our Discord server.

# Getting Started

downloading repository, build configurations. Alternatively, the plugin can be directly downloaded from
*ZonesConvolution.com*

# Contributing

We would really welcome any contributions, feel free to open up a PR and we'll review it as soon as possible. The
project largely follows the Google C++ coding style and provides a ```.clang-format``` that should be used.

Currently on our roadmap,

*

## Contribution Guide

Understanding architecture and design goals, following our roadmap...

# License

Distributed under the MIT License. See `LICENSE` for more information.

# Contact

Feel free to get in touch,

**Leon Paterson-Stephens** - leon@leonps.com

**Micah Strange** - micahstrange16@gmail.com

# Library Introduction

## Overview of the architecture

*high-level-figure* provides a high level overview of the architecture. We use a webview UI which
handles getting the list of zones from the web API. Communication with the backend is done across
relays.

When an IR is selected, the relevant metadata is sent to the backend so that the Audio can be
downloaded on the C++ side. From here the audio can be written to disk and then loaded to be formatted
in the correct way to pass to IR engine.

IR engine handles the augmentation of the IR offline in the IR graph, which contains a number
of processors that pass the buffer along sequentially.

The buffer is then passed to the audio engine where it can be safely loaded into the convolver.
The audio graph handles the processing on the real-time thread, including the convolution and other
real-time parameters such as dry/wet mix.

The following sections provide a more detailed overview of specific modules.

## Web API

## Visualiser

## IR Engine

## Audio Engine

