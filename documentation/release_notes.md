# AMD's Machine-Readable ISA Specification and Tools - Release Notes #

## Highlights of this release ##

Meet AMD's machine-readable GPU Instruction Set Architecture specifications: a set of XML files that describe anything computer programs need to know about AMD's latest GPU ISA: instructions, encodings, operands, data formats and even human-readable description strings. You do not even have to spend time parsing the XML specification files - we released a decoding API that does that for you. Using the ISA decoding API computer programs can break down any AMD GPU assembly instruction, basic block and even whole shaders.

This release includes the specification XML files for the following architectures:
* MI-300
* MI-200
* MI-100
* RDNA3
* RDNA2
* RDNA
* Vega

In addition, this release includes the codebase for the `IsaDecoder` API, which can be used to decode AMD ISA assembly and disassembly using the specifications:
* Load XML specification files and automatically parse them, so you don't need to write your own parser.
* Decode single instructions and whole kernels and shaders in binary or text format.
* Handle multiple architectures in flight with the `DecodeManager` convenience API.

For usage examples and instructions on how to build the project, please see [isa_spec_manager GitHub repository](https://github.com/GPUOpen-Tools/isa_spec_manager).

## Known issues ##

### Specification ###
* Mention known issues about the specification files

### API and tools ###

* Mention known issues about the API Tools

