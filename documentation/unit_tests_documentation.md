# `Unit Tests` Documentation #

## Description ##
Unit tests are defined for functionality check and error handling coverage while using the ISASPEC Decoder API. The unit tests implementation is based on the `catch2`(v2.13.10) testing framework. More on catch2 hereq: https://github.com/catchorg/Catch2.

## Tests Execution ##
Upon building the `amdisa_tests` target, the executable `"amdisa_tests.exe"` for windows and `"amdisa_tests.so"` for linux is generated in the output directory.

usage:
  amdisa_test.exe [<test name|pattern|tags> ... ] options\
where options are:\
  -?, -h, --help;                           Display usage information \
  -I, --spec \<spec\>;                      Path to the AMDISA XML specification [REQUIRED] \
  -F, --disassembly \<disassembly_file\>;   Path to the binary disassembly input file [OPTIONAL]

The unit tests are performed on the ISA XML specification provided. The relevant unit tests are executed for the ISA Architecture as described by the XML specification. The binary disassembly input file is only required for the tests that validate the decoder API for a complete shader disassembly decode.

## Tests Output ##
- Upon success, no assertion errors are observed.\
Success message: All tests passed

- Upon failure, assertion errors are raised. Also, the failing test condition and test description are described.\
For example,
```
-------------------------------------------------------------------------------
Test to decode of a single instruction binary
-------------------------------------------------------------------------------
FAILED:
  REQUIRE( info[0].bundle[0].instruction_name == "S_ADD_U32" )
with expansion:
  "S_ADD_CO_U32" == "S_ADD_U32"

test cases:  8 |  6 passed | 2 failed
```

## Unit Tests Description ##

### 1. XML Spec Initialization Tests ###

#### 1.1 XML Spec Initialize ####
Tests the successful initialization of the provided AMDISA specification xml.

#### 1.2 Fail Test: XML Spec Initialize ###
Tests the API for failure handling due to failied initialization of the provided AMDISA specification xml.

### 2. Decode Single Instruction Tests ###

#### 2.1 Decode single instruction by name ####
Tests the decoding of a single instruction by instruction name.

#### 2.2 Decode single instruction using binary ####
Tests the decoding of a single instruction using the instruction's binary representation (single dword).

#### 2.3 Decode an instruction stream ####
Tests the decoding of an instruction stream of an instruction represented with multiple dwords.

### 3. Decode Shader Disassembly Tests ###

#### 3.1 Test to decode of a shader disassembly text ####
Tests the decoding of a shader disassembly text.

#### 3.2 Test to fail decode of a shader disassembly text ####
Tests the API for failure handling due to failied decoding of a shader disassembly text.

#### 3.3 Test to fail decode of a shader disassembly file ####
Tests the API for failure handling due to failied decoding of a shader disassembly file.


