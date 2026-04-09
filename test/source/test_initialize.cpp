/*
 * Copyright (c) 2025 Advanced Micro Devices, Inc. All rights reserved.
 */
#include "amdisa_tests.h"
#include "amdisa/isa_decoder.h"
#include "amdisa/isa_explorer.h"
#include "catch.hpp"

#include <fstream>
#include <memory>


// isa_decoder
TEST_CASE("Perform isa_decoder initialization with an invalid path", "[isa_decoder][initialize][fail]")
{
    amdisa::IsaDecoder decoder;
    std::string        msg;
    REQUIRE(decoder.Initialize("./invalid_path.xml", msg) == false);
}

TEST_CASE("Perform isa_decoder initialization with an user specified amdisa spec file", "[isa_decoder][initialize]")
{
    amdisa::IsaDecoder decoder;
    std::string        msg;
    TestConfig& config = TestConfig::getInstance();
    REQUIRE(decoder.Initialize(config.GetXmlPath(), msg));
}

// isa_explorer
TEST_CASE("Perform isa_explorer initialization with an invalid path", "[isa_explorer][initialize][fail]")
{
    amdisa::explorer::Spec explorer;
    std::string            msg;
    REQUIRE(explorer.Init("./invalid_spec.xml", msg) == false);
}

TEST_CASE("Perform isa_explorer initialization with an user specified amdisa spec file", "[isa_explorer][initialize]")
{
    amdisa::explorer::Spec explorer;
    std::string            msg;
    TestConfig&            config = TestConfig::getInstance();
    REQUIRE(explorer.Init(config.GetXmlPath(), msg));
}

TEST_CASE("Perform Initialization with a user specified amdisa spec file loaded from memory", "[initialize][membuf][isa_decoder][isa_explorer]")
{
    amdisa::IsaDecoder     decoder;
    amdisa::explorer::Spec explorer;
    std::string            msg;
    TestConfig             &config = TestConfig::getInstance();
    std::ifstream          file(config.GetXmlPath(), std::ios::binary);

    REQUIRE(file.is_open());

    file.seekg(0, std::ios::end);
    std::streampos filesz = file.tellg();
    file.seekg(0, std::ios::beg);

    auto buf = std::make_unique<char[]>(filesz);
    file.read(buf.get(), filesz);
    file.close();

    REQUIRE(decoder.Initialize(buf.get(), filesz, msg));
    REQUIRE(explorer.Init(buf.get(), filesz, msg));
}

TEST_CASE("Perform Initialization with an invalid amdisa spec file loaded from memory", "[initialize][membuf][isa_decoder][isa_explorer][fail]")
{
    amdisa::IsaDecoder     decoder;
    amdisa::explorer::Spec explorer;
    std::string            msg;

    auto buf = std::make_unique<char[]>(256);
    for (size_t i = 0; i < 256; ++i) {
        buf[i] = i;
    }

    REQUIRE(!decoder.Initialize(buf.get(), 256, msg));
    REQUIRE(!explorer.Init(buf.get(), 256, msg));
}
