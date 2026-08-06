/*
 * Copyright (c) 2026 Advanced Micro Devices, Inc. All rights reserved.
 */
#ifndef ENCODING_CONDITION_HANDLER_HPP_

#include <functional>
#include <map>

namespace amdisa {

// Static auxiliary functions.
static bool NotWithin(int x, const std::vector<int>& bucket) {
    for (int v : bucket) {
        if (x == v) {
            return false;
        }
    }
    return true;
}

static bool Within(int x, const std::vector<int>& bucket) {
    for (int v : bucket) {
        if (x == v) {
            return true;
        }
    }
    return false;
}

// Encoding Conditions for Architecture ID: 0
static bool IsArch0Encoding0Condition0(uint64_t dword) {
    return ((dword & 0xff)) != 255;
}

static bool IsArch0Encoding1Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch0Encoding4Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch0Encoding6Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,255,249});
}

static bool IsArch0Encoding7Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,255,249});
}

static bool IsArch0Encoding8Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,255,249});
}

static bool IsArch0Encoding20Condition1(uint64_t dword) {
    return ((dword & 0xff)) == 255;
}

static bool IsArch0Encoding21Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch0Encoding21Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch0Encoding21Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch0Encoding22Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch0Encoding22Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch0Encoding22Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch0Encoding24Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch0Encoding25Condition4(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch0Encoding26Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch0Encoding27Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,255,249});
}

static bool IsArch0Encoding27Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch0Encoding28Condition4(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch0Encoding29Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch0Encoding30Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch0Encoding33Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch0Encoding34Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

// Encoding Conditions for Architecture ID: 1
static bool IsArch1Encoding0Condition0(uint64_t dword) {
    return ((dword & 0xff)) != 255;
}

static bool IsArch1Encoding1Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch1Encoding4Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch1Encoding6Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,255,249});
}

static bool IsArch1Encoding7Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,255,249});
}

static bool IsArch1Encoding8Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,255,249});
}

static bool IsArch1Encoding20Condition1(uint64_t dword) {
    return ((dword & 0xff)) == 255;
}

static bool IsArch1Encoding21Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch1Encoding21Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch1Encoding21Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch1Encoding22Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch1Encoding22Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch1Encoding22Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch1Encoding24Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch1Encoding25Condition4(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch1Encoding26Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch1Encoding27Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,255,249});
}

static bool IsArch1Encoding27Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch1Encoding28Condition4(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch1Encoding29Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch1Encoding30Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch1Encoding33Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch1Encoding34Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

// Encoding Conditions for Architecture ID: 2
static bool IsArch2Encoding0Condition0(uint64_t dword) {
    return ((dword & 0xff)) != 255;
}

static bool IsArch2Encoding1Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch2Encoding4Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch2Encoding6Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,255,249});
}

static bool IsArch2Encoding7Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,255,249});
}

static bool IsArch2Encoding8Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,255,249});
}

static bool IsArch2Encoding20Condition1(uint64_t dword) {
    return ((dword & 0xff)) == 255;
}

static bool IsArch2Encoding21Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch2Encoding21Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch2Encoding21Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch2Encoding22Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch2Encoding22Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch2Encoding22Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch2Encoding24Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch2Encoding25Condition4(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch2Encoding26Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch2Encoding27Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,255,249});
}

static bool IsArch2Encoding27Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch2Encoding28Condition4(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch2Encoding29Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch2Encoding30Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch2Encoding33Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch2Encoding34Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

// Encoding Conditions for Architecture ID: 3
static bool IsArch3Encoding0Condition0(uint64_t dword) {
    return ((dword & 0xff)) != 255;
}

static bool IsArch3Encoding1Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch3Encoding4Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch3Encoding6Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,255,249});
}

static bool IsArch3Encoding7Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,255,249});
}

static bool IsArch3Encoding8Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,255,249});
}

static bool IsArch3Encoding21Condition1(uint64_t dword) {
    return ((dword & 0xff)) == 255;
}

static bool IsArch3Encoding22Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch3Encoding22Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch3Encoding22Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch3Encoding23Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch3Encoding23Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch3Encoding23Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch3Encoding25Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch3Encoding26Condition4(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch3Encoding27Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch3Encoding28Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,255,249});
}

static bool IsArch3Encoding28Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch3Encoding29Condition4(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch3Encoding30Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch3Encoding31Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch3Encoding34Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch3Encoding35Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

// Encoding Conditions for Architecture ID: 5
static bool IsArch5Encoding0Condition0(uint64_t dword) {
    return ((dword & 0xff)) != 255;
}

static bool IsArch5Encoding1Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch5Encoding4Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch5Encoding7Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255,249});
}

static bool IsArch5Encoding8Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255,249});
}

static bool IsArch5Encoding9Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255,249});
}

