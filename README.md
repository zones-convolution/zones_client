# CMake Format

For formatting CMake files, the cmake-format tool is used, documented here https://github.com/cheshirekow/cmake_format .

A CLion external tool can also be setup to setup a command within the IDE to perform formatting, this article describes
this setup https://medium.com/@amitiitm2009/cmake-formatter-in-clion-7c1917763b34 .

# Dependencies

Currently, a number of dependencies exist as submodules of the repo including JUCE, Lager, Immer and Zug. When pulling
the repo for development, submodules should be fetched recursively.

Other dependencies including Boost (used by Lager) and Catch2 for testing are managed by VCPKG and a manifest is
included in the root of the repository containing specific dependency versions. Using VCPKG in Clion (> 2023.1) is
supprted and documented here https://blog.jetbrains.com/clion/2023/01/support-for-vcpkg-in-clion/ . 