# Getting Started - CLion

To get started with CLion, follow these steps:

1. Ensure all git submodules have been fetched recursively.
2. Make sure you have CLion version `>2023.1` installed, as it is required to support VCPKG.
3. Install VCPKG via CLion using the following steps (see dependencies for more information on this):
    - Open CLion and go to `View | Tool Windows | Vcpkg` interface.
    - In the Vcpkg panel, click the `+` button located at the top left.
    - Use the following details:
        - **Name**: vcpkg
        - **URL**: https://github.com/microsoft/vcpkg
        - **Directory**: Use the suggested directory.
4. After installing VCPKG, reload the CMake project.

# CMake Format

To format CMake files, the cmake-format tool is used. You can find the documentation for cmake-format
at [cheshirekow/cmake_format](https://github.com/cheshirekow/cmake_format).

You can also set up a CLion external tool to perform formatting within the IDE. This
article [@amitiitm2009/cmake-formatter-in-clion-7c1917763b34](https://medium.com/@amitiitm2009/cmake-formatter-in-clion-7c1917763b34)
provides instructions for setting up the tool.

# Dependencies

The repository has several dependencies as submodules, including JUCE, Lager, Immer, and Zug. When cloning the
repository for development, make sure to fetch the submodules recursively.

Some other dependencies, such as Boost (used by Lager) and Catch2 for testing, are managed by VCPKG. The repository
includes a manifest in the root directory, which specifies the specific versions of these dependencies.
CLion ```> 2023.1```
supports and documents the usage of VCPKG. You can find more information about using VCPKG in CLion
at [clion/2023/01/support-for-vcpkg-in-clion/](https://blog.jetbrains.com/clion/2023/01/support-for-vcpkg-in-clion/).