static bool IsArch5Encoding11Condition0(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch5Encoding11Condition23(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch5Encoding13Condition0(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch5Encoding18Condition0(uint64_t dword) {
    return NotWithin(((dword >> 1) & 0x3),{1,2,3});
}

static bool IsArch5Encoding23Condition7(uint64_t dword) {
    return ((dword >> 1) & 0x3) == 1;
}

static bool IsArch5Encoding24Condition8(uint64_t dword) {
    return ((dword >> 1) & 0x3) == 2;
}

static bool IsArch5Encoding25Condition9(uint64_t dword) {
    return ((dword >> 1) & 0x3) == 3;
}

static bool IsArch5Encoding26Condition1(uint64_t dword) {
    return ((dword & 0xff)) == 255;
}

static bool IsArch5Encoding27Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch5Encoding27Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch5Encoding27Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch5Encoding28Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch5Encoding28Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch5Encoding28Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch5Encoding30Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch5Encoding30Condition2(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch5Encoding30Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch5Encoding30Condition10(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch5Encoding30Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch5Encoding30Condition12(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch5Encoding30Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch5Encoding31Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch5Encoding32Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch5Encoding33Condition15(uint64_t dword) {
    return Within(((dword & 0x1ff)),{233,234});
}

static bool IsArch5Encoding34Condition14(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch5Encoding35Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255,249});
}

static bool IsArch5Encoding35Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch5Encoding36Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch5Encoding37Condition15(uint64_t dword) {
    return Within(((dword & 0x1ff)),{233,234});
}

static bool IsArch5Encoding38Condition14(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch5Encoding39Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch5Encoding40Condition0(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch5Encoding41Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch5Encoding41Condition2(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch5Encoding41Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch5Encoding41Condition10(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch5Encoding41Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch5Encoding41Condition12(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch5Encoding41Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch5Encoding42Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch5Encoding42Condition2(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch5Encoding42Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch5Encoding42Condition10(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch5Encoding42Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch5Encoding42Condition12(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch5Encoding42Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch5Encoding43Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch5Encoding44Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

// Encoding Conditions for Architecture ID: 6
static bool IsArch6Encoding0Condition0(uint64_t dword) {
    return ((dword & 0xff)) != 255;
}

static bool IsArch6Encoding1Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch6Encoding4Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch6Encoding7Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255,249});
}

static bool IsArch6Encoding8Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255,249});
}

static bool IsArch6Encoding9Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255,249});
}

static bool IsArch6Encoding11Condition0(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch6Encoding11Condition23(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch6Encoding13Condition0(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch6Encoding17Condition0(uint64_t dword) {
    return NotWithin(((dword >> 1) & 0x3),{1,2,3});
}

static bool IsArch6Encoding22Condition7(uint64_t dword) {
    return ((dword >> 1) & 0x3) == 1;
}

static bool IsArch6Encoding23Condition8(uint64_t dword) {
    return ((dword >> 1) & 0x3) == 2;
}

static bool IsArch6Encoding24Condition9(uint64_t dword) {
    return ((dword >> 1) & 0x3) == 3;
}

static bool IsArch6Encoding25Condition1(uint64_t dword) {
    return ((dword & 0xff)) == 255;
}

static bool IsArch6Encoding26Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch6Encoding26Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch6Encoding26Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch6Encoding27Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch6Encoding27Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch6Encoding27Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch6Encoding29Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch6Encoding29Condition2(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch6Encoding29Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch6Encoding29Condition10(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch6Encoding29Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch6Encoding29Condition12(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch6Encoding29Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch6Encoding30Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch6Encoding31Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch6Encoding32Condition15(uint64_t dword) {
    return Within(((dword & 0x1ff)),{233,234});
}

static bool IsArch6Encoding33Condition14(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch6Encoding34Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255,249});
}

static bool IsArch6Encoding34Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch6Encoding35Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch6Encoding36Condition15(uint64_t dword) {
    return Within(((dword & 0x1ff)),{233,234});
}

static bool IsArch6Encoding37Condition14(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch6Encoding38Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

static bool IsArch6Encoding39Condition0(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch6Encoding40Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch6Encoding40Condition2(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch6Encoding40Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch6Encoding40Condition10(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch6Encoding40Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch6Encoding40Condition12(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch6Encoding40Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch6Encoding41Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch6Encoding41Condition2(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch6Encoding41Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch6Encoding41Condition10(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch6Encoding41Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch6Encoding41Condition12(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) != 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch6Encoding41Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch6Encoding42Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch6Encoding43Condition6(uint64_t dword) {
    return ((dword & 0x1ff)) == 249;
}

// Encoding Conditions for Architecture ID: 8
static bool IsArch8Encoding0Condition17(uint64_t dword) {
    return ((dword & 0xff)) != 255;
}

static bool IsArch8Encoding1Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch8Encoding4Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch8Encoding6Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255});
}

static bool IsArch8Encoding7Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255});
}

static bool IsArch8Encoding8Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255});
}

static bool IsArch8Encoding9Condition0(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding10Condition0(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding19Condition0(uint64_t dword) {
    return ((dword & 0x1)) != 1;
}

static bool IsArch8Encoding24Condition16(uint64_t dword) {
    return ((dword & 0x1)) == 1;
}

static bool IsArch8Encoding25Condition1(uint64_t dword) {
    return ((dword & 0xff)) == 255;
}

static bool IsArch8Encoding26Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch8Encoding26Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch8Encoding26Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch8Encoding27Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch8Encoding27Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch8Encoding27Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch8Encoding29Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch8Encoding30Condition15(uint64_t dword) {
    return Within(((dword & 0x1ff)),{233,234});
}

static bool IsArch8Encoding31Condition14(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch8Encoding32Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding32Condition2(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding32Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding32Condition10(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch8Encoding32Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch8Encoding32Condition12(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch8Encoding32Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch8Encoding33Condition15(uint64_t dword) {
    return ((Within(((dword >> 32) & 0x1ff),{233,234})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding34Condition14(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 250) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding35Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255});
}

static bool IsArch8Encoding35Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch8Encoding36Condition15(uint64_t dword) {
    return Within(((dword & 0x1ff)),{233,234});
}

static bool IsArch8Encoding37Condition14(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch8Encoding38Condition0(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding39Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding39Condition2(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding39Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding39Condition10(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch8Encoding39Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch8Encoding39Condition12(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch8Encoding39Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch8Encoding40Condition15(uint64_t dword) {
    return ((Within(((dword >> 32) & 0x1ff),{233,234})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding41Condition14(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 250) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding42Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding42Condition2(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding42Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding42Condition10(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch8Encoding42Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch8Encoding42Condition12(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch8Encoding42Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch8Encoding43Condition15(uint64_t dword) {
    return ((Within(((dword >> 32) & 0x1ff),{233,234})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding44Condition14(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 250) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch8Encoding45Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch8Encoding46Condition15(uint64_t dword) {
    return Within(((dword & 0x1ff)),{233,234});
}

static bool IsArch8Encoding47Condition14(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch8Encoding48Condition0(uint64_t dword) {
    return ((dword & 0x1ff)) != 255;
}

static bool IsArch8Encoding49Condition0(uint64_t dword) {
    return ((dword & 0x1ff)) != 255;
}

static bool IsArch8Encoding49Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch8Encoding50Condition0(uint64_t dword) {
    return ((dword >> 32) & 0x1ff) != 255;
}

static bool IsArch8Encoding51Condition0(uint64_t dword) {
    return ((dword >> 32) & 0x1ff) != 255;
}

static bool IsArch8Encoding51Condition5(uint64_t dword) {
    return ((dword >> 32) & 0x1ff) == 255;
}

// Encoding Conditions for Architecture ID: 9
static bool IsArch9Encoding0Condition17(uint64_t dword) {
    return ((dword & 0xff)) != 255;
}

static bool IsArch9Encoding1Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch9Encoding4Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch9Encoding6Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255});
}

static bool IsArch9Encoding7Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255});
}

static bool IsArch9Encoding8Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255});
}

static bool IsArch9Encoding9Condition0(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding10Condition0(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding19Condition0(uint64_t dword) {
    return ((dword & 0x1)) != 1;
}

static bool IsArch9Encoding24Condition16(uint64_t dword) {
    return ((dword & 0x1)) == 1;
}

static bool IsArch9Encoding25Condition1(uint64_t dword) {
    return ((dword & 0xff)) == 255;
}

static bool IsArch9Encoding26Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch9Encoding26Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch9Encoding26Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch9Encoding26Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch9Encoding27Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch9Encoding27Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch9Encoding27Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch9Encoding29Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch9Encoding30Condition15(uint64_t dword) {
    return Within(((dword & 0x1ff)),{233,234});
}

static bool IsArch9Encoding31Condition14(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch9Encoding32Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding32Condition2(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding32Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding32Condition10(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch9Encoding32Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch9Encoding32Condition12(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch9Encoding32Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch9Encoding33Condition15(uint64_t dword) {
    return ((Within(((dword >> 32) & 0x1ff),{233,234})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding34Condition14(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 250) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding35Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255});
}

static bool IsArch9Encoding35Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch9Encoding36Condition15(uint64_t dword) {
    return Within(((dword & 0x1ff)),{233,234});
}

static bool IsArch9Encoding37Condition14(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch9Encoding38Condition0(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding39Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding39Condition2(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding39Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding39Condition10(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch9Encoding39Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch9Encoding39Condition12(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch9Encoding39Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch9Encoding40Condition15(uint64_t dword) {
    return ((Within(((dword >> 32) & 0x1ff),{233,234})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding41Condition14(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 250) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding42Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding42Condition2(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding42Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding42Condition10(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch9Encoding42Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch9Encoding42Condition12(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch9Encoding42Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch9Encoding43Condition15(uint64_t dword) {
    return ((Within(((dword >> 32) & 0x1ff),{233,234})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding44Condition14(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 250) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch9Encoding45Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch9Encoding46Condition15(uint64_t dword) {
    return Within(((dword & 0x1ff)),{233,234});
}

static bool IsArch9Encoding47Condition14(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch9Encoding48Condition0(uint64_t dword) {
    return ((dword & 0x1ff)) != 255;
}

static bool IsArch9Encoding49Condition0(uint64_t dword) {
    return ((dword & 0x1ff)) != 255;
}

static bool IsArch9Encoding49Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch9Encoding50Condition0(uint64_t dword) {
    return ((dword >> 32) & 0x1ff) != 255;
}

static bool IsArch9Encoding51Condition0(uint64_t dword) {
    return ((dword >> 32) & 0x1ff) != 255;
}

static bool IsArch9Encoding51Condition5(uint64_t dword) {
    return ((dword >> 32) & 0x1ff) == 255;
}

// Encoding Conditions for Architecture ID: 10
static bool IsArch10Encoding0Condition17(uint64_t dword) {
    return ((dword & 0xff)) != 255;
}

static bool IsArch10Encoding1Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch10Encoding4Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch10Encoding6Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255});
}

static bool IsArch10Encoding7Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255});
}

static bool IsArch10Encoding8Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255});
}

static bool IsArch10Encoding9Condition0(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding10Condition0(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding24Condition1(uint64_t dword) {
    return ((dword & 0xff)) == 255;
}

static bool IsArch10Encoding25Condition0(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch10Encoding25Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch10Encoding25Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch10Encoding25Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch10Encoding26Condition1(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch10Encoding26Condition2(uint64_t dword) {
    return (((dword & 0xff)) != 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch10Encoding26Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch10Encoding28Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch10Encoding29Condition15(uint64_t dword) {
    return Within(((dword & 0x1ff)),{233,234});
}

static bool IsArch10Encoding30Condition14(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch10Encoding31Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding31Condition2(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding31Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding31Condition10(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch10Encoding31Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch10Encoding31Condition12(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch10Encoding31Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch10Encoding32Condition15(uint64_t dword) {
    return ((Within(((dword >> 32) & 0x1ff),{233,234})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding33Condition14(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 250) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding34Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255});
}

static bool IsArch10Encoding34Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch10Encoding35Condition15(uint64_t dword) {
    return Within(((dword & 0x1ff)),{233,234});
}

static bool IsArch10Encoding36Condition14(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch10Encoding37Condition0(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding38Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding38Condition2(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding38Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding38Condition10(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch10Encoding38Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch10Encoding38Condition12(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch10Encoding38Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch10Encoding39Condition15(uint64_t dword) {
    return ((Within(((dword >> 32) & 0x1ff),{233,234})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding40Condition14(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 250) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding41Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding41Condition2(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding41Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding41Condition10(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch10Encoding41Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch10Encoding41Condition12(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch10Encoding41Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch10Encoding42Condition15(uint64_t dword) {
    return ((Within(((dword >> 32) & 0x1ff),{233,234})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding43Condition14(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 250) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch10Encoding44Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch10Encoding45Condition15(uint64_t dword) {
    return Within(((dword & 0x1ff)),{233,234});
}

static bool IsArch10Encoding46Condition14(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch10Encoding47Condition0(uint64_t dword) {
    return ((dword & 0x1ff)) != 255;
}

static bool IsArch10Encoding48Condition0(uint64_t dword) {
    return ((dword & 0x1ff)) != 255;
}

static bool IsArch10Encoding48Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch10Encoding49Condition0(uint64_t dword) {
    return ((dword >> 32) & 0x1ff) != 255;
}

static bool IsArch10Encoding50Condition0(uint64_t dword) {
    return ((dword >> 32) & 0x1ff) != 255;
}

static bool IsArch10Encoding50Condition5(uint64_t dword) {
    return ((dword >> 32) & 0x1ff) == 255;
}

// Encoding Conditions for Architecture ID: 4
static bool IsArch4Encoding0Condition18(uint64_t dword) {
    return NotWithin(((dword & 0xff)),{254,255});
}

static bool IsArch4Encoding1Condition0(uint64_t dword) {
    return (((((dword & 0xff)) != 254) && (((dword >> 8) & 0xff) != 254)) && (((dword & 0xff)) != 255)) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch4Encoding4Condition0(uint64_t dword) {
    return (((((dword & 0xff)) != 254) && (((dword >> 8) & 0xff) != 254)) && (((dword & 0xff)) != 255)) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch4Encoding6Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255,254});
}

static bool IsArch4Encoding7Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255,254});
}

static bool IsArch4Encoding8Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255,254});
}

static bool IsArch4Encoding9Condition0(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding12Condition0(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding29Condition1(uint64_t dword) {
    return ((dword & 0xff)) == 255;
}

static bool IsArch4Encoding30Condition19(uint64_t dword) {
    return ((dword & 0xff)) == 254;
}

static bool IsArch4Encoding31Condition0(uint64_t dword) {
    return (((((dword & 0xff)) != 254) && (((dword >> 8) & 0xff) != 254)) && (((dword & 0xff)) != 255)) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch4Encoding31Condition1(uint64_t dword) {
    return ((((dword >> 8) & 0xff) != 254) && (((dword & 0xff)) == 255)) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch4Encoding31Condition2(uint64_t dword) {
    return (NotWithin(((dword & 0xff)),{254,255})) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch4Encoding31Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch4Encoding32Condition19(uint64_t dword) {
    return (((((dword & 0xff)) == 254) && (((dword >> 8) & 0xff) != 254)) && (((dword & 0xff)) != 255)) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch4Encoding32Condition20(uint64_t dword) {
    return (((((dword & 0xff)) != 254) && (((dword >> 8) & 0xff) == 254)) && (((dword & 0xff)) != 255)) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch4Encoding32Condition21(uint64_t dword) {
    return (((((dword & 0xff)) == 254) && (((dword >> 8) & 0xff) == 254)) && (((dword & 0xff)) != 255)) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch4Encoding33Condition1(uint64_t dword) {
    return ((((dword >> 8) & 0xff) != 254) && (((dword & 0xff)) == 255)) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch4Encoding33Condition2(uint64_t dword) {
    return (NotWithin(((dword & 0xff)),{254,255})) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch4Encoding33Condition3(uint64_t dword) {
    return (((dword & 0xff)) == 255) && (((dword >> 8) & 0xff) == 255);
}

static bool IsArch4Encoding34Condition19(uint64_t dword) {
    return (((((dword & 0xff)) == 254) && (((dword >> 8) & 0xff) != 254)) && (((dword & 0xff)) != 255)) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch4Encoding34Condition20(uint64_t dword) {
    return (((((dword & 0xff)) != 254) && (((dword >> 8) & 0xff) == 254)) && (((dword & 0xff)) != 255)) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch4Encoding34Condition21(uint64_t dword) {
    return (((((dword & 0xff)) == 254) && (((dword >> 8) & 0xff) == 254)) && (((dword & 0xff)) != 255)) && (((dword >> 8) & 0xff) != 255);
}

static bool IsArch4Encoding36Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch4Encoding37Condition22(uint64_t dword) {
    return ((dword & 0x1ff)) == 254;
}

static bool IsArch4Encoding38Condition15(uint64_t dword) {
    return Within(((dword & 0x1ff)),{233,234});
}

static bool IsArch4Encoding39Condition14(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch4Encoding40Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding40Condition2(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding40Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding40Condition10(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch4Encoding40Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch4Encoding40Condition12(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch4Encoding40Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch4Encoding41Condition15(uint64_t dword) {
    return ((Within(((dword >> 32) & 0x1ff),{233,234})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding42Condition14(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 250) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding43Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255,254});
}

static bool IsArch4Encoding43Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch4Encoding44Condition0(uint64_t dword) {
    return NotWithin(((dword & 0x1ff)),{250,233,234,255,254});
}

static bool IsArch4Encoding44Condition22(uint64_t dword) {
    return ((dword & 0x1ff)) == 254;
}

static bool IsArch4Encoding45Condition15(uint64_t dword) {
    return Within(((dword & 0x1ff)),{233,234});
}

static bool IsArch4Encoding46Condition14(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch4Encoding47Condition0(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding48Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding48Condition2(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding48Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding48Condition10(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch4Encoding48Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch4Encoding48Condition12(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch4Encoding48Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch4Encoding49Condition15(uint64_t dword) {
    return ((Within(((dword >> 32) & 0x1ff),{233,234})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding50Condition14(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 250) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding51Condition1(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding51Condition2(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding51Condition3(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding51Condition10(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch4Encoding51Condition11(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch4Encoding51Condition12(uint64_t dword) {
    return ((NotWithin(((dword >> 32) & 0x1ff),{250,233,234,255})) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch4Encoding51Condition13(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 255) && (((dword >> 41) & 0x1ff) == 255)) && (((dword >> 50) & 0x1ff) == 255);
}

static bool IsArch4Encoding52Condition15(uint64_t dword) {
    return ((Within(((dword >> 32) & 0x1ff),{233,234})) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding53Condition14(uint64_t dword) {
    return ((((dword >> 32) & 0x1ff) == 250) && (((dword >> 41) & 0x1ff) != 255)) && (((dword >> 50) & 0x1ff) != 255);
}

static bool IsArch4Encoding54Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch4Encoding55Condition22(uint64_t dword) {
    return ((dword & 0x1ff)) == 254;
}

static bool IsArch4Encoding56Condition15(uint64_t dword) {
    return Within(((dword & 0x1ff)),{233,234});
}

static bool IsArch4Encoding57Condition14(uint64_t dword) {
    return ((dword & 0x1ff)) == 250;
}

static bool IsArch4Encoding59Condition0(uint64_t dword) {
    return ((dword & 0x1ff)) != 255;
}

static bool IsArch4Encoding60Condition0(uint64_t dword) {
    return ((dword & 0x1ff)) != 255;
}

static bool IsArch4Encoding60Condition5(uint64_t dword) {
    return ((dword & 0x1ff)) == 255;
}

static bool IsArch4Encoding61Condition0(uint64_t dword) {
    return ((dword >> 32) & 0x1ff) != 255;
}

static bool IsArch4Encoding62Condition0(uint64_t dword) {
    return ((dword >> 32) & 0x1ff) != 255;
}

static bool IsArch4Encoding62Condition5(uint64_t dword) {
    return ((dword >> 32) & 0x1ff) == 255;
}

// Encoding condition handler struct. This struct is used when decoding instructions.
struct EncodingConditionHandler {
    EncodingConditionHandler() {
        arch_conditions_[0][0][0] = IsArch0Encoding0Condition0;
        arch_conditions_[0][1][0] = IsArch0Encoding1Condition0;
        arch_conditions_[0][4][0] = IsArch0Encoding4Condition0;
        arch_conditions_[0][6][0] = IsArch0Encoding6Condition0;
        arch_conditions_[0][7][0] = IsArch0Encoding7Condition0;
        arch_conditions_[0][8][0] = IsArch0Encoding8Condition0;
        arch_conditions_[0][20][1] = IsArch0Encoding20Condition1;
        arch_conditions_[0][21][1] = IsArch0Encoding21Condition1;
        arch_conditions_[0][21][2] = IsArch0Encoding21Condition2;
        arch_conditions_[0][21][3] = IsArch0Encoding21Condition3;
        arch_conditions_[0][22][1] = IsArch0Encoding22Condition1;
        arch_conditions_[0][22][2] = IsArch0Encoding22Condition2;
        arch_conditions_[0][22][3] = IsArch0Encoding22Condition3;
        arch_conditions_[0][24][5] = IsArch0Encoding24Condition5;
        arch_conditions_[0][25][4] = IsArch0Encoding25Condition4;
        arch_conditions_[0][26][6] = IsArch0Encoding26Condition6;
        arch_conditions_[0][27][0] = IsArch0Encoding27Condition0;
        arch_conditions_[0][27][5] = IsArch0Encoding27Condition5;
        arch_conditions_[0][28][4] = IsArch0Encoding28Condition4;
        arch_conditions_[0][29][6] = IsArch0Encoding29Condition6;
        arch_conditions_[0][30][6] = IsArch0Encoding30Condition6;
        arch_conditions_[0][33][5] = IsArch0Encoding33Condition5;
        arch_conditions_[0][34][6] = IsArch0Encoding34Condition6;
        arch_conditions_[1][0][0] = IsArch1Encoding0Condition0;
        arch_conditions_[1][1][0] = IsArch1Encoding1Condition0;
        arch_conditions_[1][4][0] = IsArch1Encoding4Condition0;
        arch_conditions_[1][6][0] = IsArch1Encoding6Condition0;
        arch_conditions_[1][7][0] = IsArch1Encoding7Condition0;
        arch_conditions_[1][8][0] = IsArch1Encoding8Condition0;
        arch_conditions_[1][20][1] = IsArch1Encoding20Condition1;
        arch_conditions_[1][21][1] = IsArch1Encoding21Condition1;
        arch_conditions_[1][21][2] = IsArch1Encoding21Condition2;
        arch_conditions_[1][21][3] = IsArch1Encoding21Condition3;
        arch_conditions_[1][22][1] = IsArch1Encoding22Condition1;
        arch_conditions_[1][22][2] = IsArch1Encoding22Condition2;
        arch_conditions_[1][22][3] = IsArch1Encoding22Condition3;
        arch_conditions_[1][24][5] = IsArch1Encoding24Condition5;
        arch_conditions_[1][25][4] = IsArch1Encoding25Condition4;
        arch_conditions_[1][26][6] = IsArch1Encoding26Condition6;
        arch_conditions_[1][27][0] = IsArch1Encoding27Condition0;
        arch_conditions_[1][27][5] = IsArch1Encoding27Condition5;
        arch_conditions_[1][28][4] = IsArch1Encoding28Condition4;
        arch_conditions_[1][29][6] = IsArch1Encoding29Condition6;
        arch_conditions_[1][30][6] = IsArch1Encoding30Condition6;
        arch_conditions_[1][33][5] = IsArch1Encoding33Condition5;
        arch_conditions_[1][34][6] = IsArch1Encoding34Condition6;
        arch_conditions_[2][0][0] = IsArch2Encoding0Condition0;
        arch_conditions_[2][1][0] = IsArch2Encoding1Condition0;
        arch_conditions_[2][4][0] = IsArch2Encoding4Condition0;
        arch_conditions_[2][6][0] = IsArch2Encoding6Condition0;
        arch_conditions_[2][7][0] = IsArch2Encoding7Condition0;
        arch_conditions_[2][8][0] = IsArch2Encoding8Condition0;
        arch_conditions_[2][20][1] = IsArch2Encoding20Condition1;
        arch_conditions_[2][21][1] = IsArch2Encoding21Condition1;
        arch_conditions_[2][21][2] = IsArch2Encoding21Condition2;
        arch_conditions_[2][21][3] = IsArch2Encoding21Condition3;
        arch_conditions_[2][22][1] = IsArch2Encoding22Condition1;
        arch_conditions_[2][22][2] = IsArch2Encoding22Condition2;
        arch_conditions_[2][22][3] = IsArch2Encoding22Condition3;
        arch_conditions_[2][24][5] = IsArch2Encoding24Condition5;
        arch_conditions_[2][25][4] = IsArch2Encoding25Condition4;
        arch_conditions_[2][26][6] = IsArch2Encoding26Condition6;
        arch_conditions_[2][27][0] = IsArch2Encoding27Condition0;
        arch_conditions_[2][27][5] = IsArch2Encoding27Condition5;
        arch_conditions_[2][28][4] = IsArch2Encoding28Condition4;
        arch_conditions_[2][29][6] = IsArch2Encoding29Condition6;
        arch_conditions_[2][30][6] = IsArch2Encoding30Condition6;
        arch_conditions_[2][33][5] = IsArch2Encoding33Condition5;
        arch_conditions_[2][34][6] = IsArch2Encoding34Condition6;
        arch_conditions_[3][0][0] = IsArch3Encoding0Condition0;
        arch_conditions_[3][1][0] = IsArch3Encoding1Condition0;
        arch_conditions_[3][4][0] = IsArch3Encoding4Condition0;
        arch_conditions_[3][6][0] = IsArch3Encoding6Condition0;
        arch_conditions_[3][7][0] = IsArch3Encoding7Condition0;
        arch_conditions_[3][8][0] = IsArch3Encoding8Condition0;
        arch_conditions_[3][21][1] = IsArch3Encoding21Condition1;
        arch_conditions_[3][22][1] = IsArch3Encoding22Condition1;
        arch_conditions_[3][22][2] = IsArch3Encoding22Condition2;
        arch_conditions_[3][22][3] = IsArch3Encoding22Condition3;
        arch_conditions_[3][23][1] = IsArch3Encoding23Condition1;
        arch_conditions_[3][23][2] = IsArch3Encoding23Condition2;
        arch_conditions_[3][23][3] = IsArch3Encoding23Condition3;
        arch_conditions_[3][25][5] = IsArch3Encoding25Condition5;
        arch_conditions_[3][26][4] = IsArch3Encoding26Condition4;
        arch_conditions_[3][27][6] = IsArch3Encoding27Condition6;
        arch_conditions_[3][28][0] = IsArch3Encoding28Condition0;
        arch_conditions_[3][28][5] = IsArch3Encoding28Condition5;
        arch_conditions_[3][29][4] = IsArch3Encoding29Condition4;
        arch_conditions_[3][30][6] = IsArch3Encoding30Condition6;
        arch_conditions_[3][31][6] = IsArch3Encoding31Condition6;
        arch_conditions_[3][34][5] = IsArch3Encoding34Condition5;
        arch_conditions_[3][35][6] = IsArch3Encoding35Condition6;
        arch_conditions_[5][0][0] = IsArch5Encoding0Condition0;
        arch_conditions_[5][1][0] = IsArch5Encoding1Condition0;
        arch_conditions_[5][4][0] = IsArch5Encoding4Condition0;
        arch_conditions_[5][7][0] = IsArch5Encoding7Condition0;
        arch_conditions_[5][8][0] = IsArch5Encoding8Condition0;
        arch_conditions_[5][9][0] = IsArch5Encoding9Condition0;
        arch_conditions_[5][11][0] = IsArch5Encoding11Condition0;
        arch_conditions_[5][11][23] = IsArch5Encoding11Condition23;
        arch_conditions_[5][13][0] = IsArch5Encoding13Condition0;
        arch_conditions_[5][18][0] = IsArch5Encoding18Condition0;
        arch_conditions_[5][23][7] = IsArch5Encoding23Condition7;
        arch_conditions_[5][24][8] = IsArch5Encoding24Condition8;
        arch_conditions_[5][25][9] = IsArch5Encoding25Condition9;
        arch_conditions_[5][26][1] = IsArch5Encoding26Condition1;
        arch_conditions_[5][27][1] = IsArch5Encoding27Condition1;
        arch_conditions_[5][27][2] = IsArch5Encoding27Condition2;
        arch_conditions_[5][27][3] = IsArch5Encoding27Condition3;
        arch_conditions_[5][28][1] = IsArch5Encoding28Condition1;
        arch_conditions_[5][28][2] = IsArch5Encoding28Condition2;
        arch_conditions_[5][28][3] = IsArch5Encoding28Condition3;
        arch_conditions_[5][30][1] = IsArch5Encoding30Condition1;
        arch_conditions_[5][30][2] = IsArch5Encoding30Condition2;
        arch_conditions_[5][30][3] = IsArch5Encoding30Condition3;
        arch_conditions_[5][30][10] = IsArch5Encoding30Condition10;
        arch_conditions_[5][30][11] = IsArch5Encoding30Condition11;
        arch_conditions_[5][30][12] = IsArch5Encoding30Condition12;
        arch_conditions_[5][30][13] = IsArch5Encoding30Condition13;
        arch_conditions_[5][31][5] = IsArch5Encoding31Condition5;
        arch_conditions_[5][32][6] = IsArch5Encoding32Condition6;
        arch_conditions_[5][33][15] = IsArch5Encoding33Condition15;
        arch_conditions_[5][34][14] = IsArch5Encoding34Condition14;
        arch_conditions_[5][35][0] = IsArch5Encoding35Condition0;
        arch_conditions_[5][35][5] = IsArch5Encoding35Condition5;
        arch_conditions_[5][36][6] = IsArch5Encoding36Condition6;
        arch_conditions_[5][37][15] = IsArch5Encoding37Condition15;
        arch_conditions_[5][38][14] = IsArch5Encoding38Condition14;
        arch_conditions_[5][39][6] = IsArch5Encoding39Condition6;
        arch_conditions_[5][40][0] = IsArch5Encoding40Condition0;
        arch_conditions_[5][41][1] = IsArch5Encoding41Condition1;
        arch_conditions_[5][41][2] = IsArch5Encoding41Condition2;
        arch_conditions_[5][41][3] = IsArch5Encoding41Condition3;
        arch_conditions_[5][41][10] = IsArch5Encoding41Condition10;
        arch_conditions_[5][41][11] = IsArch5Encoding41Condition11;
        arch_conditions_[5][41][12] = IsArch5Encoding41Condition12;
        arch_conditions_[5][41][13] = IsArch5Encoding41Condition13;
        arch_conditions_[5][42][1] = IsArch5Encoding42Condition1;
        arch_conditions_[5][42][2] = IsArch5Encoding42Condition2;
        arch_conditions_[5][42][3] = IsArch5Encoding42Condition3;
        arch_conditions_[5][42][10] = IsArch5Encoding42Condition10;
        arch_conditions_[5][42][11] = IsArch5Encoding42Condition11;
        arch_conditions_[5][42][12] = IsArch5Encoding42Condition12;
        arch_conditions_[5][42][13] = IsArch5Encoding42Condition13;
        arch_conditions_[5][43][5] = IsArch5Encoding43Condition5;
        arch_conditions_[5][44][6] = IsArch5Encoding44Condition6;
        arch_conditions_[6][0][0] = IsArch6Encoding0Condition0;
        arch_conditions_[6][1][0] = IsArch6Encoding1Condition0;
        arch_conditions_[6][4][0] = IsArch6Encoding4Condition0;
        arch_conditions_[6][7][0] = IsArch6Encoding7Condition0;
        arch_conditions_[6][8][0] = IsArch6Encoding8Condition0;
        arch_conditions_[6][9][0] = IsArch6Encoding9Condition0;
        arch_conditions_[6][11][0] = IsArch6Encoding11Condition0;
        arch_conditions_[6][11][23] = IsArch6Encoding11Condition23;
        arch_conditions_[6][13][0] = IsArch6Encoding13Condition0;
        arch_conditions_[6][17][0] = IsArch6Encoding17Condition0;
        arch_conditions_[6][22][7] = IsArch6Encoding22Condition7;
        arch_conditions_[6][23][8] = IsArch6Encoding23Condition8;
        arch_conditions_[6][24][9] = IsArch6Encoding24Condition9;
        arch_conditions_[6][25][1] = IsArch6Encoding25Condition1;
        arch_conditions_[6][26][1] = IsArch6Encoding26Condition1;
        arch_conditions_[6][26][2] = IsArch6Encoding26Condition2;
        arch_conditions_[6][26][3] = IsArch6Encoding26Condition3;
        arch_conditions_[6][27][1] = IsArch6Encoding27Condition1;
        arch_conditions_[6][27][2] = IsArch6Encoding27Condition2;
        arch_conditions_[6][27][3] = IsArch6Encoding27Condition3;
        arch_conditions_[6][29][1] = IsArch6Encoding29Condition1;
        arch_conditions_[6][29][2] = IsArch6Encoding29Condition2;
        arch_conditions_[6][29][3] = IsArch6Encoding29Condition3;
        arch_conditions_[6][29][10] = IsArch6Encoding29Condition10;
        arch_conditions_[6][29][11] = IsArch6Encoding29Condition11;
        arch_conditions_[6][29][12] = IsArch6Encoding29Condition12;
        arch_conditions_[6][29][13] = IsArch6Encoding29Condition13;
        arch_conditions_[6][30][5] = IsArch6Encoding30Condition5;
        arch_conditions_[6][31][6] = IsArch6Encoding31Condition6;
        arch_conditions_[6][32][15] = IsArch6Encoding32Condition15;
        arch_conditions_[6][33][14] = IsArch6Encoding33Condition14;
        arch_conditions_[6][34][0] = IsArch6Encoding34Condition0;
        arch_conditions_[6][34][5] = IsArch6Encoding34Condition5;
        arch_conditions_[6][35][6] = IsArch6Encoding35Condition6;
        arch_conditions_[6][36][15] = IsArch6Encoding36Condition15;
        arch_conditions_[6][37][14] = IsArch6Encoding37Condition14;
        arch_conditions_[6][38][6] = IsArch6Encoding38Condition6;
        arch_conditions_[6][39][0] = IsArch6Encoding39Condition0;
        arch_conditions_[6][40][1] = IsArch6Encoding40Condition1;
        arch_conditions_[6][40][2] = IsArch6Encoding40Condition2;
        arch_conditions_[6][40][3] = IsArch6Encoding40Condition3;
        arch_conditions_[6][40][10] = IsArch6Encoding40Condition10;
        arch_conditions_[6][40][11] = IsArch6Encoding40Condition11;
        arch_conditions_[6][40][12] = IsArch6Encoding40Condition12;
        arch_conditions_[6][40][13] = IsArch6Encoding40Condition13;
        arch_conditions_[6][41][1] = IsArch6Encoding41Condition1;
        arch_conditions_[6][41][2] = IsArch6Encoding41Condition2;
        arch_conditions_[6][41][3] = IsArch6Encoding41Condition3;
        arch_conditions_[6][41][10] = IsArch6Encoding41Condition10;
        arch_conditions_[6][41][11] = IsArch6Encoding41Condition11;
        arch_conditions_[6][41][12] = IsArch6Encoding41Condition12;
        arch_conditions_[6][41][13] = IsArch6Encoding41Condition13;
        arch_conditions_[6][42][5] = IsArch6Encoding42Condition5;
        arch_conditions_[6][43][6] = IsArch6Encoding43Condition6;
        arch_conditions_[8][0][17] = IsArch8Encoding0Condition17;
        arch_conditions_[8][1][0] = IsArch8Encoding1Condition0;
        arch_conditions_[8][4][0] = IsArch8Encoding4Condition0;
        arch_conditions_[8][6][0] = IsArch8Encoding6Condition0;
        arch_conditions_[8][7][0] = IsArch8Encoding7Condition0;
        arch_conditions_[8][8][0] = IsArch8Encoding8Condition0;
        arch_conditions_[8][9][0] = IsArch8Encoding9Condition0;
        arch_conditions_[8][10][0] = IsArch8Encoding10Condition0;
        arch_conditions_[8][19][0] = IsArch8Encoding19Condition0;
        arch_conditions_[8][24][16] = IsArch8Encoding24Condition16;
        arch_conditions_[8][25][1] = IsArch8Encoding25Condition1;
        arch_conditions_[8][26][1] = IsArch8Encoding26Condition1;
        arch_conditions_[8][26][2] = IsArch8Encoding26Condition2;
        arch_conditions_[8][26][3] = IsArch8Encoding26Condition3;
        arch_conditions_[8][27][1] = IsArch8Encoding27Condition1;
        arch_conditions_[8][27][2] = IsArch8Encoding27Condition2;
        arch_conditions_[8][27][3] = IsArch8Encoding27Condition3;
        arch_conditions_[8][29][5] = IsArch8Encoding29Condition5;
        arch_conditions_[8][30][15] = IsArch8Encoding30Condition15;
        arch_conditions_[8][31][14] = IsArch8Encoding31Condition14;
        arch_conditions_[8][32][1] = IsArch8Encoding32Condition1;
        arch_conditions_[8][32][2] = IsArch8Encoding32Condition2;
        arch_conditions_[8][32][3] = IsArch8Encoding32Condition3;
        arch_conditions_[8][32][10] = IsArch8Encoding32Condition10;
        arch_conditions_[8][32][11] = IsArch8Encoding32Condition11;
        arch_conditions_[8][32][12] = IsArch8Encoding32Condition12;
        arch_conditions_[8][32][13] = IsArch8Encoding32Condition13;
        arch_conditions_[8][33][15] = IsArch8Encoding33Condition15;
        arch_conditions_[8][34][14] = IsArch8Encoding34Condition14;
        arch_conditions_[8][35][0] = IsArch8Encoding35Condition0;
        arch_conditions_[8][35][5] = IsArch8Encoding35Condition5;
        arch_conditions_[8][36][15] = IsArch8Encoding36Condition15;
        arch_conditions_[8][37][14] = IsArch8Encoding37Condition14;
        arch_conditions_[8][38][0] = IsArch8Encoding38Condition0;
        arch_conditions_[8][39][1] = IsArch8Encoding39Condition1;
        arch_conditions_[8][39][2] = IsArch8Encoding39Condition2;
        arch_conditions_[8][39][3] = IsArch8Encoding39Condition3;
        arch_conditions_[8][39][10] = IsArch8Encoding39Condition10;
        arch_conditions_[8][39][11] = IsArch8Encoding39Condition11;
        arch_conditions_[8][39][12] = IsArch8Encoding39Condition12;
        arch_conditions_[8][39][13] = IsArch8Encoding39Condition13;
        arch_conditions_[8][40][15] = IsArch8Encoding40Condition15;
        arch_conditions_[8][41][14] = IsArch8Encoding41Condition14;
        arch_conditions_[8][42][1] = IsArch8Encoding42Condition1;
        arch_conditions_[8][42][2] = IsArch8Encoding42Condition2;
        arch_conditions_[8][42][3] = IsArch8Encoding42Condition3;
        arch_conditions_[8][42][10] = IsArch8Encoding42Condition10;
        arch_conditions_[8][42][11] = IsArch8Encoding42Condition11;
        arch_conditions_[8][42][12] = IsArch8Encoding42Condition12;
        arch_conditions_[8][42][13] = IsArch8Encoding42Condition13;
        arch_conditions_[8][43][15] = IsArch8Encoding43Condition15;
        arch_conditions_[8][44][14] = IsArch8Encoding44Condition14;
        arch_conditions_[8][45][5] = IsArch8Encoding45Condition5;
        arch_conditions_[8][46][15] = IsArch8Encoding46Condition15;
        arch_conditions_[8][47][14] = IsArch8Encoding47Condition14;
        arch_conditions_[8][48][0] = IsArch8Encoding48Condition0;
        arch_conditions_[8][49][0] = IsArch8Encoding49Condition0;
        arch_conditions_[8][49][5] = IsArch8Encoding49Condition5;
        arch_conditions_[8][50][0] = IsArch8Encoding50Condition0;
        arch_conditions_[8][51][0] = IsArch8Encoding51Condition0;
        arch_conditions_[8][51][5] = IsArch8Encoding51Condition5;
        arch_conditions_[9][0][17] = IsArch9Encoding0Condition17;
        arch_conditions_[9][1][0] = IsArch9Encoding1Condition0;
        arch_conditions_[9][4][0] = IsArch9Encoding4Condition0;
        arch_conditions_[9][6][0] = IsArch9Encoding6Condition0;
        arch_conditions_[9][7][0] = IsArch9Encoding7Condition0;
        arch_conditions_[9][8][0] = IsArch9Encoding8Condition0;
        arch_conditions_[9][9][0] = IsArch9Encoding9Condition0;
        arch_conditions_[9][10][0] = IsArch9Encoding10Condition0;
        arch_conditions_[9][19][0] = IsArch9Encoding19Condition0;
        arch_conditions_[9][24][16] = IsArch9Encoding24Condition16;
        arch_conditions_[9][25][1] = IsArch9Encoding25Condition1;
        arch_conditions_[9][26][0] = IsArch9Encoding26Condition0;
        arch_conditions_[9][26][1] = IsArch9Encoding26Condition1;
        arch_conditions_[9][26][2] = IsArch9Encoding26Condition2;
        arch_conditions_[9][26][3] = IsArch9Encoding26Condition3;
        arch_conditions_[9][27][1] = IsArch9Encoding27Condition1;
        arch_conditions_[9][27][2] = IsArch9Encoding27Condition2;
        arch_conditions_[9][27][3] = IsArch9Encoding27Condition3;
        arch_conditions_[9][29][5] = IsArch9Encoding29Condition5;
        arch_conditions_[9][30][15] = IsArch9Encoding30Condition15;
        arch_conditions_[9][31][14] = IsArch9Encoding31Condition14;
        arch_conditions_[9][32][1] = IsArch9Encoding32Condition1;
        arch_conditions_[9][32][2] = IsArch9Encoding32Condition2;
        arch_conditions_[9][32][3] = IsArch9Encoding32Condition3;
        arch_conditions_[9][32][10] = IsArch9Encoding32Condition10;
        arch_conditions_[9][32][11] = IsArch9Encoding32Condition11;
        arch_conditions_[9][32][12] = IsArch9Encoding32Condition12;
        arch_conditions_[9][32][13] = IsArch9Encoding32Condition13;
        arch_conditions_[9][33][15] = IsArch9Encoding33Condition15;
        arch_conditions_[9][34][14] = IsArch9Encoding34Condition14;
        arch_conditions_[9][35][0] = IsArch9Encoding35Condition0;
        arch_conditions_[9][35][5] = IsArch9Encoding35Condition5;
        arch_conditions_[9][36][15] = IsArch9Encoding36Condition15;
        arch_conditions_[9][37][14] = IsArch9Encoding37Condition14;
        arch_conditions_[9][38][0] = IsArch9Encoding38Condition0;
        arch_conditions_[9][39][1] = IsArch9Encoding39Condition1;
        arch_conditions_[9][39][2] = IsArch9Encoding39Condition2;
        arch_conditions_[9][39][3] = IsArch9Encoding39Condition3;
        arch_conditions_[9][39][10] = IsArch9Encoding39Condition10;
        arch_conditions_[9][39][11] = IsArch9Encoding39Condition11;
        arch_conditions_[9][39][12] = IsArch9Encoding39Condition12;
        arch_conditions_[9][39][13] = IsArch9Encoding39Condition13;
        arch_conditions_[9][40][15] = IsArch9Encoding40Condition15;
        arch_conditions_[9][41][14] = IsArch9Encoding41Condition14;
        arch_conditions_[9][42][1] = IsArch9Encoding42Condition1;
        arch_conditions_[9][42][2] = IsArch9Encoding42Condition2;
        arch_conditions_[9][42][3] = IsArch9Encoding42Condition3;
        arch_conditions_[9][42][10] = IsArch9Encoding42Condition10;
        arch_conditions_[9][42][11] = IsArch9Encoding42Condition11;
        arch_conditions_[9][42][12] = IsArch9Encoding42Condition12;
        arch_conditions_[9][42][13] = IsArch9Encoding42Condition13;
        arch_conditions_[9][43][15] = IsArch9Encoding43Condition15;
        arch_conditions_[9][44][14] = IsArch9Encoding44Condition14;
        arch_conditions_[9][45][5] = IsArch9Encoding45Condition5;
        arch_conditions_[9][46][15] = IsArch9Encoding46Condition15;
        arch_conditions_[9][47][14] = IsArch9Encoding47Condition14;
        arch_conditions_[9][48][0] = IsArch9Encoding48Condition0;
        arch_conditions_[9][49][0] = IsArch9Encoding49Condition0;
        arch_conditions_[9][49][5] = IsArch9Encoding49Condition5;
        arch_conditions_[9][50][0] = IsArch9Encoding50Condition0;
        arch_conditions_[9][51][0] = IsArch9Encoding51Condition0;
        arch_conditions_[9][51][5] = IsArch9Encoding51Condition5;
        arch_conditions_[10][0][17] = IsArch10Encoding0Condition17;
        arch_conditions_[10][1][0] = IsArch10Encoding1Condition0;
        arch_conditions_[10][4][0] = IsArch10Encoding4Condition0;
        arch_conditions_[10][6][0] = IsArch10Encoding6Condition0;
        arch_conditions_[10][7][0] = IsArch10Encoding7Condition0;
        arch_conditions_[10][8][0] = IsArch10Encoding8Condition0;
        arch_conditions_[10][9][0] = IsArch10Encoding9Condition0;
        arch_conditions_[10][10][0] = IsArch10Encoding10Condition0;
        arch_conditions_[10][24][1] = IsArch10Encoding24Condition1;
        arch_conditions_[10][25][0] = IsArch10Encoding25Condition0;
        arch_conditions_[10][25][1] = IsArch10Encoding25Condition1;
        arch_conditions_[10][25][2] = IsArch10Encoding25Condition2;
        arch_conditions_[10][25][3] = IsArch10Encoding25Condition3;
        arch_conditions_[10][26][1] = IsArch10Encoding26Condition1;
        arch_conditions_[10][26][2] = IsArch10Encoding26Condition2;
        arch_conditions_[10][26][3] = IsArch10Encoding26Condition3;
        arch_conditions_[10][28][5] = IsArch10Encoding28Condition5;
        arch_conditions_[10][29][15] = IsArch10Encoding29Condition15;
        arch_conditions_[10][30][14] = IsArch10Encoding30Condition14;
        arch_conditions_[10][31][1] = IsArch10Encoding31Condition1;
        arch_conditions_[10][31][2] = IsArch10Encoding31Condition2;
        arch_conditions_[10][31][3] = IsArch10Encoding31Condition3;
        arch_conditions_[10][31][10] = IsArch10Encoding31Condition10;
        arch_conditions_[10][31][11] = IsArch10Encoding31Condition11;
        arch_conditions_[10][31][12] = IsArch10Encoding31Condition12;
        arch_conditions_[10][31][13] = IsArch10Encoding31Condition13;
        arch_conditions_[10][32][15] = IsArch10Encoding32Condition15;
        arch_conditions_[10][33][14] = IsArch10Encoding33Condition14;
        arch_conditions_[10][34][0] = IsArch10Encoding34Condition0;
        arch_conditions_[10][34][5] = IsArch10Encoding34Condition5;
        arch_conditions_[10][35][15] = IsArch10Encoding35Condition15;
        arch_conditions_[10][36][14] = IsArch10Encoding36Condition14;
        arch_conditions_[10][37][0] = IsArch10Encoding37Condition0;
        arch_conditions_[10][38][1] = IsArch10Encoding38Condition1;
        arch_conditions_[10][38][2] = IsArch10Encoding38Condition2;
        arch_conditions_[10][38][3] = IsArch10Encoding38Condition3;
        arch_conditions_[10][38][10] = IsArch10Encoding38Condition10;
        arch_conditions_[10][38][11] = IsArch10Encoding38Condition11;
        arch_conditions_[10][38][12] = IsArch10Encoding38Condition12;
        arch_conditions_[10][38][13] = IsArch10Encoding38Condition13;
        arch_conditions_[10][39][15] = IsArch10Encoding39Condition15;
        arch_conditions_[10][40][14] = IsArch10Encoding40Condition14;
        arch_conditions_[10][41][1] = IsArch10Encoding41Condition1;
        arch_conditions_[10][41][2] = IsArch10Encoding41Condition2;
        arch_conditions_[10][41][3] = IsArch10Encoding41Condition3;
        arch_conditions_[10][41][10] = IsArch10Encoding41Condition10;
        arch_conditions_[10][41][11] = IsArch10Encoding41Condition11;
        arch_conditions_[10][41][12] = IsArch10Encoding41Condition12;
        arch_conditions_[10][41][13] = IsArch10Encoding41Condition13;
        arch_conditions_[10][42][15] = IsArch10Encoding42Condition15;
        arch_conditions_[10][43][14] = IsArch10Encoding43Condition14;
        arch_conditions_[10][44][5] = IsArch10Encoding44Condition5;
        arch_conditions_[10][45][15] = IsArch10Encoding45Condition15;
        arch_conditions_[10][46][14] = IsArch10Encoding46Condition14;
        arch_conditions_[10][47][0] = IsArch10Encoding47Condition0;
        arch_conditions_[10][48][0] = IsArch10Encoding48Condition0;
        arch_conditions_[10][48][5] = IsArch10Encoding48Condition5;
        arch_conditions_[10][49][0] = IsArch10Encoding49Condition0;
        arch_conditions_[10][50][0] = IsArch10Encoding50Condition0;
        arch_conditions_[10][50][5] = IsArch10Encoding50Condition5;
        arch_conditions_[4][0][18] = IsArch4Encoding0Condition18;
        arch_conditions_[4][1][0] = IsArch4Encoding1Condition0;
        arch_conditions_[4][4][0] = IsArch4Encoding4Condition0;
        arch_conditions_[4][6][0] = IsArch4Encoding6Condition0;
        arch_conditions_[4][7][0] = IsArch4Encoding7Condition0;
        arch_conditions_[4][8][0] = IsArch4Encoding8Condition0;
        arch_conditions_[4][9][0] = IsArch4Encoding9Condition0;
        arch_conditions_[4][12][0] = IsArch4Encoding12Condition0;
        arch_conditions_[4][29][1] = IsArch4Encoding29Condition1;
        arch_conditions_[4][30][19] = IsArch4Encoding30Condition19;
        arch_conditions_[4][31][0] = IsArch4Encoding31Condition0;
        arch_conditions_[4][31][1] = IsArch4Encoding31Condition1;
        arch_conditions_[4][31][2] = IsArch4Encoding31Condition2;
        arch_conditions_[4][31][3] = IsArch4Encoding31Condition3;
        arch_conditions_[4][32][19] = IsArch4Encoding32Condition19;
        arch_conditions_[4][32][20] = IsArch4Encoding32Condition20;
        arch_conditions_[4][32][21] = IsArch4Encoding32Condition21;
        arch_conditions_[4][33][1] = IsArch4Encoding33Condition1;
        arch_conditions_[4][33][2] = IsArch4Encoding33Condition2;
        arch_conditions_[4][33][3] = IsArch4Encoding33Condition3;
        arch_conditions_[4][34][19] = IsArch4Encoding34Condition19;
        arch_conditions_[4][34][20] = IsArch4Encoding34Condition20;
        arch_conditions_[4][34][21] = IsArch4Encoding34Condition21;
        arch_conditions_[4][36][5] = IsArch4Encoding36Condition5;
        arch_conditions_[4][37][22] = IsArch4Encoding37Condition22;
        arch_conditions_[4][38][15] = IsArch4Encoding38Condition15;
        arch_conditions_[4][39][14] = IsArch4Encoding39Condition14;
        arch_conditions_[4][40][1] = IsArch4Encoding40Condition1;
        arch_conditions_[4][40][2] = IsArch4Encoding40Condition2;
        arch_conditions_[4][40][3] = IsArch4Encoding40Condition3;
        arch_conditions_[4][40][10] = IsArch4Encoding40Condition10;
        arch_conditions_[4][40][11] = IsArch4Encoding40Condition11;
        arch_conditions_[4][40][12] = IsArch4Encoding40Condition12;
        arch_conditions_[4][40][13] = IsArch4Encoding40Condition13;
        arch_conditions_[4][41][15] = IsArch4Encoding41Condition15;
        arch_conditions_[4][42][14] = IsArch4Encoding42Condition14;
        arch_conditions_[4][43][0] = IsArch4Encoding43Condition0;
        arch_conditions_[4][43][5] = IsArch4Encoding43Condition5;
        arch_conditions_[4][44][0] = IsArch4Encoding44Condition0;
        arch_conditions_[4][44][22] = IsArch4Encoding44Condition22;
        arch_conditions_[4][45][15] = IsArch4Encoding45Condition15;
        arch_conditions_[4][46][14] = IsArch4Encoding46Condition14;
        arch_conditions_[4][47][0] = IsArch4Encoding47Condition0;
        arch_conditions_[4][48][1] = IsArch4Encoding48Condition1;
        arch_conditions_[4][48][2] = IsArch4Encoding48Condition2;
        arch_conditions_[4][48][3] = IsArch4Encoding48Condition3;
        arch_conditions_[4][48][10] = IsArch4Encoding48Condition10;
        arch_conditions_[4][48][11] = IsArch4Encoding48Condition11;
        arch_conditions_[4][48][12] = IsArch4Encoding48Condition12;
        arch_conditions_[4][48][13] = IsArch4Encoding48Condition13;
        arch_conditions_[4][49][15] = IsArch4Encoding49Condition15;
        arch_conditions_[4][50][14] = IsArch4Encoding50Condition14;
        arch_conditions_[4][51][1] = IsArch4Encoding51Condition1;
        arch_conditions_[4][51][2] = IsArch4Encoding51Condition2;
        arch_conditions_[4][51][3] = IsArch4Encoding51Condition3;
        arch_conditions_[4][51][10] = IsArch4Encoding51Condition10;
        arch_conditions_[4][51][11] = IsArch4Encoding51Condition11;
        arch_conditions_[4][51][12] = IsArch4Encoding51Condition12;
        arch_conditions_[4][51][13] = IsArch4Encoding51Condition13;
        arch_conditions_[4][52][15] = IsArch4Encoding52Condition15;
        arch_conditions_[4][53][14] = IsArch4Encoding53Condition14;
        arch_conditions_[4][54][5] = IsArch4Encoding54Condition5;
        arch_conditions_[4][55][22] = IsArch4Encoding55Condition22;
        arch_conditions_[4][56][15] = IsArch4Encoding56Condition15;
        arch_conditions_[4][57][14] = IsArch4Encoding57Condition14;
        arch_conditions_[4][59][0] = IsArch4Encoding59Condition0;
        arch_conditions_[4][60][0] = IsArch4Encoding60Condition0;
        arch_conditions_[4][60][5] = IsArch4Encoding60Condition5;
        arch_conditions_[4][61][0] = IsArch4Encoding61Condition0;
        arch_conditions_[4][62][0] = IsArch4Encoding62Condition0;
        arch_conditions_[4][62][5] = IsArch4Encoding62Condition5;
    } // EncodingConditionHandler

    using ConditionIdToConditionFunctions = std::map<uint32_t, std::function<bool(uint64_t)>>;
    using EncodingOrderToConditionId = std::map<uint32_t, ConditionIdToConditionFunctions>;
    std::map<uint32_t, EncodingOrderToConditionId> arch_conditions_;
}; // struct EncodingConditionHandler
} // namespace amdisa
#endif // ENCODING_CONDITION_HANDLER_HPP_
