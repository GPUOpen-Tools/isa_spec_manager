/*
 * Copyright (c) 2025 Advanced Micro Devices, Inc. All rights reserved.
 */
#include "amdisa_tests.h"
#include "amdisa/isa_decoder.h"
#include "catch.hpp"

TEST_CASE("Test to fail decode of a shader disassembly text", "[isa_decoder][decode][text][fail]")
{
    amdisa::IsaDecoder decoder;
    std::string        msg;
    TestConfig&        config = TestConfig::getInstance();
    REQUIRE(decoder.Initialize(config.GetXmlPath(), msg));

    std::vector<amdisa::InstructionInfoBundle> info;
    REQUIRE(decoder.DecodeShaderDisassemblyText("", info, msg, false) == false);
}

TEST_CASE("Test to decode of a shader disassembly text", "[isa_decoder][decode][text]")
{
    amdisa::IsaDecoder decoder;
    std::string        msg;
    TestConfig&        config = TestConfig::getInstance();
    REQUIRE(decoder.Initialize(config.GetXmlPath(), msg));

    std::vector<amdisa::InstructionInfoBundle> info;
    REQUIRE(decoder.DecodeShaderDisassemblyText("s_cmp_eq_u32  s9, 0 // 000000000000: BF068009", info, msg, false));
    REQUIRE(info[0].bundle[0].instruction_name == "S_CMP_EQ_U32");
}

TEST_CASE("Test that a multi-dword line that fails to decode keeps the previous instruction", "[isa_decoder][decode][text]")
{
    amdisa::IsaDecoder decoder;
    std::string        msg;
    TestConfig&        config = TestConfig::getInstance();
    REQUIRE(decoder.Initialize(config.GetXmlPath(), msg));

    const std::string kText = "s_cmp_eq_u32  s9, 0 // 000000000000: BF068009\n"
                              "unknown // 000000000004: FFFFFFFF FFFFFFFF\n"
                              "s_cmp_eq_u32  s9, 0 // 00000000000C: BF068009";

    std::vector<amdisa::InstructionInfoBundle> info;
    decoder.DecodeShaderDisassemblyText(kText, info, msg, false);
    REQUIRE(info.size() == 3);
    REQUIRE(info[0].bundle[0].instruction_name == "S_CMP_EQ_U32");
    REQUIRE(info[1].bundle[0].instruction_name == "N/A");
    REQUIRE(info[2].bundle[0].instruction_name == "S_CMP_EQ_U32");
}

TEST_CASE("Test to fail decode of a shader disassembly file", "[isa_decoder][decode][file][fail]")
{
    amdisa::IsaDecoder decoder;
    std::string        msg;
    TestConfig&        config = TestConfig::getInstance();
    REQUIRE(decoder.Initialize(config.GetXmlPath(), msg));

    std::vector<amdisa::InstructionInfoBundle> info;
    REQUIRE(decoder.DecodeShaderDisassemblyFile("./invalid_disassembly_file.sp3", info, msg, false) == false);
}
