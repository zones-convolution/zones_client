# Convolver Web Page

## About

[Zones Convolver](https://github.com/zones-convolution/zones_convolver) is our state-of-the-art
convolution engine used in Zones as the underlying power of the plugin.
Just like the Zones plug-in, the convolution engine is completely free and open-source,
allowing you to easily integrate it into your own project.
It uses modified garcia schemes with time-distributed non-uniform partitions, making it an effective
convolver for short and long IR's. A huge feature is its ability to run all the real-time computations
on a single thread, making it ideal in a number of use cases where multi-threading is awkward or not possible.

[//]: # (## Profiling)

## Using the Library

The library comes with a ready-to-use convolution engine class. IR's can be loaded safely on a separate thread,
and each IR can be given a spec to control the routing and fading behaviour. The engine is ideal for using a
single IR with occasional changes as the time to swap to the new IR can't be guaranteed as it depends on the
loading thread and length of the IR. However, this provides the most efficient partitioning based on each IR
rather than a compromised best-fit scheme.
The class also handles varying block sizes and inherits from ```juce::dsp::ProcessorBase``` which allows easy
integration into a plug-in.

The underlying uniform and time-distributed non-uniform convolvers can also be used if you want to use your
own methods of safely loading IR's.

## Partitioning Schemes

For each IR, the convolver calculates the best partitioning based on the length and block size.
Garcia partitioning is used because sufficient clearance is need to allow for time-distribution of the FFTs.
These schemes are pre-calculated and embedded for varying block sizes and IR lengths. Partitions are added or removed
to match the IR length allowing close to optimal solutions. Furthermore, it provides flexibility for future
improvements as more schemes can be added to the look-up table easily.

## Time-Distributed Layouts

Non-uniform partitions achieve a close to even load distribution across buffers by decomposing the incoming data
in such a way to allow the FFT and IFFT of the incoming audio to be calculated as multiple smaller transforms.
For more information on how this is achieved, please read
our [documentation.](https://github.com/zones-convolution/zones_convolver/blob/main/resources/zones_convolver_documentation.pdf)

Without this solution, using non-uniform layouts results in uneven load distribution which greatly reduces
the performance as the max number of computations per buffer is reached sooner. Whilst multi-threading
also reduces this problem, it leads to complexity with implementation, scheduling and real-time guarantees.

## Routing Capabilities

The library also allows you to specify the input and output routing per IR. This allows uneven channel counts
for the input and IR as well as unusual combinations.
The channel count for the streamed audio is fixed when the Convolution Engine class is given a process spec.
However, the IR can have any number of channels. This allows you to switch from a stereo IR to Quadraphonic IR
without creating a new engine.

Convolving a stereo IR with a stereo input:

```
//Stereo routing
.input_routing = {0, 1},
.output_routing = {0, 1}
```

Convolving the first two channels of the IR with the left channel
and the last two channels with the right channel:

```
//Quadraphonic to stereo routing
.input_routing = {0, 0, 1, 1},
.output_routing = {0, 0, 1, 1}
```

This setup also allows true stereo convolution and other combinations.

```
//True stereo routing
.input_routing = {0, 0, 1, 1},
.output_routing = {0, 1, 0, 1}
```

## Fading Capabilities

Swapping between IR's is smooth thanks to the ```.fade_strategy``` spec. Cross-fade with process both IR's and apply
a simple fade between the two, with the length determined by the length of the IR. In-out fading fades out
the old IR before fading in the new one. This has the advantage of only processing one convolver at a time,
which can prevent overload when dealing with long IR's. In the Zones plug-in, we utilise this spec to
cross-fade when possible for a seamless transition, until the IR is above a certain length, when we resort to
in-out fading.



