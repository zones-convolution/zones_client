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
downloaded on the C++ side. From here the audio can be written to disk and then loaded and formatted
in the correct way to pass to IR engine.

IR engine handles the augmentation of the IR offline in the IR graph, which contains a number
of processors that pass the buffer along sequentially.

The buffer is then passed to the audio engine where it can be safely loaded into the convolver.
The audio graph handles the processing on the real-time thread, including the convolution and other
real-time parameters such as dry/wet mix.

The following sections provide a more detailed overview of specific modules.

## Web UI

how the webview ui works / relays

## Web API

how we get data from the web

## Saving IR's to Disk

how zones are formatted

## Loading IR's

IR engine is owned by the processor container. As the processor container also owns the audio engine and visualiser
controller,
it is able to add them as listeners of IR engine in its constructor. The other task processor container has is to
pass a reference of IR Engine to IR controller, which calls IR Engine's functions.

### IR Controller

A good place to start is in IRController::LoadIR(), which is called on a background thread when an IR has been selected.
Here the IR Selection (which contains the relevant metadata) is used to load the IR from disk and format the buffer.
A mutex for the current graph state is then obtained to update it with the new audio.
IrEngine::RenderState() can then be called, passing in the state stored by IRController.

IRGraphState is a struct containing the parameter values for IR engine controller, the base IR buffer that was loaded
from disk and
other useful data such as sample rate and bit depth. immer::box is used to wrap the audio buffer so that it can be
passed around
without lots of copying

Another role of IR Controller is to collect the relevant parameter values and add them to the graph state. A re-render
can then
be triggered, with a de-bounce time added to prevent excessive render calls.

### Ir Engine

IR Engine owns the IRGraph, IrEngineProcessors, listener list and ProcessResultPool. It is responsible for organising
processing
on the IRGraph and caching results, then letting the listeners know when a render is complete.

In RenderState(), IR engine obtains a mutex and creates a new job which calls process() on IR graph. It will only pass
on
the result if job->shouldExit() is false. This means that it hasn't been invalidated by a more recent render result.

### IR Graph

For each state that is passed to IR engine, a new graph is created. This allows the possibilty of different processors
to be added and removed based on the target format.
IrGraph::Process() loops through each processor. If a result for that processor for a given state already exists, it
obtains
the buffer and passes it on to the next processor. Otherwise, the processor is called then the result is stored in the
ProcessResultPool. This means that if you were to alter a parameter at the end of the chain, the entire graph would
not be re-processed.

### Audio Engine

As a listener of IrEngine, AudioEngine::RenderFinished() is called when the buffer is ready
to be passed to the convolver. Here it checks that the target format is compatible then
loads the IR into the convolution engine with a given ConvolverSpec.

It also has a part in the notification/command queue system to update the player state as this
communication is bidirectional and not part of the parameter tree.

### Audio Graph

Audio graph is a relatively simple processor, inheriting from juce::dsp::ProcessorBase.
Unlike IrGraph, the processors are fixed and handle real time controls, with smoothing
where necessary.

## Other Features/Modules

### Visualiser

### Meter



