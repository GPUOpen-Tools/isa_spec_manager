/*
 * Copyright (c) 2025 Advanced Micro Devices, Inc. All rights reserved.
 */
// C++ libraries.
#include <vector>
#include <fstream>
#include <memory>

// Local libraries.
#include "amdisa_tests.h"
#include "amdisa/isa_decoder.h"
#include "amdisa_utility.h"
#include "catch.hpp"

// Define machine code to assembly code mapping for different architectures.
static void GetTestCaseSingle(const amdisa::GpuArchitecture architecture,
    uint32_t& machine_code, std::string& instruction_name)
{
    machine_code = 0x80000002;
    instruction_name = "S_ADD_U32";
    if (architecture == amdisa::GpuArchitecture::kRdna4 ||
        architecture == amdisa::GpuArchitecture::kCdna5)
    {
        instruction_name = "S_ADD_CO_U32";
    }
}

static void GetTestCaseStream(const amdisa::GpuArchitecture architecture,
    std::vector<uint32_t>& machine_code_stream, std::string& instruction_name)
{
    machine_code_stream = { 0x801FFF1F, 0x00000000 };
    instruction_name = "S_ADD_U32";
    if (architecture == amdisa::GpuArchitecture::kRdna4 ||
        architecture == amdisa::GpuArchitecture::kCdna5)
    {
        instruction_name = "S_ADD_CO_U32";
    }
}

TEST_CASE("Test to decode of a single instruction name", "[isa_decoder][text][single][name]")
{
    amdisa::IsaDecoder decoder;
    std::string        msg;
    TestConfig&        config = TestConfig::getInstance();
    REQUIRE(decoder.Initialize(config.GetXmlPath(), msg));

    amdisa::InstructionInfo info;
    REQUIRE(decoder.DecodeInstruction("S_MOV_B32", info, msg));
    REQUIRE(info.instruction_name == "S_MOV_B32");
}

TEST_CASE("Test to decode of a single instruction binary", "[isa_decoder][binary][single]")
{
    amdisa::IsaDecoder decoder;
    std::string        msg;
    TestConfig&        config = TestConfig::getInstance();
    REQUIRE(decoder.Initialize(config.GetXmlPath(), msg));

    const auto kArchitecture = decoder.GetArchitecture();
    REQUIRE(kArchitecture != amdisa::GpuArchitecture::kUnknown);

    uint32_t machine_code = 0;
    std::string expected_instruction_name;
    GetTestCaseSingle(kArchitecture, machine_code, expected_instruction_name);

    amdisa::InstructionInfoBundle info;
    REQUIRE(decoder.DecodeInstruction(machine_code, info, msg));
    REQUIRE(info.bundle[0].instruction_name == expected_instruction_name);
}

TEST_CASE("Test to decode of an instruction stream", "[isa_decoder][binary][stream]")
{
    amdisa::IsaDecoder decoder;
    std::string        msg;
    TestConfig&        config = TestConfig::getInstance();
    REQUIRE(decoder.Initialize(config.GetXmlPath(), msg));

    const auto kArchitecture = decoder.GetArchitecture();
    REQUIRE(kArchitecture != amdisa::GpuArchitecture::kUnknown);

    std::vector<uint32_t> machine_code_stream;
    std::string expected_instruction_name;
    GetTestCaseStream(kArchitecture, machine_code_stream, expected_instruction_name);

    std::vector<amdisa::InstructionInfoBundle> info_stream;
    REQUIRE(decoder.DecodeInstructionStream(machine_code_stream, info_stream, msg));
    REQUIRE(info_stream[0].bundle[0].instruction_name == expected_instruction_name);
}

TEST_CASE("Test utility strip function", "[utils]")
{
    REQUIRE(amdisa::AmdIsaUtility::Strip("  Hello World  ") == "Hello World");
    REQUIRE(amdisa::AmdIsaUtility::Strip("    Leading spaces") == "Leading spaces");
    REQUIRE(amdisa::AmdIsaUtility::Strip("Trailing spaces    ") == "Trailing spaces");
    REQUIRE(amdisa::AmdIsaUtility::Strip("\t\n  Mixed whitespace \n\r\t") == "Mixed whitespace");
    REQUIRE(amdisa::AmdIsaUtility::Strip("NoSpaces") == "NoSpaces");
    REQUIRE(amdisa::AmdIsaUtility::Strip("") == "");
    REQUIRE(amdisa::AmdIsaUtility::Strip("    ") == "");
    REQUIRE(amdisa::AmdIsaUtility::Strip("") == "");
    REQUIRE(amdisa::AmdIsaUtility::Strip("\n\t\r") == "");
    REQUIRE(amdisa::AmdIsaUtility::Strip("A") == "A");
    REQUIRE(amdisa::AmdIsaUtility::Strip(" ") == "");
    REQUIRE(amdisa::AmdIsaUtility::Strip("  Middle   Spaces  ") == "Middle   Spaces");
    REQUIRE(amdisa::AmdIsaUtility::Strip(" \t \n !@#$%^&*() \n \r ") == "!@#$%^&*()");
}

TEST_CASE("Test to decode of an instruction stream (using in-memory XML)", "[isa_decoder][single][membuf]")
{
    amdisa::IsaDecoder decoder;
    std::string        msg;
    TestConfig&        config = TestConfig::getInstance();
    std::ifstream      file(config.GetXmlPath(), std::ios::binary);
    REQUIRE(file.is_open());

    file.seekg(0, std::ios::end);
    std::streampos filesz = file.tellg();
    file.seekg(0, std::ios::beg);

    auto buf = std::make_unique<char[]>(filesz);
    file.read(buf.get(), filesz);
    file.close();

    REQUIRE(decoder.Initialize(buf.get(), filesz, msg));

    const auto kArchitecture = decoder.GetArchitecture();
    REQUIRE(kArchitecture != amdisa::GpuArchitecture::kUnknown);

    std::vector<uint32_t> machine_code_stream;
    std::string expected_instruction_name;
    GetTestCaseStream(kArchitecture, machine_code_stream, expected_instruction_name);

    std::vector<amdisa::InstructionInfoBundle> info_stream;
    REQUIRE(decoder.DecodeInstructionStream(machine_code_stream, info_stream, msg));
    REQUIRE(info_stream[0].bundle[0].instruction_name == expected_instruction_name);
}
