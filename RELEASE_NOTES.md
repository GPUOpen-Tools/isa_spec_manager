# AMD's Machine-Readable ISA Specification and Tools - Release Notes #

## Highlights of this release ##

AMD's machine-readable GPU Instruction Set Architecture specifications is a set of XML files that describe AMD's latest GPU ISA: instructions, encodings, operands, data formats and even human-readable description strings. 

The release includes the specification XML files for the following GPU architectures:
* AMD CDNA™ 4 (AMD MI350 Instinct™)
* AMD CDNA™ 3 (AMD MI300 Instinct™)
* AMD CDNA™ 2 (AMD MI200 Instinct™)
* AMD CDNA™ 1 (AMD MI100 Instinct™)
* AMD RDNA™ 3
* AMD RDNA™ 2
* AMD RDNA™ 1

The XML files can be downloaded from [GPUOpen.com](https://gpuopen.com/machine-readable-isa/).

This codebase includes the `IsaDecoder` API that can be used to decode, which can be used to decode AMD ISA assembly and disassembly using the specifications:
* Load XML specification files and automatically parse them, so you don't need to write your own parser.
* Decode single instructions and whole kernels and shaders in binary or text format.
* Handle multiple architectures in flight with the `DecodeManager` convenience API.

For usage examples and instructions on how to build the project, please see [source/examples subdirectory on the isa_spec_manager GitHub repository](https://github.com/GPUOpen-Tools/isa_spec_manager).

**Note:** while the `IsaDecoder` API is a good way to get started with parsing the XML files, nothing prevents you from parsing the files yourself and building your own custom workflow. To do that please refer to the XML schema documentation [XML schema documentation](https://github.com/GPUOpen-Tools/isa_spec_manager/blob/main/documentation/spec_documentation.md).

New in this release:
* Fixed decoding of `MIMG` instructions (such as `IMAGE_STORE` and `IMAGE_LOAD`)
* Fixed decoding of `MUBUF` and `MTBUF` instructions of RDNA™2 targets in binary representation.
* Fixed an issue where `DS` instructions returned the wrong operands when decoded via `IsaDecoder::DecodeInstruction()` with an `uint64_t` argument.
* The `explorer` API now supports Functional Groups.
* Added basic test for the `explorer` API.
* Documentation updates.


## Known issues ##

### Specification ###
* Information about encoding modifiers is not provided in the specification.

### API and tools ###