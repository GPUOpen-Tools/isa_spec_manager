/*
 * Copyright (c) 2025 Advanced Micro Devices, Inc. All rights reserved.
 */
#include "amdisa_tests.h"
#include "amdisa/isa_explorer.h"
#include "catch.hpp"

TEST_CASE("Test for isa_explorer to lookup an instruction", "[isa_explorer][lookup]")
{
    amdisa::explorer::Spec explorer;
    std::string            msg;
    TestConfig&            config = TestConfig::getInstance();
    REQUIRE(explorer.Init(config.GetXmlPath(), msg));
    auto v_mov_b32 = explorer.GetInstructions().at("V_MOV_B32");
    REQUIRE(!(v_mov_b32.Name().compare("V_MOV_B32")));
}
