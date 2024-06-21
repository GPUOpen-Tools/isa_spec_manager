# isa_spec_manager
A set of tools for parsing and using AMD's machine-readable GPU ISA specifications.

The `IsaDecoder` API makes it easy to parse the specification XML files, decode instructions and even decode whole shaders.

For usage examples, see the [examples subfolder](https://github.com/GPUOpen-Tools/isa_spec_manager/tree/main/source/examples).

## Building isa_spec_manager
To build the project, use the build scripts located in the ./build subfolder. Please note that the build process requires CMake with minimum version of 3.0.

### Building on Linux
```
cd ./isa_spec_manager/build
./prebuild_linux.sh
cd linux
make
```

The above script will launch the cmake. The script will generate projects directory.

### Building on Windows
```
cd ./isa_spec_manager/build
./prebuild_windows.bat
```

The above script will create a windows directory and generate a solution for Visual Studio.

## Using the API
The following example files can give you a quick overview of how to start using the XML ISA spec in your project:
* Basic usage example: [./source/examples/basic_decoder.cpp](./source/examples/basic_decoder.cpp)
* Usage example with multiple architectures in flight: [./source/examples/multi_arch_decoder.cpp](./source/examples/multi_arch_decoder.cpp)

## Getting the ISA specification files
The Machine-Readable GPU ISA specification files can be downloaded from [AMD's Machine-Readable GPU ISA Specification page on GPUOpen.com](https://gpuopen.com/machine-readable-isa/).
