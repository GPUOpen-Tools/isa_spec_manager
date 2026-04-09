/*
 * Copyright (c) 2025 Advanced Micro Devices, Inc. All rights reserved.
 */
#include "amdisa/isa_decoder.h"

// C++ libraries.
#include <algorithm>
#include <cassert>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>

// Local libraries.
#include "amdisa_structures.h"
#include "amdisa_utility.h"
#include "amdisa/api_version.h"
#include "encoding_condition_handler.hpp"
#include "isa_xml_reader.h"

namespace amdisa
{
    // Error string constants.
    static const char* kStringErrorEmptyRange                  = "Error: Processing empty range.";
    static const char* kStringErrorEncodingNotFound            = "Error: Encoding or opcode not found for the instruction: ";
    static const char* kStringErrorEmptyOperandName            = "Error: Empty operand name was provided.";
    static const char* kStringErrorInstructionNotFoundInSpec   = "Error: Instruction not found in the XML ISA Specification. ";
    static const char* kStringErrorFailedToDecodeOperands      = "Error: Failed to decode operands.";
    static const char* kStringErrorSpecNotInitialized          = "Error: API Implementation is not initialized.";
    static const char* kStringErrorModiferFieldNotFound        = "Error: Defined modifier was not found in the encoding.Check encodings to modifiers mapping.";
    static const char* kStringErrorShaderFileNotOpen           = "Error: Failed to read shader disassembly file.";
    static const char* kStringErrorBranchTargetInfoNotFound    = "Error: Failed to find target information for the direct branch. ";
    static const char* kStringErrorBranchTargetIndexNotFound   = "Error: Failed to find target index for the direct branch: ";
    static const char* kStringErrorBranchTargetLabelNotFound   = "Error: Failed to find target label for the direct branch: ";
    static const char* kStringExceptionShaderFileNotOpen       = "Error: Exception occured while opening shader disassembly for reading: ";
    static const char* kStringErrorShaderTextInvalidDwordByte  = "Error: Invalid DWORD Byte: ";
    static const char* kStringErrorShaderTextDecodeFailed      = "Error: Instruction decoding failed during shader disassembly processing.";
    static const char* kStringErrorShaderTextDecodeNoInst      = "Error: No valid instructions found in the provided shader disassembly file.";
    static const char* kStringErrorMissingTargetResolutionInfo = "Error: Branch target resolution information missing";
    static const char* kStringErrorIsaDecoderNotFound          = "Error: Invalid IsaDecoder provided to decode the instructions";
    static const char* kStringNa                               = "N/A";
    static const char* kStringErrorApiImplAllocationFailed     = "Error: API Implementation object allocation failed";
    static const char* kStringErrorManagerImplAllocationFailed = "Error: Manager Implementation object allocation failed";
    static const char* kStringErrorEmptyRangesInField          = "Error: Range is empty for the field: ";
    static const char* kStringErrorEncodingIdentifiersNotFound = "Error: Could not find the encoding identifier for the instruction.";

    // Decoder manager string constants.
    static const char* kStringErrorDecodeManagerUnknownArch  = "Error: Undefined architecture in specification.";
    static const char* kStringErrorDecodeManagerInitFailed   = "Error: Failed to initialize ISA specification. Provided file path: ";
    static const char* kStringErrorDecodeManagerArchNotFound = "Error: Provided architecture was not initialized.";

    // Warnings.
    static const char* kStringWarningEmptyStream            = "Warning: Trying to get DWORD from an empty stream.";
    static const char* kStringWarningOutRangeDwordAccess    = "Warning: GetField() was provided short working_dwords.";
    static const char* kStringWarningApiInitSameMappingInst = "Warning: Id is mapping to the same instruction.";
    static const char* kStringWarningApiInitSameMappingEnc  = "Warning: Id is mapping to the same instruction encoding.";
    static const char* kStringWarningSegBits                = "Warning: Marked with SEG field, but doesn't match with constants.";

    // Encoding modifier definitions. Constructed based on the PDF ISA.
    static const char* kModifierNameNegation       = "NEG";
    static const char* kModifierNameOutputModifier = "OMOD";
    static const char* kModifierNameAbsoluteValue  = "ABS";
    static const char* kModifierNameClamp          = "CLAMP";
    static const char* kModifierNameOffset         = "OFFSET";

    // Define which encodings have modifiers.
    static const std::map<std::string, std::vector<std::string>> kEncodingToOperandModifiers = {
        {"ENC_VOP3", {kModifierNameAbsoluteValue, kModifierNameNegation, kModifierNameOutputModifier, kModifierNameClamp}},
        {"ENC_VOP3B", {kModifierNameNegation, kModifierNameOutputModifier, kModifierNameClamp}},
        {"ENC_VOP3P", {kModifierNameNegation, kModifierNameClamp}},
        {"ENC_FLAT", {kModifierNameOffset}},
        {"ENC_FLAT_SCRATCH", {kModifierNameOffset}},
        {"ENC_FLAT_GLBL", {kModifierNameOffset}},
        {"ENC_FLAT_GLOBAL", {kModifierNameOffset}}};

    // SEG bits for FLAT, SCRATCH, and GLOBAL encodings.
    static const std::map<std::string, uint32_t> kSegBits = {{"ENC_FLAT", 0}, {"ENC_FLAT_SCRATCH", 1}, {"ENC_FLAT_GLBL", 2}, {"ENC_FLAT_GLOBAL", 2}};

    // Map architecture IDs to enums.
    static const std::map<uint32_t, GpuArchitecture> kArchitectureIdToEnum = {{0, GpuArchitecture::kCdna1},
                                                                              {1, GpuArchitecture::kCdna2},
                                                                              {2, GpuArchitecture::kCdna3},
                                                                              {3, GpuArchitecture::kCdna4},
                                                                              {5, GpuArchitecture::kRdna1},
                                                                              {6, GpuArchitecture::kRdna2},
                                                                              {8, GpuArchitecture::kRdna3},
                                                                              {9, GpuArchitecture::kRdna3_5},
                                                                              {10, GpuArchitecture::kRdna4}};
    static GpuArchitecture GetArchitectureWithId(uint32_t architecture_id)
    {
        GpuArchitecture ret            = GpuArchitecture::kUnknown;
        auto            arch_enum_iter = kArchitectureIdToEnum.find(architecture_id);
        if (arch_enum_iter != kArchitectureIdToEnum.end())
        {
            ret = arch_enum_iter->second;
        }
        return ret;
    }

    // Masks.
    static const uint32_t kDwordMask = 0xffffffff;

    // Instructions shader disassembly text delimiter
    static const char kShaderTextDelimiter = '\n';

    // Instruction byte size constant (32 bit, 4 bytes)
    static const unsigned int kInstructionBytes32 = 4;
    static const unsigned int kDwordSize          = 32;

    // ISA information that require special handling.
    static const char* kWaveDependentFormat = "FMT_NUM_M64";

    // Type definitions.
    using IdToEncoding             = std::unordered_map<uint64_t, std::shared_ptr<Encoding>>;
    using IdToInstruction          = std::unordered_map<uint64_t, std::shared_ptr<Instruction>>;
    using IdToInstructionEncodings = std::unordered_map<uint64_t, std::vector<std::shared_ptr<InstructionEncoding>>>;
    using EncodingIterator         = std::vector<Encoding>::const_iterator;
    using FieldIterator            = std::vector<Field>::const_iterator;
    struct InstructionPtrs
    {
        std::shared_ptr<Instruction>         instruction_ptr          = nullptr;
        std::shared_ptr<InstructionEncoding> instruction_encoding_ptr = nullptr;
    };

    // *** INTERNALLY-LINKED AUXILIARY FUNCTIONS - BEGIN ***

    static uint64_t GetFieldValue(const Field& field, const std::vector<uint32_t>& working_dwords, std::vector<std::string>& log)
    {
        Range    range;
        uint64_t field_value = 0;
        if (!field.ranges.empty())
        {
            uint32_t              range_order   = 0;
            uint32_t              prev_bitcount = 0;
            std::vector<uint32_t> range_values;
            for (const auto& range : field.ranges)
            {
                assert(range.order == range_order);
                ++range_order;

                uint64_t bit_count   = range.bit_count;
                uint64_t bit_offset  = range.bit_offset;
                uint64_t dword_index = bit_offset / 32;
                uint32_t dword       = 0;
                uint32_t range_value = 0;

                if (dword_index < working_dwords.size())
                {
                    dword = working_dwords[dword_index];
                }
                else
                {
                    log.push_back(kStringWarningOutRangeDwordAccess);
                }
                uint64_t mask = ((1ULL << bit_count) - 1);
                range_value   = (dword >> (bit_offset - 32 * dword_index)) & mask;
                uint64_t padding_size = range.padding.bit_count;
                if (padding_size > 0)
                {
                    uint32_t padding_value = range.padding.value;
                    range_value            = (range_value << padding_size) | padding_value;
                }
                range_value <<= prev_bitcount;
                prev_bitcount += range.bit_count;

                range_values.push_back(range_value);
            }

            for (uint32_t range_value : range_values)
            {
                field_value |= range_value;
            }
        }
        else
        {
            log.push_back(kStringErrorEmptyRange);
            assert(false);
        }
        return field_value;
    }

    // Gets the iterator to the field from the bitmap with a specific name and gets the value of
    // the field from the machine code.
    static FieldIterator GetFieldIterator(const std::vector<uint32_t>& machine_code,
                                          const std::string&           field_name,
                                          const MicrocodeFormat&       microcode_format,
                                          uint64_t&                    field_value,
                                          std::vector<std::string>&    log)
    {
        FieldIterator found_field    = microcode_format.bit_map.end();
        bool          is_field_found = field_name.empty();

        // Go over each field in the passed microcode format structure.
        for (auto field_iterator = microcode_format.bit_map.begin(); !is_field_found && field_iterator != microcode_format.bit_map.end(); ++field_iterator)
        {
            if (field_iterator->name.find(field_name) != std::string::npos)
            {
                is_field_found = true;
                field_value    = GetFieldValue(*field_iterator, machine_code, log);
                found_field    = field_iterator;
            }
        }

        return found_field;
    }

    // FIXME: This namespace groups the code for the workaround to handle
    // address component handling in MIMG instructions. The information
    // hardcoded in this workaround must be transferred to XML.
    namespace mimg_workaround
    {
        static const char* kStringErrorFailedToGetAddressCount = "Error: Failed to retrieve address count (acnt)";

        // In MIMG encoding acnt (Address Component Count) indicates how many address
        // components (like x, y, z, slice, face_id, fragid, mipid) are passed
        // through VGPRs to the image instruction.
        // This structure holds and provides Acnt values based on various fields
        // of the instruction.
        struct AcntHandler
        {
            // Dimension of the surface.
            // Specifiction of DIM field in MIMG encoding.
            enum class Dimension : int8_t
            {
                kUndefined = -1,
                // A linear image.
                k1d = 0,

                // A two-dimensional image.
                k2d = 1,

                // A volumetric image.
                k3d = 2,

                // A cubemap consisting of 6 square 2D faces.
                kCube = 3,

                // A collection of 1D images.
                k1dArray = 4,

                // A stack of 2D images.
                k2dArray = 5,

                // A 2D image with multisample anti-aliasing support.
                k2dMsaa = 6,

                // A combination of 2D array and multisample anti-aliasing support.
                k2dMsaaArray = 7
            };

            // At the core, Acnt is based on dimension. It then varies based on
            // the opcode.
            using DimensionToAcntTable = std::unordered_map<Dimension, uint32_t>;

            // Generates Acnt information for RDNA 2 architecture.
            void GenerateRdnaCdna1or2Table(GpuArchitecture architecture)
            {
                current_architecture = architecture;
                tables[current_architecture]         = std::make_unique<AcntTable>();
                AcntTable& table                     = *tables[current_architecture];

                // Define table for load/store/atomic instructions.
                std::shared_ptr<DimensionToAcntTable> lsa_table = std::make_shared<DimensionToAcntTable>();
                lsa_table->emplace(Dimension::k1d, 0);
                lsa_table->emplace(Dimension::k2d, 1);
                lsa_table->emplace(Dimension::k3d, 2);
                lsa_table->emplace(Dimension::kCube, 2);
                lsa_table->emplace(Dimension::k1dArray, 1);
                lsa_table->emplace(Dimension::k2dArray, 2);
                lsa_table->emplace(Dimension::k2dMsaa, 2);
                lsa_table->emplace(Dimension::k2dMsaaArray, 3);
                table[0]   = lsa_table;  // IMAGE_LOAD
                table[2]   = lsa_table;  // IMAGE_LOAD_PCK
                table[3]   = lsa_table;  // IMAGE_LOAD_PCK_SGN
                table[8]   = lsa_table;  // IMAGE_STORE
                table[10]  = lsa_table;  // IMAGE_STORE_PCK
                table[15]  = lsa_table;  // IMAGE_ATOMIC_SWAP
                table[16]  = lsa_table;  // IMAGE_ATOMIC_CMPSWAP
                table[17]  = lsa_table;  // IMAGE_ATOMIC_ADD
                table[18]  = lsa_table;  // IMAGE_ATOMIC_SUB
                table[20]  = lsa_table;  // IMAGE_ATOMIC_SMIN
                table[21]  = lsa_table;  // IMAGE_ATOMIC_UMIN
                table[22]  = lsa_table;  // IMAGE_ATOMIC_SMAX
                table[23]  = lsa_table;  // IMAGE_ATOMIC_UMAX
                table[24]  = lsa_table;  // IMAGE_ATOMIC_AND
                table[25]  = lsa_table;  // IMAGE_ATOMIC_OR
                table[26]  = lsa_table;  // IMAGE_ATOMIC_XOR
                table[27]  = lsa_table;  // IMAGE_ATOMIC_INC
                table[28]  = lsa_table;  // IMAGE_ATOMIC_DEC
                table[29]  = lsa_table;  // IMAGE_ATOMIC_FCMPSWAP
                table[30]  = lsa_table;  // IMAGE_ATOMIC_FMIN
                table[31]  = lsa_table;  // IMAGE_ATOMIC_FMAX
                table[66]  = lsa_table;  // IMAGE_LOAD_BY2
                table[67]  = lsa_table;  // IMAGE_LOAD_BY4
                table[82]  = lsa_table;  // IMAGE_STORE_BY2
                table[83]  = lsa_table;  // IMAGE_STORE_BY4
                table[112] = lsa_table;  // IMAGE_LOAD_PCK2
                table[113] = lsa_table;  // IMAGE_LOAD_PCK4
                table[118] = lsa_table;  // IMAGE_STORE_PCK2
                table[119] = lsa_table;  // IMAGE_STORE_PCK4
                table[128] = lsa_table;  // IMAGE_MSAA_LOAD

                // Define table for load_mip/store_mip instructions.
                std::shared_ptr<DimensionToAcntTable> ls_mip_table = std::make_shared<DimensionToAcntTable>();
                ls_mip_table->emplace(Dimension::k1d, 1);
                ls_mip_table->emplace(Dimension::k2d, 2);
                ls_mip_table->emplace(Dimension::k3d, 3);
                ls_mip_table->emplace(Dimension::kCube, 3);
                ls_mip_table->emplace(Dimension::k1dArray, 2);
                ls_mip_table->emplace(Dimension::k2dArray, 3);
                table[1]   = ls_mip_table;  // IMAGE_LOAD_MIP
                table[4]   = ls_mip_table;  // IMAGE_LOAD_MIP_PCK
                table[5]   = ls_mip_table;  // IMAGE_LOAD_MIP_PCK_SGN
                table[9]   = ls_mip_table;  // IMAGE_STORE_MIP
                table[11]  = ls_mip_table;  // IMAGE_STORE_MIP_PCK
                table[74]  = ls_mip_table;  // IMAGE_LOAD_MIP_BY2
                table[75]  = ls_mip_table;  // IMAGE_LOAD_MIP_BY4
                table[90]  = ls_mip_table;  // IMAGE_STORE_MIP_BY2
                table[91]  = ls_mip_table;  // IMAGE_STORE_MIP_BY4
                table[115] = ls_mip_table;  // IMAGE_LOAD_MIP_PCK2
                table[116] = ls_mip_table;  // IMAGE_LOAD_MIP_PCK4
                table[121] = ls_mip_table;  // IMAGE_STORE_MIP_PCK2
                table[122] = ls_mip_table;  // IMAGE_STORE_MIP_PCK4

                // Define table for sample instructions.
                std::shared_ptr<DimensionToAcntTable> sample_table = std::make_shared<DimensionToAcntTable>();
                sample_table->emplace(Dimension::k1d, 0);
                sample_table->emplace(Dimension::k2d, 1);
                sample_table->emplace(Dimension::k3d, 2);
                sample_table->emplace(Dimension::kCube, 2);
                sample_table->emplace(Dimension::k1dArray, 1);
                sample_table->emplace(Dimension::k2dArray, 2);
                table[32]  = sample_table;  // IMAGE_SAMPLE
                table[34]  = sample_table;  // IMAGE_SAMPLE_D
                table[37]  = sample_table;  // IMAGE_SAMPLE_B
                table[40]  = sample_table;  // IMAGE_SAMPLE_C
                table[42]  = sample_table;  // IMAGE_SAMPLE_C_D
                table[45]  = sample_table;  // IMAGE_SAMPLE_C_B
                table[48]  = sample_table;  // IMAGE_SAMPLE_O
                table[50]  = sample_table;  // IMAGE_SAMPLE_D_O
                table[53]  = sample_table;  // IMAGE_SAMPLE_B_O
                table[56]  = sample_table;  // IMAGE_SAMPLE_C_O
                table[58]  = sample_table;  // IMAGE_SAMPLE_C_D_O
                table[61]  = sample_table;  // IMAGE_SAMPLE_C_B_O
                table[104] = sample_table;  // IMAGE_SAMPLE_CD
                table[106] = sample_table;  // IMAGE_SAMPLE_C_CD
                table[108] = sample_table;  // IMAGE_SAMPLE_CD_O
                table[110] = sample_table;  // IMAGE_SAMPLE_C_CD_O
                table[162] = sample_table;  // IMAGE_SAMPLE_D_G16
                table[170] = sample_table;  // IMAGE_SAMPLE_C_D_G16
                table[178] = sample_table;  // IMAGE_SAMPLE_D_O_G16
                table[186] = sample_table;  // IMAGE_SAMPLE_C_D_O_G16
                table[232] = sample_table;  // IMAGE_SAMPLE_CD_G16
                table[234] = sample_table;  // IMAGE_SAMPLE_C_CD_G16
                table[236] = sample_table;  // IMAGE_SAMPLE_CD_O_G16
                table[238] = sample_table;  // IMAGE_SAMPLE_C_CD_O_G16

                // Define table for sample_l_cl instructions.
                std::shared_ptr<DimensionToAcntTable> sample_l_cl_table = std::make_shared<DimensionToAcntTable>();
                sample_l_cl_table->emplace(Dimension::k1d, 1);
                sample_l_cl_table->emplace(Dimension::k2d, 2);
                sample_l_cl_table->emplace(Dimension::k3d, 3);
                sample_l_cl_table->emplace(Dimension::kCube, 3);
                sample_l_cl_table->emplace(Dimension::k1dArray, 2);
                sample_l_cl_table->emplace(Dimension::k2dArray, 3);
                table[33]  = sample_l_cl_table;  // IMAGE_SAMPLE_CL
                table[35]  = sample_l_cl_table;  // IMAGE_SAMPLE_D_CL
                table[36]  = sample_l_cl_table;  // IMAGE_SAMPLE_L
                table[38]  = sample_l_cl_table;  // IMAGE_SAMPLE_B_CL
                table[39]  = sample_l_cl_table;  // IMAGE_SAMPLE_LZ
                table[41]  = sample_l_cl_table;  // IMAGE_SAMPLE_C_CL
                table[43]  = sample_l_cl_table;  // IMAGE_SAMPLE_C_D_CL
                table[44]  = sample_l_cl_table;  // IMAGE_SAMPLE_C_L
                table[46]  = sample_l_cl_table;  // IMAGE_SAMPLE_C_B_CL
                table[47]  = sample_l_cl_table;  // IMAGE_SAMPLE_C_LZ
                table[49]  = sample_l_cl_table;  // IMAGE_SAMPLE_CL_O
                table[51]  = sample_l_cl_table;  // IMAGE_SAMPLE_D_CL_O
                table[52]  = sample_l_cl_table;  // IMAGE_SAMPLE_L_O
                table[54]  = sample_l_cl_table;  // IMAGE_SAMPLE_B_CL_O
                table[55]  = sample_l_cl_table;  // IMAGE_SAMPLE_LZ_O
                table[57]  = sample_l_cl_table;  // IMAGE_SAMPLE_C_CL_O
                table[59]  = sample_l_cl_table;  // IMAGE_SAMPLE_C_D_CL_O
                table[60]  = sample_l_cl_table;  // IMAGE_SAMPLE_C_L_O
                table[62]  = sample_l_cl_table;  // IMAGE_SAMPLE_C_B_CL_O
                table[63]  = sample_l_cl_table;  // IMAGE_SAMPLE_C_LZ_O
                table[105] = sample_l_cl_table;  // IMAGE_SAMPLE_CD_CL
                table[107] = sample_l_cl_table;  // IMAGE_SAMPLE_C_CD_CL
                table[109] = sample_l_cl_table;  // IMAGE_SAMPLE_CD_CL_O
                table[111] = sample_l_cl_table;  // IMAGE_SAMPLE_C_CD_CL_O
                table[163] = sample_l_cl_table;  // IMAGE_SAMPLE_D_CL_G16
                table[171] = sample_l_cl_table;  // IMAGE_SAMPLE_C_D_CL_G16
                table[179] = sample_l_cl_table;  // IMAGE_SAMPLE_D_CL_O_G16
                table[187] = sample_l_cl_table;  // IMAGE_SAMPLE_C_D_CL_O_G16
                table[233] = sample_l_cl_table;  // IMAGE_SAMPLE_CD_CL_G16
                table[235] = sample_l_cl_table;  // IMAGE_SAMPLE_C_CD_CL_G16
                table[237] = sample_l_cl_table;  // IMAGE_SAMPLE_CD_CL_O_G16
                table[239] = sample_l_cl_table;  // IMAGE_SAMPLE_C_CD_CL_O_G16

                // Define table for gather4 instruction.
                std::shared_ptr<DimensionToAcntTable> gather4_table = std::make_shared<DimensionToAcntTable>();
                gather4_table->emplace(Dimension::k2d, 1);
                gather4_table->emplace(Dimension::k2dArray, 2);
                gather4_table->emplace(Dimension::kCube, 2);
                table[64] = gather4_table;  // IMAGE_GATHER4
                table[69] = gather4_table;  // IMAGE_GATHER4_B
                table[72] = gather4_table;  // IMAGE_GATHER4_C
                table[77] = gather4_table;  // IMAGE_GATHER4_C_B
                table[80] = gather4_table;  // IMAGE_GATHER4_O
                table[85] = gather4_table;  // IMAGE_GATHER4_B_O
                table[88] = gather4_table;  // IMAGE_GATHER4_C_O
                table[93] = gather4_table;  // IMAGE_GATHER4_C_B_O
                table[97] = gather4_table;  // IMAGE_GATHER4H
                table[98] = gather4_table;  // IMAGE_GATHER4H_PCK

                // Define table for gather4 instructions with lod and clamp.
                std::shared_ptr<DimensionToAcntTable> gather4_l_cl_table = std::make_shared<DimensionToAcntTable>();
                gather4_l_cl_table->emplace(Dimension::k2d, 2);
                gather4_l_cl_table->emplace(Dimension::k2dArray, 3);
                gather4_l_cl_table->emplace(Dimension::kCube, 3);
                table[65] = gather4_l_cl_table;  // IMAGE_GATHER4_CL
                table[68] = gather4_l_cl_table;  // IMAGE_GATHER4_L
                table[70] = gather4_l_cl_table;  // IMAGE_GATHER4_B_CL
                table[71] = gather4_l_cl_table;  // IMAGE_GATHER4_LZ
                table[73] = gather4_l_cl_table;  // IMAGE_GATHER4_C_CL
                table[76] = gather4_l_cl_table;  // IMAGE_GATHER4_C_L
                table[78] = gather4_l_cl_table;  // IMAGE_GATHER4_C_B_CL
                table[79] = gather4_l_cl_table;  // IMAGE_GATHER4_C_LZ
                table[81] = gather4_l_cl_table;  // IMAGE_GATHER4_CL_O
                table[84] = gather4_l_cl_table;  // IMAGE_GATHER4_L_O
                table[86] = gather4_l_cl_table;  // IMAGE_GATHER4_B_CL_O
                table[87] = gather4_l_cl_table;  // IMAGE_GATHER4_LZ_O
                table[89] = gather4_l_cl_table;  // IMAGE_GATHER4_C_CL_O
                table[92] = gather4_l_cl_table;  // IMAGE_GATHER4_C_L_O
                table[94] = gather4_l_cl_table;  // IMAGE_GATHER4_C_B_CL_O
                table[95] = gather4_l_cl_table;  // IMAGE_GATHER4_C_LZ_O

                std::shared_ptr<DimensionToAcntTable> bvh_table = std::make_shared<DimensionToAcntTable>();
                bvh_table->emplace(Dimension::k1d, 11);
                bvh_table->emplace(Dimension::k2d, 11);
                bvh_table->emplace(Dimension::k3d, 11);
                bvh_table->emplace(Dimension::kCube, 11);
                bvh_table->emplace(Dimension::k1dArray, 11);
                bvh_table->emplace(Dimension::k2dArray, 11);
                bvh_table->emplace(Dimension::k2dMsaa, 11);
                bvh_table->emplace(Dimension::k2dMsaaArray, 11);
                table[230] = bvh_table;  // IMAGE_BVH_INTERSECT_RAY
                table[231] = bvh_table;  // IMAGE_BVH64_INTERSECT_RAY

                // Address field information.
                addr_fields[current_architecture] = {
                    "VADDR", "VADDRA", "VADDRB", "VADDRC", "VADDRD", "VADDRE", "VADDRF", "VADDRG", "VADDRH", "VADDRI", "VADDRJ", "VADDRK", "VADDRL"};
            }

            // Generates Acnt information for RDNA 3 architecture.
            void GenerateRdna3or3p5Table(GpuArchitecture architecture)
            {
                current_architecture = architecture;
                tables[current_architecture]         = std::make_unique<AcntTable>();
                AcntTable& table                     = *tables[current_architecture];

                // Define table for msaa_load instructions.
                std::shared_ptr<DimensionToAcntTable> msaa_load_table = std::make_shared<DimensionToAcntTable>();
                msaa_load_table->emplace(Dimension::k2dMsaa, 2);
                msaa_load_table->emplace(Dimension::k2dMsaaArray, 3);
                // Map to relevant opcodes.
                table[24] = msaa_load_table;

                // Define table for msaa_load instructions.
                std::shared_ptr<DimensionToAcntTable> load_store_table = std::make_shared<DimensionToAcntTable>();
                load_store_table->emplace(Dimension::k1d, 0);
                load_store_table->emplace(Dimension::k2d, 1);
                load_store_table->emplace(Dimension::k3d, 2);
                load_store_table->emplace(Dimension::kCube, 2);
                load_store_table->emplace(Dimension::k1dArray, 1);
                load_store_table->emplace(Dimension::k2dArray, 2);
                load_store_table->emplace(Dimension::k2dMsaa, 2);
                load_store_table->emplace(Dimension::k2dMsaaArray, 3);
                table[0]  = load_store_table;  // IMAGE_LOAD
                table[2]  = load_store_table;  // IMAGE_LOAD_PCK
                table[3]  = load_store_table;  // IMAGE_LOAD_PCK_SGN
                table[6]  = load_store_table;  // IMAGE_STORE
                table[8]  = load_store_table;  // IMAGE_STORE_PCK

                // Define table for atomic instructions.
                std::shared_ptr<DimensionToAcntTable> atomic_table = std::make_shared<DimensionToAcntTable>();
                atomic_table->emplace(Dimension::k1d, 0);
                atomic_table->emplace(Dimension::k2d, 1);
                atomic_table->emplace(Dimension::k3d, 2);
                atomic_table->emplace(Dimension::k1dArray, 1);
                atomic_table->emplace(Dimension::k2dArray, 2);
                atomic_table->emplace(Dimension::k2dMsaa, 2);
                atomic_table->emplace(Dimension::k2dMsaaArray, 3);
                table[10] = atomic_table;  // IMAGE_ATOMIC_SWAP
                table[11] = atomic_table;  // IMAGE_ATOMIC_CMPSWAP
                table[12] = atomic_table;  // IMAGE_ATOMIC_ADD
                table[13] = atomic_table;  // IMAGE_ATOMIC_SUB
                table[14] = atomic_table;  // IMAGE_ATOMIC_SMIN
                table[15] = atomic_table;  // IMAGE_ATOMIC_UMIN
                table[16] = atomic_table;  // IMAGE_ATOMIC_SMAX
                table[17] = atomic_table;  // IMAGE_ATOMIC_UMAX
                table[18] = atomic_table;  // IMAGE_ATOMIC_AND
                table[19] = atomic_table;  // IMAGE_ATOMIC_OR
                table[20] = atomic_table;  // IMAGE_ATOMIC_XOR
                table[21] = atomic_table;  // IMAGE_ATOMIC_INC
                table[22] = atomic_table;  // IMAGE_ATOMIC_DEC

                // Define table for load_mip/store_mip instructions.
                std::shared_ptr<DimensionToAcntTable> ls_mip_table = std::make_shared<DimensionToAcntTable>();
                ls_mip_table->emplace(Dimension::k1d, 1);
                ls_mip_table->emplace(Dimension::k2d, 2);
                ls_mip_table->emplace(Dimension::k3d, 3);
                ls_mip_table->emplace(Dimension::kCube, 3);
                ls_mip_table->emplace(Dimension::k1dArray, 2);
                ls_mip_table->emplace(Dimension::k2dArray, 3);
                table[1] = ls_mip_table;  // IMAGE_LOAD_MIP
                table[4] = ls_mip_table;  // IMAGE_LOAD_MIP_PCK
                table[5] = ls_mip_table;  // IMAGE_LOAD_MIP_PCK_SGN
                table[7] = ls_mip_table;  // IMAGE_STORE_MIP
                table[9] = ls_mip_table;  // IMAGE_STORE_MIP_PCK

                // Define table for sample instructions.
                std::shared_ptr<DimensionToAcntTable> sample_table = std::make_shared<DimensionToAcntTable>();
                sample_table->emplace(Dimension::k1d, 0);
                sample_table->emplace(Dimension::k2d, 1);
                sample_table->emplace(Dimension::k3d, 2);
                sample_table->emplace(Dimension::kCube, 2);
                sample_table->emplace(Dimension::k1dArray, 1);
                sample_table->emplace(Dimension::k2dArray, 2);
                table[27] = sample_table;  // IMAGE_SAMPLE
                table[28] = sample_table;  // IMAGE_SAMPLE_D
                table[30] = sample_table;  // IMAGE_SAMPLE_B
                table[32] = sample_table;  // IMAGE_SAMPLE_C
                table[33] = sample_table;  // IMAGE_SAMPLE_C_D
                table[35] = sample_table;  // IMAGE_SAMPLE_C_B
                table[37] = sample_table;  // IMAGE_SAMPLE_O
                table[38] = sample_table;  // IMAGE_SAMPLE_D_O
                table[40] = sample_table;  // IMAGE_SAMPLE_B_O
                table[42] = sample_table;  // IMAGE_SAMPLE_C_O
                table[43] = sample_table;  // IMAGE_SAMPLE_C_D_O
                table[45] = sample_table;  // IMAGE_SAMPLE_C_B_O
                table[57] = sample_table;  // IMAGE_SAMPLE_D_G16
                table[58] = sample_table;  // IMAGE_SAMPLE_C_D_G16
                table[59] = sample_table;  // IMAGE_SAMPLE_D_O_G16
                table[60] = sample_table;  // IMAGE_SAMPLE_C_D_O_G16

                // Define table for sample_l_cl instructions.
                std::shared_ptr<DimensionToAcntTable> sample_l_cl_table = std::make_shared<DimensionToAcntTable>();
                sample_l_cl_table->emplace(Dimension::k1d, 1);
                sample_l_cl_table->emplace(Dimension::k2d, 2);
                sample_l_cl_table->emplace(Dimension::k3d, 3);
                sample_l_cl_table->emplace(Dimension::kCube, 3);
                sample_l_cl_table->emplace(Dimension::k1dArray, 2);
                sample_l_cl_table->emplace(Dimension::k2dArray, 3);
                table[29] = sample_l_cl_table;  // IMAGE_SAMPLE_L
                table[31] = sample_l_cl_table;  // IMAGE_SAMPLE_LZ
                table[34] = sample_l_cl_table;  // IMAGE_SAMPLE_C_L
                table[36] = sample_l_cl_table;  // IMAGE_SAMPLE_C_LZ
                table[39] = sample_l_cl_table;  // IMAGE_SAMPLE_L_O
                table[41] = sample_l_cl_table;  // IMAGE_SAMPLE_LZ_O
                table[44] = sample_l_cl_table;  // IMAGE_SAMPLE_C_L_O
                table[46] = sample_l_cl_table;  // IMAGE_SAMPLE_C_LZ_O
                table[64] = sample_l_cl_table;  // IMAGE_SAMPLE_CL
                table[65] = sample_l_cl_table;  // IMAGE_SAMPLE_D_CL
                table[66] = sample_l_cl_table;  // IMAGE_SAMPLE_B_CL
                table[67] = sample_l_cl_table;  // IMAGE_SAMPLE_C_CL
                table[68] = sample_l_cl_table;  // IMAGE_SAMPLE_C_D_CL
                table[69] = sample_l_cl_table;  // IMAGE_SAMPLE_C_B_CL
                table[70] = sample_l_cl_table;  // IMAGE_SAMPLE_CL_O
                table[71] = sample_l_cl_table;  // IMAGE_SAMPLE_D_CL_O
                table[72] = sample_l_cl_table;  // IMAGE_SAMPLE_B_CL_O
                table[73] = sample_l_cl_table;  // IMAGE_SAMPLE_C_CL_O
                table[74] = sample_l_cl_table;  // IMAGE_SAMPLE_C_D_CL_O
                table[75] = sample_l_cl_table;  // IMAGE_SAMPLE_C_B_CL_O
                table[84] = sample_l_cl_table;  // IMAGE_SAMPLE_C_D_CL_G16
                table[85] = sample_l_cl_table;  // IMAGE_SAMPLE_D_CL_O_G16
                table[86] = sample_l_cl_table;  // IMAGE_SAMPLE_C_D_CL_O_G16
                table[95] = sample_l_cl_table;  // IMAGE_SAMPLE_D_CL_G16

                // Define table for gather4 instruction.
                std::shared_ptr<DimensionToAcntTable> gather4_table = std::make_shared<DimensionToAcntTable>();
                gather4_table->emplace(Dimension::k2d, 1);
                gather4_table->emplace(Dimension::k2dArray, 2);
                gather4_table->emplace(Dimension::kCube, 2);
                table[47]  = gather4_table;  // IMAGE_GATHER4
                table[49]  = gather4_table;  // IMAGE_GATHER4_B
                table[51]  = gather4_table;  // IMAGE_GATHER4_C
                table[53]  = gather4_table;  // IMAGE_GATHER4_O
                table[100] = gather4_table;  // IMAGE_GATHER4_C_B
                table[144] = gather4_table;  // IMAGE_GATHER4H

                // Define table for gather4 instructions with lod and clamp.
                std::shared_ptr<DimensionToAcntTable> gather4_l_cl_table = std::make_shared<DimensionToAcntTable>();
                gather4_l_cl_table->emplace(Dimension::k2d, 2);
                gather4_l_cl_table->emplace(Dimension::k2dArray, 3);
                gather4_l_cl_table->emplace(Dimension::kCube, 3);
                table[48]  = gather4_l_cl_table;  // IMAGE_GATHER4_L
                table[50]  = gather4_l_cl_table;  // IMAGE_GATHER4_LZ
                table[52]  = gather4_l_cl_table;  // IMAGE_GATHER4_C_LZ
                table[54]  = gather4_l_cl_table;  // IMAGE_GATHER4_LZ_O
                table[55]  = gather4_l_cl_table;  // IMAGE_GATHER4_C_LZ_O
                table[96]  = gather4_l_cl_table;  // IMAGE_GATHER4_CL
                table[97]  = gather4_l_cl_table;  // IMAGE_GATHER4_B_CL
                table[98]  = gather4_l_cl_table;  // IMAGE_GATHER4_C_CL
                table[99]  = gather4_l_cl_table;  // IMAGE_GATHER4_C_L
                table[101] = gather4_l_cl_table;  // IMAGE_GATHER4_C_B_CL

                // Address field information.
                addr_fields[current_architecture] = {
                    "VADDR", "VADDRA", "VADDRB", "VADDRC", "VADDRD"};
            }

            // Generates Acnt information for RDNA 4 architecture.
            void GenerateRdna4Table()
            {
                current_architecture = GpuArchitecture::kRdna4;
                tables[current_architecture]         = std::make_unique<AcntTable>();
                AcntTable& table                     = *tables[current_architecture];

                // Define table for msaa_load instructions.
                std::shared_ptr<DimensionToAcntTable> load_store_table = std::make_shared<DimensionToAcntTable>();
                load_store_table->emplace(Dimension::k1d, 0);
                load_store_table->emplace(Dimension::k2d, 1);
                load_store_table->emplace(Dimension::k3d, 2);
                load_store_table->emplace(Dimension::kCube, 2);
                load_store_table->emplace(Dimension::k1dArray, 1);
                load_store_table->emplace(Dimension::k2dArray, 2);
                load_store_table->emplace(Dimension::k2dMsaa, 2);
                load_store_table->emplace(Dimension::k2dMsaaArray, 3);
                table[0]  = load_store_table;  // IMAGE_LOAD
                table[2]  = load_store_table;  // IMAGE_LOAD_PCK
                table[3]  = load_store_table;  // IMAGE_LOAD_PCK_SGN
                table[6]  = load_store_table;  // IMAGE_STORE
                table[8]  = load_store_table;  // IMAGE_STORE_PCK

                // Define table for atomic instructions.
                std::shared_ptr<DimensionToAcntTable> atomic_table = std::make_shared<DimensionToAcntTable>();
                atomic_table->emplace(Dimension::k1d, 0);
                atomic_table->emplace(Dimension::k2d, 1);
                atomic_table->emplace(Dimension::k3d, 2);
                atomic_table->emplace(Dimension::k1dArray, 1);
                atomic_table->emplace(Dimension::k2dArray, 2);
                atomic_table->emplace(Dimension::k2dMsaa, 2);
                atomic_table->emplace(Dimension::k2dMsaaArray, 3);
                table[10]  = atomic_table;  // IMAGE_ATOMIC_SWAP
                table[11]  = atomic_table;  // IMAGE_ATOMIC_CMPSWAP
                table[12]  = atomic_table;  // IMAGE_ATOMIC_ADD_UINT
                table[13]  = atomic_table;  // IMAGE_ATOMIC_SUB_UINT
                table[14]  = atomic_table;  // IMAGE_ATOMIC_MIN_INT
                table[15]  = atomic_table;  // IMAGE_ATOMIC_MIN_UINT
                table[16]  = atomic_table;  // IMAGE_ATOMIC_MAX_INT
                table[17]  = atomic_table;  // IMAGE_ATOMIC_MAX_UINT
                table[18]  = atomic_table;  // IMAGE_ATOMIC_AND
                table[19]  = atomic_table;  // IMAGE_ATOMIC_OR
                table[20]  = atomic_table;  // IMAGE_ATOMIC_XOR
                table[21]  = atomic_table;  // IMAGE_ATOMIC_INC_UINT
                table[22]  = atomic_table;  // IMAGE_ATOMIC_DEC_UINT
                table[131] = atomic_table;  // IMAGE_ATOMIC_ADD_FLT
                table[132] = atomic_table;  // IMAGE_ATOMIC_MIN_FLT
                table[133] = atomic_table;  // IMAGE_ATOMIC_MAX_FLT
                table[134] = atomic_table;  // IMAGE_ATOMIC_PK_ADD_F16
                table[135] = atomic_table;  // IMAGE_ATOMIC_PK_ADD_BF16

                // Define table for load_mip/store_mip instructions.
                std::shared_ptr<DimensionToAcntTable> ls_mip_table = std::make_shared<DimensionToAcntTable>();
                ls_mip_table->emplace(Dimension::k1d, 1);
                ls_mip_table->emplace(Dimension::k2d, 2);
                ls_mip_table->emplace(Dimension::k3d, 3);
                ls_mip_table->emplace(Dimension::kCube, 3);
                ls_mip_table->emplace(Dimension::k1dArray, 2);
                ls_mip_table->emplace(Dimension::k2dArray, 3);
                table[1] = ls_mip_table;  // IMAGE_LOAD_MIP
                table[4] = ls_mip_table;  // IMAGE_LOAD_MIP_PCK
                table[5] = ls_mip_table;  // IMAGE_LOAD_MIP_PCK_SGN
                table[7] = ls_mip_table;  // IMAGE_STORE_MIP
                table[9] = ls_mip_table;  // IMAGE_STORE_MIP_PCK

                // Define table for sample instructions.
                std::shared_ptr<DimensionToAcntTable> sample_table = std::make_shared<DimensionToAcntTable>();
                sample_table->emplace(Dimension::k1d, 0);
                sample_table->emplace(Dimension::k2d, 1);
                sample_table->emplace(Dimension::k3d, 2);
                sample_table->emplace(Dimension::kCube, 2);
                sample_table->emplace(Dimension::k1dArray, 1);
                sample_table->emplace(Dimension::k2dArray, 2);
                table[27] = sample_table;  // IMAGE_SAMPLE
                table[28] = sample_table;  // IMAGE_SAMPLE_D
                table[30] = sample_table;  // IMAGE_SAMPLE_B
                table[32] = sample_table;  // IMAGE_SAMPLE_C
                table[33] = sample_table;  // IMAGE_SAMPLE_C_D
                table[35] = sample_table;  // IMAGE_SAMPLE_C_B
                table[37] = sample_table;  // IMAGE_SAMPLE_O
                table[38] = sample_table;  // IMAGE_SAMPLE_D_O
                table[40] = sample_table;  // IMAGE_SAMPLE_B_O
                table[42] = sample_table;  // IMAGE_SAMPLE_C_O
                table[43] = sample_table;  // IMAGE_SAMPLE_C_D_O
                table[45] = sample_table;  // IMAGE_SAMPLE_C_B_O
                table[57] = sample_table;  // IMAGE_SAMPLE_D_G16
                table[58] = sample_table;  // IMAGE_SAMPLE_C_D_G16
                table[59] = sample_table;  // IMAGE_SAMPLE_D_O_G16
                table[60] = sample_table;  // IMAGE_SAMPLE_C_D_O_G16


                // Define table for sample_l_cl instructions.
                std::shared_ptr<DimensionToAcntTable> sample_l_cl_table = std::make_shared<DimensionToAcntTable>();
                sample_l_cl_table->emplace(Dimension::k1d, 1);
                sample_l_cl_table->emplace(Dimension::k2d, 2);
                sample_l_cl_table->emplace(Dimension::k3d, 3);
                sample_l_cl_table->emplace(Dimension::kCube, 3);
                sample_l_cl_table->emplace(Dimension::k1dArray, 2);
                sample_l_cl_table->emplace(Dimension::k2dArray, 3);
                table[29] = sample_l_cl_table;  // IMAGE_SAMPLE_L
                table[31] = sample_l_cl_table;  // IMAGE_SAMPLE_LZ
                table[34] = sample_l_cl_table;  // IMAGE_SAMPLE_C_L
                table[36] = sample_l_cl_table;  // IMAGE_SAMPLE_C_LZ
                table[39] = sample_l_cl_table;  // IMAGE_SAMPLE_L_O
                table[41] = sample_l_cl_table;  // IMAGE_SAMPLE_LZ_O
                table[44] = sample_l_cl_table;  // IMAGE_SAMPLE_C_L_O
                table[46] = sample_l_cl_table;  // IMAGE_SAMPLE_C_LZ_O
                table[64] = sample_l_cl_table;  // IMAGE_SAMPLE_CL
                table[65] = sample_l_cl_table;  // IMAGE_SAMPLE_D_CL
                table[66] = sample_l_cl_table;  // IMAGE_SAMPLE_B_CL
                table[67] = sample_l_cl_table;  // IMAGE_SAMPLE_C_CL
                table[68] = sample_l_cl_table;  // IMAGE_SAMPLE_C_D_CL
                table[69] = sample_l_cl_table;  // IMAGE_SAMPLE_C_B_CL
                table[70] = sample_l_cl_table;  // IMAGE_SAMPLE_CL_O
                table[71] = sample_l_cl_table;  // IMAGE_SAMPLE_D_CL_O
                table[72] = sample_l_cl_table;  // IMAGE_SAMPLE_B_CL_O
                table[73] = sample_l_cl_table;  // IMAGE_SAMPLE_C_CL_O
                table[74] = sample_l_cl_table;  // IMAGE_SAMPLE_C_D_CL_O
                table[75] = sample_l_cl_table;  // IMAGE_SAMPLE_C_B_CL_O
                table[84] = sample_l_cl_table;  // IMAGE_SAMPLE_C_D_CL_G16
                table[85] = sample_l_cl_table;  // IMAGE_SAMPLE_D_CL_O_G16
                table[86] = sample_l_cl_table;  // IMAGE_SAMPLE_C_D_CL_O_G16
                table[95] = sample_l_cl_table;  // IMAGE_SAMPLE_D_CL_G16

                // Define table for gather4 instruction.
                std::shared_ptr<DimensionToAcntTable> gather4_table = std::make_shared<DimensionToAcntTable>();
                gather4_table->emplace(Dimension::k2d, 1);
                gather4_table->emplace(Dimension::k2dArray, 2);
                gather4_table->emplace(Dimension::kCube, 2);
                table[47]  = gather4_table;  // IMAGE_GATHER4
                table[49]  = gather4_table;  // IMAGE_GATHER4_B
                table[51]  = gather4_table;  // IMAGE_GATHER4_C
                table[53]  = gather4_table;  // IMAGE_GATHER4_O
                table[100] = gather4_table;  // IMAGE_GATHER4_C_B
                table[144] = gather4_table;  // IMAGE_GATHER4H

                // Define table for gather4 instructions with lod and clamp.
                std::shared_ptr<DimensionToAcntTable> gather4_l_cl_table = std::make_shared<DimensionToAcntTable>();
                gather4_l_cl_table->emplace(Dimension::k2d, 2);
                gather4_l_cl_table->emplace(Dimension::k2dArray, 3);
                gather4_l_cl_table->emplace(Dimension::kCube, 3);
                table[48]  = gather4_l_cl_table;  // IMAGE_GATHER4_L
                table[50]  = gather4_l_cl_table;  // IMAGE_GATHER4_LZ
                table[52]  = gather4_l_cl_table;  // IMAGE_GATHER4_C_LZ
                table[54]  = gather4_l_cl_table;  // IMAGE_GATHER4_LZ_O
                table[55]  = gather4_l_cl_table;  // IMAGE_GATHER4_C_LZ_O
                table[96]  = gather4_l_cl_table;  // IMAGE_GATHER4_CL
                table[97]  = gather4_l_cl_table;  // IMAGE_GATHER4_B_CL
                table[98]  = gather4_l_cl_table;  // IMAGE_GATHER4_C_CL
                table[99]  = gather4_l_cl_table;  // IMAGE_GATHER4_C_L
                table[101] = gather4_l_cl_table;  // IMAGE_GATHER4_C_B_CL

                // Address field information.
                addr_fields[current_architecture] = {
                    "VADDR0", "VADDR1", "VADDR2", "VADDR3", "VADDR4"};
            }
            AcntHandler() = default;

            int32_t GetAcnt(uint64_t opcode, uint64_t dim) const
            {
                assert(current_architecture != GpuArchitecture::kUnknown);
                assert(tables.count(current_architecture) > 0);
                AcntTable&  table      = *tables.at(current_architecture);
                int32_t     ret        = -1;
                const auto& table_iter = table.find(opcode);
                if (table_iter != table.end())
                {
                    const auto& acnt_iter = table_iter->second->find(static_cast<Dimension>(dim));
                    if (acnt_iter != table_iter->second->end())
                    {
                        ret = acnt_iter->second;
                    }
                }
                return ret;
            }

            // Maps opcode to the DIM to Acnt table.
            using AcntTable = std::unordered_map<uint64_t, std::shared_ptr<DimensionToAcntTable>>;
            std::unordered_map<GpuArchitecture, std::unique_ptr<AcntTable>> tables;

            // Arch ID to address fields information.
            std::unordered_map<GpuArchitecture, std::vector<std::string>> addr_fields;

            // Architecture.
            GpuArchitecture current_architecture = GpuArchitecture::kUnknown;
        };

        static int32_t GetAcnt(const std::vector<uint32_t>& machine_code,
                               const MicrocodeFormat& microcode_format,
                               const AcntHandler& mimg_acnt_table,
                               std::vector<std::string>& error_log)
        {
            uint64_t opcode = 0;
            uint64_t dim    = 0;
            GetFieldIterator(machine_code, "OP", microcode_format, opcode, error_log);
            GetFieldIterator(machine_code, "DIM", microcode_format, dim, error_log);
            return mimg_acnt_table.GetAcnt(opcode, dim);
        }

        static std::string GetNameAsList(const std::vector<uint32_t>& machine_code,
                                         const MicrocodeFormat& microcode_format,
                                         const std::vector<PredefinedValue>& predefined_values,
                                         const AcntHandler& mimg_acnt_table,
                                         const GpuArchitecture& architecture,
                                         std::vector<std::string>& error_log)
        {
            std::string ret = "[";
            int32_t acnt = mimg_workaround::GetAcnt(machine_code, microcode_format, mimg_acnt_table, error_log);
            if (acnt > 0)
            {
                for (uint32_t i = 0; i <= static_cast<uint32_t>(acnt); i++)
                {
                    std::string addr_field = mimg_acnt_table.addr_fields.at(architecture)[i];
                    uint64_t    addr_reg   = 0;
                    GetFieldIterator(machine_code, addr_field, microcode_format, addr_reg, error_log);
                    const auto& predefined_value_iterator =
                        std::find_if(predefined_values.begin(), predefined_values.end(), [&](const PredefinedValue& predefined_value) {
                            return predefined_value.value == addr_reg;
                        });
                    if (predefined_value_iterator != predefined_values.end())
                    {
                        ret += predefined_value_iterator->name + ", ";
                    }
                }
                if (ret.size() > 2)
                {
                    ret.erase(ret.size() - 2);
                }
                ret += "]";
            }
            else
            {
                error_log.push_back(kStringErrorFailedToGetAddressCount);
            }
            return ret;
        }
    }  // namespace mimg_workaround

    class MachineCodeStream
    {
    public:
        MachineCodeStream() = default;
        void Init(const std::vector<uint32_t>& machine_code_stream)
        {
            for (uint32_t machine_code : machine_code_stream)
            {
                machine_code_stream_.push_back(machine_code);
            }
            is_empty_ = machine_code_stream_.empty();
        }

        uint32_t GetNextDword(std::vector<std::string>& warnings)
        {
            uint32_t ret = 0;
            if (!is_empty_)
            {
                ret = machine_code_stream_.front();
                machine_code_stream_.pop_front();
                is_empty_ = machine_code_stream_.empty();
            }
            else
            {
                warnings.push_back(kStringWarningEmptyStream);
            }
            return ret;
        }

        bool IsEmpty() const
        {
            return is_empty_;
        }

    private:
        std::deque<uint32_t> machine_code_stream_;
        bool                 is_empty_ = true;
    };

    static void RetrieveFieldInfo(const std::vector<uint32_t>& working_dwords,
                                  const std::vector<Field>&    bitmap,
                                  InstructionInfo&             instruction_info,
                                  std::vector<std::string>&    log)
    {
        // Get the fields info.
        for (const auto& field : bitmap)
        {
            Range range;
            if (AmdIsaUtility::GetRange(field, range, 0))
            {
                instruction_info.encoding_fields.push_back(EncodingField());
                auto&    encoding_field = instruction_info.encoding_fields.back();
                uint64_t bit_count      = range.bit_count;
                uint64_t bit_offset     = range.bit_offset;
                uint64_t dword_index    = bit_offset / 32;

                encoding_field.bit_count   = bit_count;
                encoding_field.bit_offset  = bit_offset;
                encoding_field.field_name  = field.name;
                encoding_field.field_value = GetFieldValue(field, working_dwords, log);
            }
            else
            {
                log.push_back(kStringErrorEmptyRange);
                assert(false);
            }
        }
    }

    static uint64_t GetFieldMask(const Field& field)
    {
        uint64_t mask = 0;

        for (const auto& range : field.ranges)
        {
            uint64_t current_mask = static_cast<uint64_t>((1 << range.bit_count) - 1) << range.bit_offset;
            mask |= current_mask;
        }

        return mask;
    }

    static std::shared_ptr<Encoding> GetEncodingIterator(const IdToEncoding& id_to_encodings, uint64_t first_dword, const IsaSpec& spec_data)
    {
        // Go over each encoding in the passed spec data.
        std::shared_ptr<Encoding> encoding          = nullptr;
        bool                      is_encoding_found = false;
        for (auto encoding_iterator = spec_data.encodings.begin(); !is_encoding_found && encoding_iterator != spec_data.encodings.end(); ++encoding_iterator)
        {
            // Dereference the iterator.
            const auto& single_encoding = *encoding_iterator;

            // Mask the encoding identifier bits.
            uint64_t masked_machine_code = first_dword & (single_encoding.mask | single_encoding.opcode_mask | single_encoding.seg_mask);
            if (id_to_encodings.find(masked_machine_code) != id_to_encodings.end())
            {
                encoding = id_to_encodings.at(masked_machine_code);
                if (encoding == nullptr)
                {
                    break;
                }
                if (encoding->name == single_encoding.name)
                {
                    is_encoding_found = true;
                }
            }

            // If the instruction is dual, pick the encoding of the second instruction if its encoding size is bigger.
            if (is_encoding_found)
            {
                auto field_iter = std::find_if(single_encoding.microcode_format.bit_map.begin(),
                                               single_encoding.microcode_format.bit_map.end(),
                                               [&](const Field& field) { return field.name == "OPY"; });

                if (field_iter != single_encoding.microcode_format.bit_map.end())
                {
                    uint64_t opy_mask                = GetFieldMask(*field_iter);
                    uint64_t masked_machine_code_opy = first_dword & (single_encoding.mask | opy_mask | single_encoding.seg_mask);
                    if (id_to_encodings.find(masked_machine_code_opy) != id_to_encodings.end())
                    {
                        std::shared_ptr<Encoding> encoding_new = id_to_encodings.at(masked_machine_code_opy);
                        if (encoding_new == nullptr)
                        {
                            break;
                        }
                        if (encoding->bit_count < encoding_new->bit_count)
                        {
                            encoding = encoding_new;
                        }
                    }
                }
            }
        }

        return encoding;
    }

    // Checks if the operand with the specific name present in the instruction encoding.
    static bool IsOperandPresent(const InstructionEncoding& instruction_encoding, const std::string& operand_name)
    {
        const auto& operand_iter = std::find_if(instruction_encoding.operands.begin(), instruction_encoding.operands.end(), [&](const Operand& operand) {
            return operand.encoding_field_name == operand_name;
        });
        return operand_iter != instruction_encoding.operands.end();
    }

    // Gets the iterator to the field from the bitmap with a specific name.
    static FieldIterator GetFieldIterator(const MicrocodeFormat& microcode, const std::string& field_name)
    {
        return std::find_if(microcode.bit_map.begin(), microcode.bit_map.end(), [&](const Field& field) { return field.name == field_name; });
    }

    static std::string GetNameAsRegisterRange(const std::string& operand_name, uint32_t operand_size)
    {
        std::stringstream reg_name_formatter;
        if (!operand_name.empty())
        {
            const char     base_reg_name = operand_name[0];
            const uint64_t reg_id        = AmdIsaUtility::StringToUnsignedInt(operand_name.substr(1));
            const uint32_t dword_num     = operand_size / kDwordSize;

            reg_name_formatter << base_reg_name << "[" << reg_id << ":" << reg_id + dword_num - 1 << "]";
        }
        else
        {
            assert(false);
        }
        return reg_name_formatter.str();
    }

    static const std::map<std::string, std::string> kVersionMap = {
        {"uc_version_gfx10", "RDNA1"},
        {"uc_version_gfx11", "RDNA3"},
        {"uc_version_gfx12", "RDNA4"}
    };
    static std::string GeneratePartitionedOperand(const std::string inst_name, const amdisa::MicrocodeFormat& microcode_format, const uint32_t field_value, std::vector<std::string>& log)
    {
        std::stringstream ret;
        ret << "{ ";

        // Package field value for the use with GetFieldValue function.
        std::vector<uint32_t> working_dword;
        working_dword.push_back(field_value);
        std::map<std::string, std::string> partitioned_values;
        std::map<std::string, uint64_t>    partitioned_int_values;
        std::map<std::string, uint64_t>    partitioned_field_sizes;
        std::stringstream                  bitmap_to_str;
        for (const auto& field : microcode_format.bit_map)
        {
            ret << field.name << ":";
            uint64_t subvalue             = GetFieldValue(field, working_dword, log);
            bool     has_predefined_value = false;
            if (field.predefined_values.size() > 0)
            {
                const auto& predefined_value_iterator =
                    std::find_if(field.predefined_values.begin(), field.predefined_values.end(), [&](const amdisa::PredefinedValue& predefined_value) {
                        return static_cast<uint64_t>(predefined_value.value) == subvalue;
                    });
                has_predefined_value = (predefined_value_iterator != field.predefined_values.end());
                if (has_predefined_value)
                {
                    ret << predefined_value_iterator->name << "; ";
                    partitioned_values[field.name] = predefined_value_iterator->name;
                }
            }

            if (!has_predefined_value)
            {
                ret << subvalue << "; ";
                partitioned_values[field.name] = std::to_string(subvalue);
            }

            // Save values for reformatting.
            partitioned_int_values[field.name] = subvalue;
            uint32_t total_bit_count           = 0;
            for (const auto& range : field.ranges)
            {
                total_bit_count += range.bit_count;
            }
            partitioned_field_sizes[field.name] = total_bit_count;
            if (!bitmap_to_str.str().empty())
            {
                bitmap_to_str << " | ";
            }
            bitmap_to_str << field.name << "=" << (has_predefined_value ? partitioned_values[field.name] : std::to_string(subvalue));
        }
        ret << "}";

        // Reformat for better readability.
        // 1. Constant cases.
        if (partitioned_values.size() == 1 && partitioned_values.count("VALUE") > 0)
        {
            ret.str("");
            ret << "0x" << std::hex << std::stol(partitioned_values["VALUE"]);
        }
        // 2. s_version instruction case.
        else if ((inst_name.compare("S_VERSION") == 0) && partitioned_values.size() == 4 && partitioned_values.count("VERSION") > 0 &&
                 partitioned_values.count("W32") > 0)
        {
            ret.str("");
            std::string version = partitioned_values.at("VERSION");
            if (kVersionMap.count(version) > 0)
            {
                version = kVersionMap.at(version);
            }
            ret << "Architecture=" << version << " | WaveSize=";
            if (partitioned_values.at("W32") == "1")
            {
                // Wavesize=32.
                ret << 32;
            }
            else
            {
                // Wavesize=64.
                ret << 64;
            }
        }
        // 3. s_waitcnt instruction case.
        else if ((inst_name.compare("S_WAITCNT") == 0) && partitioned_values.size() == 3 && partitioned_values.count("LGKM") > 0)
        {
            std::stringstream counter_stream;
            for (const auto& value_info : partitioned_int_values)
            {
                uint64_t field_size = partitioned_field_sizes.at(value_info.first);
                uint64_t value      = partitioned_int_values.at(value_info.first);
                uint64_t max_value  = (1ULL << field_size) - 1;

                if (value < max_value)
                {
                    counter_stream << value_info.first << "cnt==" << value_info.second << ", ";
                }
            }
            std::string counter_str = counter_stream.str();
            if (counter_str.size() >= 2)
            {
                counter_str.erase(counter_str.size() - 2);
            }
            ret.str("");
            ret << counter_str;
        }
        // 4. s_waitcnt_depctr instruction case.
        else if ((inst_name.compare("S_WAITCNT_DEPCTR") == 0) && partitioned_values.size() == 7)
        {
            std::stringstream counter_stream;
            counter_stream << "{ ";
            for (auto value_info = partitioned_int_values.begin(); value_info != partitioned_int_values.end(); value_info++)
            {
                uint64_t field_size = partitioned_field_sizes.at(value_info->first);
                uint64_t value      = partitioned_int_values.at(value_info->first);
                uint64_t max_value  = (1ULL << field_size) - 1;

                if (value < max_value)
                {
                    counter_stream << ((counter_stream.str().size() > 2 || value_info == std::prev(partitioned_int_values.end())) ? ", " : "");
                    counter_stream << value_info->first << ":" << value_info->second;
                }
            }
            counter_stream << " }";
            ret.str("");
            ret << counter_stream.str();
        }
        // 5. s_clause and s_delay_alu instruction case.
        else if ((inst_name.compare("S_CLAUSE") == 0) || (inst_name.compare("S_DELAY_ALU") == 0))
        {
            ret.str("");
            ret << bitmap_to_str.str();
        }

        return ret.str();
    }

    static bool ExtractModifiers(const std::vector<uint32_t>& machine_code,
                                 const Encoding&              encoding,
                                 std::vector<OperandModifer>& operand_modifiers,
                                 std::string&                 err_message,
                                 std::vector<std::string>&    log)
    {
        bool is_success = false;

        auto modifier_list_iterator = kEncodingToOperandModifiers.begin();
        bool is_found               = false;
        while (!is_found && modifier_list_iterator != kEncodingToOperandModifiers.end())
        {
            if (encoding.name == modifier_list_iterator->first)
            {
                is_found = true;
            }
            else
            {
                modifier_list_iterator++;
            }
        }

        if (modifier_list_iterator != kEncodingToOperandModifiers.end())
        {
            bool        should_abort        = false;
            const auto& modifiers           = modifier_list_iterator->second;
            uint32_t    processed_modifiers = 0;
            for (auto modifier_name_iterator = modifiers.begin(); !should_abort && modifier_name_iterator != modifiers.end(); ++modifier_name_iterator)
            {
                auto& modifier_name = *modifier_name_iterator;

                // Get modifier value.
                uint64_t    modifier_value = 0;
                const auto& field_iterator = GetFieldIterator(machine_code, modifier_name, encoding.microcode_format, modifier_value, log);

                // Save the modifier.
                if (field_iterator != encoding.microcode_format.bit_map.end())
                {
                    operand_modifiers.push_back(OperandModifer());
                    auto& instruction_modifier         = operand_modifiers.back();
                    instruction_modifier.modifier_name = modifier_name;
                    instruction_modifier.value         = static_cast<uint32_t>(modifier_value);
                }
                else
                {
                    err_message  = kStringErrorModiferFieldNotFound;
                    should_abort = true;
                }

                // Success condition.
                ++processed_modifiers;
                if (processed_modifiers == modifiers.size())
                {
                    is_success = true;
                }
            }
        }
        else
        {
            // Apply extraction to only defined encodings. Return true for encodings
            // with no modifiers.
            is_success = true;
        }

        return is_success;
    }

    static InstructionPtrs GetInstructionPtrs(uint32_t                        architecture_id,
                                              const IdToInstruction&          id_to_inst,
                                              const IdToInstructionEncodings& id_to_inst_enc,
                                              const Encoding&                 encoding,
                                              const EncodingConditionHandler& condition_handler,
                                              uint32_t                        first_dword,
                                              uint32_t                        second_dword,
                                              bool                            is_vopdx,
                                              bool                            is_vopdy)
    {
        InstructionPtrs found_ptrs;

        uint64_t masked_machine_code = 0;
        uint64_t opcode_mask         = 0;
        if (is_vopdx)
        {
            assert(!is_vopdy);
            const auto& opx_iter = GetFieldIterator(encoding.microcode_format, "OPX");
            assert(opx_iter != encoding.microcode_format.bit_map.end());
            opcode_mask = GetFieldMask(*opx_iter);
        }
        else if (is_vopdy)
        {
            assert(!is_vopdx);
            const auto& opy_iter = GetFieldIterator(encoding.microcode_format, "OPY");
            assert(opy_iter != encoding.microcode_format.bit_map.end());
            opcode_mask = GetFieldMask(*opy_iter);
        }
        else
        {
            opcode_mask = encoding.opcode_mask;
        }

        masked_machine_code = first_dword & (encoding.mask | opcode_mask | encoding.seg_mask);

        // Get instruction pointer.
        if (id_to_inst.find(masked_machine_code) != id_to_inst.end())
        {
            found_ptrs.instruction_ptr = id_to_inst.at(masked_machine_code);
        }

        // Get instruction encoding pointer.
        if (id_to_inst_enc.find(masked_machine_code) != id_to_inst_enc.end())
        {
            // FIXME: Make runtime calculation on the field values.
            const auto& inst_enc_vec = id_to_inst_enc.at(masked_machine_code);
            if (inst_enc_vec.size() == 1)
            {
                found_ptrs.instruction_encoding_ptr = inst_enc_vec[0];
            }
            else
            {
                uint32_t matched_encodings = 0;
                for (const auto& inst_enc_ptr : inst_enc_vec)
                {
                    if (condition_handler.arch_conditions_.find(architecture_id) != condition_handler.arch_conditions_.end())
                    {
                        const auto& conditions    = condition_handler.arch_conditions_.at(architecture_id);
                        std::string encoding_name = inst_enc_ptr->name;
                        if (encoding_name.find("ENC_") == 0)
                        {
                            encoding_name = encoding_name.substr(4);
                        }
                        encoding_name += "_" + inst_enc_ptr->condition_name;

                        if (is_vopdx)
                        {
                            encoding_name = "VOPDX_" + encoding_name;
                        }
                        else if (is_vopdy)
                        {
                            encoding_name = "VOPDY_" + encoding_name;
                        }

                        if (conditions.find(encoding_name) != conditions.end())
                        {
                            auto& IsEncodingMatch = conditions.at(encoding_name);
                            if (IsEncodingMatch((static_cast<uint64_t>(second_dword) << 32) | first_dword))
                            {
                                found_ptrs.instruction_encoding_ptr = inst_enc_ptr;
                                matched_encodings++;
                            }
                        }
                    }
                }
                assert(matched_encodings == 1);
            }
        }

        return found_ptrs;
    }

    static std::map<std::string, FunctionalGroups> FunctionalGroupsNameMap = {{"SALU", FunctionalGroups::kFunctionalGroupSalu},
                                                                             {"SMEM", FunctionalGroups::kFunctionalGroupSmem},
                                                                             {"VALU", FunctionalGroups::kFunctionalGroupValu},
                                                                             {"VMEM", FunctionalGroups::kFunctionalGroupVmem},
                                                                             {"EXPORT", FunctionalGroups::kFunctionalGroupExport},
                                                                             {"BRANCH", FunctionalGroups::kFunctionalGroupBranch},
                                                                             {"MESSAGE", FunctionalGroups::kFunctionalGroupMessage},
                                                                             {"WAVE_CONTROL", FunctionalGroups::kFunctionalGroupWaveControl},
                                                                             {"TRAP", FunctionalGroups::kFunctionalGroupTrap}};

    static FunctionalGroups FunctionalGroupNameToEnum(std::string functional_group)
    {
        FunctionalGroups functional_group_enum = FunctionalGroups::kFunctionalGroupUnknown;
        if (FunctionalGroupsNameMap.find(functional_group) != FunctionalGroupsNameMap.end())
        {
            functional_group_enum = FunctionalGroupsNameMap[functional_group];
        }
        return functional_group_enum;
    }

    static std::map<std::string, FunctionalSubgroups> FunctionalSubgroupsNameMap = {{"FLOATING_POINT", FunctionalSubgroups::kFunctionalSubgroupFloatingPoint},
                                                                                   {"BUFFER", FunctionalSubgroups::kFunctionalSubgroupBuffer},
                                                                                   {"TEXTURE", FunctionalSubgroups::kFunctionalSubgroupTexture},
                                                                                   {"LOAD", FunctionalSubgroups::kFunctionalSubgroupLoad},
                                                                                   {"STORE", FunctionalSubgroups::kFunctionalSubgroupStore},
                                                                                   {"SAMPLE", FunctionalSubgroups::kFunctionalSubgroupSample},
                                                                                   {"BVH", FunctionalSubgroups::kFunctionalSubgroupBvh},
                                                                                   {"ATOMIC", FunctionalSubgroups::kFunctionalSubgroupAtomic},
                                                                                   {"FLAT", FunctionalSubgroups::kFunctionalSubgroupFlat},
                                                                                   {"DATA_SHARE", FunctionalSubgroups::kFunctionalSubgroupDataShare},
                                                                                   {"STATIC", FunctionalSubgroups::kFunctionalSubgroupStatic},
                                                                                   {"MFMA", FunctionalSubgroups::kFunctionalSubgroupMFMA},
                                                                                   {"WMMA", FunctionalSubgroups::kFunctionalSubgroupWMMA},
                                                                                   {"TRANSCENDENTAL", FunctionalSubgroups::kFunctionalSubgroupTranscendental}};

    static FunctionalSubgroups FunctionalSubgroupNameToEnum(std::string functional_subgroup)
    {
        FunctionalSubgroups functional_subgroup_enum = FunctionalSubgroups::kFunctionalSubgroupUnknown;
        if (FunctionalSubgroupsNameMap.find(functional_subgroup) != FunctionalSubgroupsNameMap.end())
        {
            functional_subgroup_enum = FunctionalSubgroupsNameMap[functional_subgroup];
        }
        return functional_subgroup_enum;
    }

    static void GetFunctionalGroupSubgroupInfo(InstructionInfo& info, const std::string& functional_group, const std::vector<std::string>& functional_subgroup)
    {
        // Assign functional group enum
        info.functional_group_subgroup_info.isa_functional_group = FunctionalGroupNameToEnum(functional_group);

        // Assign functional subgroup enum
        for (const auto& subgroup : functional_subgroup)
        {
            info.functional_group_subgroup_info.isa_functional_subgroups.push_back(FunctionalSubgroupNameToEnum(subgroup));
        }
    }

    static bool ExtractDisassembly(std::string&                                  shader_disassembly,
                                   const std::string&                            shader_disassembly_text,
                                   std::string&                                  err_message,
                                   bool                                          resolve_direct_branch_targets,
                                   std::vector<std::string>&                     pc_to_index_map,
                                   std::unordered_map<std::string, std::string>& pc_to_label_map)
    {
        bool is_success = true;

        // Parse and decode the instructions in the disassembly file
        std::stringstream shader_text(shader_disassembly_text);
        std::string       line;
        std::string       label;
        bool              is_prev_line_label = false;
        while (is_success && std::getline(shader_text, line, kShaderTextDelimiter))
        {
            line = AmdIsaUtility::Strip(line);

            // Go to the inline comment in the line read
            const char* kCodeCommentToken = "//";
            const char  kColumnToken      = ':';
            const char  kLlvmCommentToken = ';';
            std::size_t pos_comment       = line.find(kCodeCommentToken);
            std::size_t pos_comment_llvm  = line.find(kLlvmCommentToken);
            std::size_t pos_label_colon   = line.find(kColumnToken);

            // Ignore sp3 program line comments
            bool is_code_line = false;
            if (pos_comment > 0 && pos_comment != std::string::npos)
            {
                // Parse dword/s if match found
                pos_comment = line.find(kColumnToken, pos_comment);
                if (pos_comment != std::string::npos)
                {
                    // Skip through to the first dword (": ", 2 characters)
                    if ((pos_comment + 2) < line.length())
                    {
                        pos_comment += 2;
                        is_code_line = true;
                    }
                }
            }

            // Ignore the code lines with special words that look like assembly.
            is_code_line = is_code_line && (line.find(".long") == std::string::npos);
            is_code_line = is_code_line && (line.find(".ascii") == std::string::npos);
            is_code_line = is_code_line && (line.find(".byte") == std::string::npos);

            // If parsing was successful, then decode the instructions
            std::string pc;
            if (is_code_line)
            {
                // If resolve direct branch targets is enabled by the user
                if (resolve_direct_branch_targets)
                {
                    size_t location = line.find(kCodeCommentToken);
                    pc              = line.substr((location + 3), line.find(":", location) - (location + 3));
                    pc_to_index_map.push_back(pc);
                    if (is_prev_line_label)
                    {
                        is_prev_line_label  = false;
                        pc_to_label_map[pc] = label;
                    }
                }

                // Decode dword/s
                std::stringstream stream;
                stream << line.substr(pos_comment);
                shader_disassembly += stream.str() + kShaderTextDelimiter;
            }
            else
            {
                // Determine if the line-under-process is a label.
                // Save the label and set the flag for the next iteration.
                const bool is_current_line_label = (pos_comment > pos_label_colon) && (pos_comment_llvm > 0);
                if (is_current_line_label)
                {
                    is_prev_line_label = true;
                    line.pop_back();
                    label = line;
                }
            }
        }
        return is_success;
    }

    /*
     * DecodeShaderDisassembly --
     *
     * Accepts a shader disassembly text.
     *
     * Returns true if all the instructions present in the provided shader
     * disassembly text were successfully decoded, or false even if a single
     * instruction fails to be decoded - the failure reason is populated in
     * the err_message. On successful decode, the function outputs a vector
     * of InstructionInfoBundle with the decoded information of all the
     * instructions.
     *
     * Additional Option: resolve_direct_branch_targets
     * (optional, default - false and skipped)
     * Performs the branch target resolution for all direct branches. Stores
     * target PC, branch label name and target info index.
     */
    static bool DecodeShaderDisassembly(const IsaDecoder* const                             spec_decoder,
                                        const std::string&                                  shader_disassembly_text,
                                        std::vector<InstructionInfoBundle>&                 instruction_info_stream,
                                        std::string&                                        err_message,
                                        bool                                                resolve_direct_branch_targets,
                                        const std::vector<std::string>&                     pc_to_index_map,
                                        const std::unordered_map<std::string, std::string>& pc_to_label_map,
                                        std::vector<std::string>&                           log)
    {
        bool is_success = true;

        if (spec_decoder == nullptr)
        {
            is_success  = false;
            err_message = kStringErrorIsaDecoderNotFound;
        }

        if (is_success && resolve_direct_branch_targets && pc_to_index_map.empty() && pc_to_label_map.empty())
        {
            // Cannot perform branch target resolution since the required information is not available
            is_success  = false;
            err_message = kStringErrorMissingTargetResolutionInfo;
        }

        std::vector<uint64_t> direct_branch_indexes;
        std::stringstream     inst_stream(shader_disassembly_text);
        std::string           inst;
        while (is_success && std::getline(inst_stream, inst, kShaderTextDelimiter))
        {
            std::stringstream stream;
            stream << inst;
            static const uint8_t kDwordByte = 8;
            bool                 is_decoded = false;
            if (stream.str().length() == kDwordByte)
            {
                // Single machine code 64-bit
                std::uint64_t                 machine_code_64         = 0;
                amdisa::InstructionInfoBundle instruction_info_bundle = {};

                // Convert instruction DWORD from hex to uint64_t
                stream >> std::hex >> machine_code_64;
                is_decoded = spec_decoder->DecodeInstruction(machine_code_64, instruction_info_bundle, err_message);

                // Store the target pc for direct branch instructions, if
                // enabled
                if (is_decoded && resolve_direct_branch_targets && !instruction_info_bundle.bundle.empty() &&
                    instruction_info_bundle.bundle[0].instruction_semantic_info.branch_info.is_branch &&
                    instruction_info_bundle.bundle[0].instruction_semantic_info.branch_info.IsDirect())
                {
                    // Fetch the branch instruction's PC
                    std::stringstream pc_stream = std::stringstream(pc_to_index_map[instruction_info_stream.size()]);
                    std::uint64_t     target_pc = 0;
                    pc_stream >> std::hex >> target_pc;
                    target_pc += kInstructionBytes32 *
                                 (static_cast<uint64_t>(instruction_info_bundle.bundle[0].instruction_semantic_info.branch_info.branch_offset) + 1);

                    // Store target_pc for branch resolution
                    instruction_info_bundle.bundle[0].instruction_semantic_info.branch_info.branch_target_index = target_pc;
                    direct_branch_indexes.push_back(instruction_info_stream.size());
                }
                instruction_info_stream.push_back(instruction_info_bundle);
            }
            else if (stream.str().length() > kDwordByte)
            {
                // For 32-bit Instructions
                std::uint32_t              machine_code_32  = 0;
                std::vector<std::uint32_t> machine_codes_32 = {};

                // Pack DWORDs in an instruction stream for decoding
                while (stream >> std::hex >> machine_code_32)
                {
                    machine_codes_32.push_back(machine_code_32);
                    machine_code_32 = 0;
                }
                is_decoded = spec_decoder->DecodeInstructionStream(machine_codes_32, instruction_info_stream, err_message);
                machine_codes_32.clear();
            }
            else
            {
                is_success  = false;
                err_message = kStringErrorShaderTextInvalidDwordByte;
                err_message.append(stream.str());
            }

            if (!is_decoded)
            {
                // For debugging - Store information of instructions that failed in decoding
                amdisa::InstructionInfo ErrorInstructionInfo;
                ErrorInstructionInfo.instruction_name        = kStringNa;
                ErrorInstructionInfo.instruction_description = kStringNa;
                ErrorInstructionInfo.encoding_name           = kStringNa;
                ErrorInstructionInfo.encoding_description    = kStringNa;
                ErrorInstructionInfo.encoding_layout         = kStringNa;
                if (instruction_info_stream.empty())
                {
                    amdisa::InstructionInfoBundle ErrorInstructionBundle;
                    ErrorInstructionBundle.bundle.push_back(ErrorInstructionInfo);
                    instruction_info_stream.push_back(ErrorInstructionBundle);
                }
                else
                {
                    amdisa::InstructionInfoBundle& ErrorInstructionBundle = instruction_info_stream.back();
                    ErrorInstructionBundle.bundle.clear();
                    ErrorInstructionBundle.bundle.push_back(ErrorInstructionInfo);
                }

                std::stringstream err_ss;
                err_ss << kStringErrorShaderTextDecodeFailed << "(" << inst << ")";
                log.push_back(err_ss.str());
            }
        }

        // Replace target pc of direct branches to their respective
        // InstructionInfoBundle vector index, if enabled
        if (is_success && resolve_direct_branch_targets && !instruction_info_stream.empty())
        {
            for (const auto& index : direct_branch_indexes)
            {
                // Fetch InstructionInfo for the index
                amdisa::InstructionInfo& inst_info = instruction_info_stream[index].bundle[0];

                // Target pc conversion and lookup in pc_index_map
                std::stringstream pc_formatter;
                pc_formatter << std::uppercase << std::hex << std::setfill('0') << std::setw(12)
                             << inst_info.instruction_semantic_info.branch_info.branch_target_index;

                // Lookup for the target index in PC to Index Map
                const auto& target_index_iter = std::find(pc_to_index_map.begin(), pc_to_index_map.end(), pc_formatter.str());
                const auto& target_label_iter = pc_to_label_map.find(pc_formatter.str());
                const bool  is_label_found    = (!pc_to_label_map.empty() && target_label_iter != pc_to_label_map.end());
                const bool  can_access        = ((target_index_iter != pc_to_index_map.end()) && (pc_to_label_map.empty() || is_label_found));
                if (can_access)
                {
                    inst_info.instruction_semantic_info.branch_info.branch_target_pc    = pc_formatter.str();
                    inst_info.instruction_semantic_info.branch_info.branch_target_index = uint64_t(target_index_iter - pc_to_index_map.begin());
                    if (is_label_found)
                    {
                        inst_info.instruction_semantic_info.branch_info.branch_target_label = target_label_iter->second;
                    }
                }
                else
                {
                    is_success  = false;
                    err_message = kStringErrorBranchTargetInfoNotFound;
                    if (target_index_iter != pc_to_index_map.end())
                    {
                        err_message += kStringErrorBranchTargetIndexNotFound + pc_formatter.str();
                    }
                    if (target_label_iter != pc_to_label_map.end())
                    {
                        err_message += kStringErrorBranchTargetLabelNotFound + pc_formatter.str();
                    }
                    break;
                }
            }
        }

        if (is_success && instruction_info_stream.empty())
        {
            is_success  = false;
            err_message = kStringErrorShaderTextDecodeNoInst;
        }

        return is_success;
    }

    // *** INTERNALLY-LINKED AUXILIARY FUNCTIONS - END ***

    // API implementation definititon.
    class IsaDecoder::IsaDecoderImpl
    {
    public:
        IsaDecoderImpl() = default;
        IsaSpec& GetSpec()
        {
            return spec_data_;
        }

        bool IsInitialized() const
        {
            return is_initialized_;
        }

        void SetInitialized(bool is_initialized)
        {
            is_initialized_ = is_initialized;
        }

        bool Initialize(std::string& err_message)
        {
            // Check compatibility.
            const std::string& xml_schema_version = this->GetSpec().info.schema_version;
            bool               is_compatible      = amdisa::ApiVersion::IsCompatible(xml_schema_version, err_message);
            bool               is_isa_spec_parsed = true;

            if (is_compatible)
            {
                const IsaSpec& spec_data = this->GetSpec();

                // MIMG Address count information initialization.
                GpuArchitecture current_architecture = GetArchitectureWithId(spec_data.architecture.id);
                if (current_architecture == GpuArchitecture::kRdna1 || current_architecture == GpuArchitecture::kRdna2
                    || current_architecture == GpuArchitecture::kCdna1 || current_architecture == GpuArchitecture::kCdna2)
                {
                    mimg_acnt_table_.GenerateRdnaCdna1or2Table(current_architecture);
                }
                else if (current_architecture == GpuArchitecture::kRdna3 || current_architecture == GpuArchitecture::kRdna3_5)
                {
                    mimg_acnt_table_.GenerateRdna3or3p5Table(current_architecture);
                }
                else if (GetArchitectureWithId(spec_data.architecture.id) == GpuArchitecture::kRdna4)
                {
                    mimg_acnt_table_.GenerateRdna4Table();
                }

                // Map identifiers to encodings.
                for (const Encoding& encoding : spec_data.encodings)
                {
                    for (uint64_t identifier : encoding.identifiers)
                    {
                        // Identifier mask is the mask of the encoding fields and the opcode fields.
                        // Everything else can be safely masked out for this mapping.
                        uint64_t identifier_mask = (encoding.mask | encoding.opcode_mask | encoding.seg_mask);
                        identifier               = identifier & identifier_mask;
                        this->MapIdentifierToEncoding(identifier, std::make_shared<Encoding>(encoding));
                    }
                }

                // Map identifiers to instructions.
                for (const Instruction& instruction : spec_data.instructions)
                {
                    for (uint64_t encoding_itr = 0; (is_isa_spec_parsed && encoding_itr < instruction.encodings.size()); encoding_itr++)
                    {
                        const InstructionEncoding& instruction_encoding = instruction.encodings[encoding_itr];
                        // VOPD handling, determine if x or y layout.
                        bool is_x_layout = IsOperandPresent(instruction_encoding, "VDSTX");
                        bool is_y_layout = IsOperandPresent(instruction_encoding, "VDSTY");

                        // Form encoding identifier.
                        const auto& found_encoding_iterator = std::find_if(spec_data.encodings.begin(), spec_data.encodings.end(), [&](const Encoding& encoding) {
                            return encoding.name == instruction_encoding.name;
                        });
                        assert(found_encoding_iterator != spec_data.encodings.end());

                        const auto& microcode    = found_encoding_iterator->microcode_format;
                        const auto& op_iter      = GetFieldIterator(microcode, "OP");
                        const auto& opx_iter     = GetFieldIterator(microcode, "OPX");
                        const auto& opy_iter     = GetFieldIterator(microcode, "OPY");
                        const auto& seg_iter     = GetFieldIterator(microcode, "SEG");
                        bool        is_op_found  = op_iter != microcode.bit_map.end();
                        bool        is_opx_found = opx_iter != microcode.bit_map.end();
                        bool        is_opy_found = opy_iter != microcode.bit_map.end();
                        bool        is_seg_found = seg_iter != microcode.bit_map.end();
                        uint64_t    identifier   = found_encoding_iterator->bits;

                        Range range;
                        if (is_op_found)
                        {
                            if (!AmdIsaUtility::GetRange(*op_iter, range, 0))
                            {
                                is_isa_spec_parsed = false;
                                err_message        = kStringErrorEmptyRangesInField + op_iter->name;
                            }
                            else
                            {
                                const uint64_t positioned_op = AmdIsaUtility::PositionValueToField(instruction_encoding.opcode,
                                    found_encoding_iterator->microcode_format, "OP");
                                identifier |= positioned_op;
                                if (is_seg_found)
                                {
                                    uint64_t positioned_seg = 0;
                                    if (kSegBits.find(instruction_encoding.name) != kSegBits.end())
                                    {
                                        if (!AmdIsaUtility::GetRange(*seg_iter, range, 0))
                                        {
                                            is_isa_spec_parsed = false;
                                            err_message        = kStringErrorEmptyRangesInField + seg_iter->name;
                                        }
                                        else
                                        {
                                            positioned_seg = (static_cast<uint64_t>(kSegBits.at(instruction_encoding.name)) << range.bit_offset);
                                            identifier |= positioned_seg;
                                        }
                                    }
                                    else
                                    {
                                        this->GetLog().push_back(kStringWarningSegBits);
                                    }
                                }
                            }
                        }
                        else if (is_x_layout)
                        {
                            assert(is_opx_found && !is_y_layout);
                            if (!AmdIsaUtility::GetRange(*opx_iter, range, 0))
                            {
                                is_isa_spec_parsed = false;
                                err_message        = kStringErrorEmptyRangesInField + opx_iter->name;
                            }
                            else
                            {
                                const uint64_t positioned_opx = static_cast<uint64_t>(instruction_encoding.opcode) << range.bit_offset;
                                identifier |= positioned_opx;
                            }
                        }
                        else if (is_y_layout)
                        {
                            assert(is_opy_found && !is_x_layout);
                            if (!AmdIsaUtility::GetRange(*opy_iter, range, 0))
                            {
                                is_isa_spec_parsed = false;
                                err_message        = kStringErrorEmptyRangesInField + opy_iter->name;
                            }
                            else
                            {
                                const uint64_t positioned_opy = static_cast<uint64_t>(instruction_encoding.opcode) << range.bit_offset;
                                identifier |= positioned_opy;
                            }
                        }
                        else
                        {
                            assert(instruction.name.find("EXP") != std::string::npos);
                        }

                        // Map.
                        this->MapIdentifierToInstruction(identifier, std::make_shared<Instruction>(instruction));
                        this->MapIdentifierToInstructionEncoding(identifier, std::make_shared<InstructionEncoding>(instruction_encoding));
                    }
                }
            }

            return is_compatible && is_isa_spec_parsed;
        }

        void MapIdentifierToEncoding(uint64_t identifier, std::shared_ptr<Encoding> encoding_ptr)
        {
            if (identifier_to_encoding_.find(identifier) == identifier_to_encoding_.end())
            {
                identifier_to_encoding_[identifier] = encoding_ptr;
            }
        }

        IdToEncoding& GetEncodingMap()
        {
            return identifier_to_encoding_;
        }

        void MapIdentifierToInstruction(uint64_t identifier, std::shared_ptr<Instruction> instr_ptr)
        {
            if (identifier_to_instruction_.find(identifier) == identifier_to_instruction_.end())
            {
                identifier_to_instruction_[identifier] = instr_ptr;
            }
            else
            {
                if (identifier_to_instruction_[identifier]->name != instr_ptr->name)
                {
                    log_.push_back(kStringWarningApiInitSameMappingInst);
                }
            }
        }

        IdToInstruction& GetInstructionMap()
        {
            return identifier_to_instruction_;
        }

        void MapIdentifierToInstructionEncoding(uint64_t identifier, std::shared_ptr<InstructionEncoding> instr_enc_ptr)
        {
            auto&       instr_enc_vec  = identifier_to_instruction_encoding_vec_[identifier];
            const auto& instr_enc_iter = std::find_if(instr_enc_vec.begin(), instr_enc_vec.end(), [&](std::shared_ptr<InstructionEncoding> pushed_instr_enc) {
                return (pushed_instr_enc->name == instr_enc_ptr->name) && (pushed_instr_enc->condition_name == instr_enc_ptr->condition_name);
            });
            if (instr_enc_iter == instr_enc_vec.end())
            {
                identifier_to_instruction_encoding_vec_[identifier].push_back(instr_enc_ptr);
            }
            else if ((instr_enc_ptr->name.find("VOPD") == std::string::npos) && (instr_enc_ptr->name.find("FLAT") == std::string::npos))
            {
                log_.push_back(kStringWarningApiInitSameMappingEnc);
            }
        }

        IdToInstructionEncodings& GetInstructionEncodingMap()
        {
            return identifier_to_instruction_encoding_vec_;
        }

        EncodingConditionHandler& GetEncodingConditionHandler()
        {
            return condition_handler_;
        }

        std::vector<std::string>& GetLog()
        {
            return log_;
        }

        mimg_workaround::AcntHandler& GetMimgAcntTable()
        {
            return mimg_acnt_table_;
        }

    private:
        // Internal representation of the spec.
        IsaSpec spec_data_;

        // Is set to true once XML was successfully read.
        bool is_initialized_ = false;

        // Map from id to the encoding spec, provides constant time access to the encodings.
        IdToEncoding identifier_to_encoding_;

        // Map from id to the instruction spec, provides constant time access to the instructions.
        IdToInstruction identifier_to_instruction_;

        // Map from id to the vector of operand layout spec, provides constant time access to the operand layouts.
        IdToInstructionEncodings identifier_to_instruction_encoding_vec_;

        // Encoding conditions handler. The class contains functions that are needed for runtime
        // evaluation of encodings. For example, should we pick an encoding with literal, dpp or sdwa.
        EncodingConditionHandler condition_handler_;

        // Log messages that should be communicated from API which are not errors.
        std::vector<std::string> log_;

        // MIMG Address Count information.
        mimg_workaround::AcntHandler mimg_acnt_table_;
    };

    bool IsaDecoder::Initialize(const std::string& input_xml_file_path, std::string& err_message)
    {
        bool is_xml_read_successful = true;

        // Allocate implementation.
        if (api_impl_ == nullptr)
        {
            api_impl_ = new IsaDecoderImpl();
        }
        else
        {
            delete api_impl_;
            api_impl_ = new IsaDecoderImpl();
        }

        if (api_impl_ == nullptr)
        {
            is_xml_read_successful = false;
            err_message            = kStringErrorApiImplAllocationFailed;
        }

        // Read spec.
        if (is_xml_read_successful)
        {
            is_xml_read_successful = IsaXmlReader::ReadSpec(input_xml_file_path, api_impl_->GetSpec(), err_message);
            api_impl_->SetInitialized(is_xml_read_successful);
        }


        return is_xml_read_successful && api_impl_->Initialize(err_message) && (api_impl_ != nullptr);
    }

    bool IsaDecoder::Initialize(const char *input_xml_data, const size_t datalen, std::string& err_message)
    {
        bool is_xml_read_successful = true;

        // Allocate implementation.
        if (api_impl_ == nullptr)
        {
            api_impl_ = new IsaDecoderImpl();
        }
        else
        {
            delete api_impl_;
            api_impl_ = new IsaDecoderImpl();
        }

        if (api_impl_ == nullptr)
        {
            is_xml_read_successful = false;
            err_message            = kStringErrorApiImplAllocationFailed;
        }

        // Read spec.
        if (is_xml_read_successful)
        {
            is_xml_read_successful = IsaXmlReader::ReadSpec(input_xml_data, datalen, api_impl_->GetSpec(), err_message);
            api_impl_->SetInitialized(is_xml_read_successful);
        }


        return is_xml_read_successful && api_impl_->Initialize(err_message) && (api_impl_ != nullptr);
    }

    std::string IsaDecoder::GetVersion() const
    {
        return amdisa::ApiVersion::GetVersion();
    }

    bool IsaDecoder::DecodeShaderDisassemblyText(const std::string&                  shader_disassembly_text,
                                                 std::vector<InstructionInfoBundle>& instruction_info_stream,
                                                 std::string&                        err_message,
                                                 bool                                resolve_direct_branch_targets) const
    {
        bool        is_success = true;
        std::string disassembly_text;

        // PC and index pair map for branch target resolution
        // Indexes are of instruction_info_stream
        std::vector<std::string> pc_to_index_map = {};

        // PC and label pair.
        std::unordered_map<std::string, std::string> pc_to_label_map = {};

        if (is_success)
        {
            is_success =
                ExtractDisassembly(disassembly_text, shader_disassembly_text, err_message, resolve_direct_branch_targets, pc_to_index_map, pc_to_label_map);
        }

        // Decode the extracted instructions
        if (is_success)
        {
            is_success = DecodeShaderDisassembly(this,
                                                 disassembly_text,
                                                 instruction_info_stream,
                                                 err_message,
                                                 resolve_direct_branch_targets,
                                                 pc_to_index_map,
                                                 pc_to_label_map,
                                                 api_impl_->GetLog());
        }
        return is_success;
    }

    bool IsaDecoder::DecodeShaderDisassemblyFile(const std::string&                  shader_disassembly_file,
                                                 std::vector<InstructionInfoBundle>& instruction_info_stream,
                                                 std::string&                        err_message,
                                                 bool                                resolve_direct_branch_targets) const
    {
        bool is_success = true;

        // PC and index pair map for branch target resolution
        // Indexes are of instruction_info_stream
        std::vector<std::string> pc_to_index_map = {};

        // PC and label pair.
        std::unordered_map<std::string, std::string> pc_to_label_map = {};

        // Read from file and prepare for extract
        std::string shader_disassembly_file_text;
        try
        {
            std::ifstream disassembly_file;
            disassembly_file.open(shader_disassembly_file, std::ios::in);
            if (disassembly_file.is_open())
            {
                while (!disassembly_file.eof() && is_success)
                {
                    std::string line;
                    getline(disassembly_file, line);
                    line = AmdIsaUtility::Strip(line);
                    shader_disassembly_file_text += (line + kShaderTextDelimiter);
                }
            }
        }
        catch (const std::exception& e)
        {
            is_success = false;
            err_message.append(e.what());
        }

        // Parse the instructions from the shader disassembly file
        std::string shader_disassembly_text;
        if (is_success)
        {
            is_success = ExtractDisassembly(
                shader_disassembly_text, shader_disassembly_file_text, err_message, resolve_direct_branch_targets, pc_to_index_map, pc_to_label_map);
        }

        // Decode the extracted instructions
        if (is_success)
        {
            is_success = DecodeShaderDisassembly(this,
                                                 shader_disassembly_text,
                                                 instruction_info_stream,
                                                 err_message,
                                                 resolve_direct_branch_targets,
                                                 pc_to_index_map,
                                                 pc_to_label_map,
                                                 api_impl_->GetLog());
        }
        return is_success;
    }

    bool IsaDecoder::DecodeInstructionStream(const std::vector<uint32_t>&        machine_code_stream,
                                             std::vector<InstructionInfoBundle>& instruction_info_stream,
                                             std::string&                        err_message) const
    {
        bool is_decode_failed = true;

        bool is_api_init = false;
        if (api_impl_ != nullptr)
        {
            is_api_init = api_impl_->IsInitialized();
        }

        if (is_api_init)
        {
            const amdisa::IsaSpec& spec_data = api_impl_->GetSpec();

            MachineCodeStream stream;
            stream.Init(machine_code_stream);
            std::vector<uint32_t> working_dwords;
            is_decode_failed = false;
            while (!is_decode_failed && !stream.IsEmpty())
            {
                // Get words from the machine code stream for the current instruction.
                working_dwords.clear();
                working_dwords.push_back(stream.GetNextDword(api_impl_->GetLog()));

                // Get the encoding.
                auto encoding_ptr = GetEncodingIterator(api_impl_->GetEncodingMap(), working_dwords[0], spec_data);

                if (encoding_ptr != nullptr)
                {
                    // Insert one instruction info group.
                    instruction_info_stream.push_back(InstructionInfoBundle());
                    auto& instruction_info_bundle = instruction_info_stream.back();

                    // Get next dword from instruction stream for wider encodings.
                    while (working_dwords.size() * 32 < encoding_ptr->bit_count)
                    {
                        working_dwords.push_back(stream.GetNextDword(api_impl_->GetLog()));
                    }

                    // VOPD encoding requires special handling due to two opcodes.
                    uint32_t opcode_count      = 1;
                    bool     is_vopd_encoding  = false;
                    bool     is_smem_encoding  = false;
                    bool     is_mubuf_encoding = false;
                    bool     is_flat_encoding  = false;
                    if (encoding_ptr->name.find("VOPD") != std::string::npos)
                    {
                        opcode_count     = 2;
                        is_vopd_encoding = true;
                    }
                    else if (encoding_ptr->name.find("SMEM") != std::string::npos)
                    {
                        is_smem_encoding = true;
                    }
                    else if (encoding_ptr->name.find("MUBUF") != std::string::npos)
                    {
                        is_mubuf_encoding = true;
                    }

                    // Dual Ops common src_literal case
                    bool        is_prev_operand_lit = false;
                    std::string prev_lit_operand_name;
                    while (opcode_count > 0)
                    {
                        // Determine the name of the OP field based on the encoding.
                        std::string opcode_field_name = "OP";
                        bool        is_vopdx          = false;
                        bool        is_vopdy          = false;
                        if (is_vopd_encoding)
                        {
                            if (opcode_count == 2)
                            {
                                opcode_field_name = "OPX";
                                is_vopdx          = true;
                            }
                            else
                            {
                                opcode_field_name = "OPY";
                                is_vopdy          = true;
                            }
                        }
                        --opcode_count;

                        // Get the opcode.
                        uint64_t    opcode_value = 0;
                        const auto& field_iterator =
                            GetFieldIterator(working_dwords, opcode_field_name, encoding_ptr->microcode_format, opcode_value, api_impl_->GetLog());

                        // EXP encoding has no OP field.
                        bool is_exp                  = encoding_ptr->name.find("EXP") != std::string::npos;
                        bool is_retrieval_successful = (field_iterator != encoding_ptr->microcode_format.bit_map.end() || is_exp);

                        // Get the instruction name.
                        if (is_retrieval_successful)
                        {
                            instruction_info_bundle.bundle.push_back(InstructionInfo());
                            auto& instruction_info = instruction_info_bundle.bundle.back();

                            // Get the modifiers.
                            bool is_extracted =
                                ExtractModifiers(working_dwords, *encoding_ptr, instruction_info.operand_modifiers, err_message, api_impl_->GetLog());
                            if (is_extracted)
                            {
                                // Get the fields info into the container.
                                RetrieveFieldInfo(working_dwords, encoding_ptr->microcode_format.bit_map, instruction_info, api_impl_->GetLog());
                                uint32_t second_dword = 0;
                                if (working_dwords.size() > 1)
                                {
                                    second_dword = working_dwords[1];
                                }
                                const auto& instruction_ptrs = GetInstructionPtrs(spec_data.architecture.id,
                                                                                  api_impl_->GetInstructionMap(),
                                                                                  api_impl_->GetInstructionEncodingMap(),
                                                                                  *encoding_ptr,
                                                                                  api_impl_->GetEncodingConditionHandler(),
                                                                                  working_dwords[0],
                                                                                  second_dword,
                                                                                  is_vopdx,
                                                                                  is_vopdy);

                                bool are_pointers_retrieved =
                                    instruction_ptrs.instruction_encoding_ptr != nullptr && instruction_ptrs.instruction_ptr != nullptr;
                                assert(are_pointers_retrieved);
                                if (are_pointers_retrieved)
                                {
                                    // Get the conditional encoding.
                                    if (encoding_ptr->name != instruction_ptrs.instruction_encoding_ptr->name)
                                    {
                                        const auto& encoding_iter =
                                            std::find_if(spec_data.encodings.begin(), spec_data.encodings.end(), [&](const Encoding& encoding) {
                                                return encoding.name == instruction_ptrs.instruction_encoding_ptr->name;
                                            });
                                        assert(encoding_iter != spec_data.encodings.end());
                                        encoding_ptr = std::make_shared<Encoding>(*encoding_iter);

                                        // Retrieve all required dwords from the instruction stream.
                                        while (working_dwords.size() * 32 < encoding_ptr->bit_count)
                                        {
                                            working_dwords.push_back(stream.GetNextDword(api_impl_->GetLog()));
                                        }
                                    }

                                    // Encoding info retrieve successfully -- save return values.
                                    instruction_info.encoding_description = encoding_ptr->description;
                                    instruction_info.encoding_name        = encoding_ptr->name;

                                    // Get the fields info in a formated single string for printing.
                                    amdisa::AmdIsaUtility::BitMapToString(encoding_ptr->microcode_format, working_dwords, instruction_info.encoding_layout);

                                    // Find the instruction.
                                    bool is_instruction_retrieval_successful =
                                        instruction_ptrs.instruction_encoding_ptr != nullptr && instruction_ptrs.instruction_ptr != nullptr;

                                    if (is_instruction_retrieval_successful)
                                    {
                                        // Dereference pointers for local use.
                                        const InstructionEncoding& inst_enc = *(instruction_ptrs.instruction_encoding_ptr);
                                        const Instruction&         inst     = *(instruction_ptrs.instruction_ptr);

                                        // Instruction info retrieved successfully -- save return values.
                                        instruction_info.instruction_name                                     = inst.name;
                                        instruction_info.aliased_names                                        = inst.aliased_names;
                                        instruction_info.instruction_semantic_info.branch_info.is_branch      = inst.is_branch;
                                        instruction_info.instruction_semantic_info.branch_info.is_conditional = inst.is_conditional_branch;
                                        instruction_info.instruction_semantic_info.branch_info.is_indirect    = inst.is_indirect_branch;
                                        instruction_info.instruction_semantic_info.is_immediately_executed    = inst.is_immediately_executed;
                                        instruction_info.instruction_semantic_info.is_program_terminator      = inst.is_program_terminator;
                                        instruction_info.instruction_description                              = inst.description;

                                        // Get Functional Group and Subgroup Information
                                        GetFunctionalGroupSubgroupInfo(instruction_info, inst.functional_group_name, inst.functional_subgroups);

                                        // Get Functional Group Description
                                        instruction_info.functional_group_subgroup_info.description = "Functional group description not found!";
                                        for (auto group_itr = spec_data.functional_group_info.begin(); group_itr != spec_data.functional_group_info.end();
                                             ++group_itr)
                                        {
                                            if (group_itr->name.compare(inst.functional_group_name) == 0)
                                            {
                                                instruction_info.functional_group_subgroup_info.description = group_itr->desc;
                                                break;
                                            }
                                        }

                                        // DMASK
                                        uint64_t dmask_value = 0;
                                        const auto& dmask_field_iterator =
                                            GetFieldIterator(working_dwords, "DMASK", encoding_ptr->microcode_format, dmask_value, api_impl_->GetLog());
                                        bool has_dmask_field = (dmask_field_iterator != encoding_ptr->microcode_format.bit_map.end());

                                        // SADDR
                                        uint64_t    saddr_value = 0;
                                        const auto& saddr_field_iterator =
                                            GetFieldIterator(working_dwords, "SADDR", encoding_ptr->microcode_format, saddr_value, api_impl_->GetLog());
                                        bool has_operand_saddr = (saddr_field_iterator != encoding_ptr->microcode_format.bit_map.end());

                                        // Get the operands.
                                        Encoding encoding = *encoding_ptr;
                                        for (auto operands_iterator = inst_enc.operands.begin();
                                             !is_decode_failed && operands_iterator != inst_enc.operands.end();
                                             ++operands_iterator)
                                        {
                                            // Dereference the iterator.
                                            const auto& operand = *operands_iterator;

                                            // No need to process implicit operands as they are not in the
                                            // machine code.
                                            if (!operand.is_implicit)
                                            {
                                                std::string field_name = operand.encoding_field_name;

                                                // Convert field name to upper case.
                                                field_name = AmdIsaUtility::ToUpper(field_name);

                                                bool is_implied_literal = false;
                                                if (encoding.name.find("LITERAL") != std::string::npos && field_name.empty())
                                                {
                                                    is_implied_literal = true;
                                                }

                                                // Get the value of the field.
                                                uint64_t    field_value = 0;
                                                const auto& field_iterator =
                                                    GetFieldIterator(working_dwords, field_name, encoding.microcode_format, field_value, api_impl_->GetLog());
                                                bool is_field_found = field_iterator != encoding.microcode_format.bit_map.end();

                                                // Known cases when the field name may not be present in the
                                                // encoding.
                                                bool is_operand_literal = false;
                                                if (!is_field_found)
                                                {
                                                    if (field_name == "LITERAL")
                                                    {
                                                        is_field_found     = true;
                                                        is_operand_literal = true;
                                                    }
                                                    else if (!operand.is_in_microcode)
                                                    {
                                                        is_field_found = true;
                                                    }
                                                    else
                                                    {
                                                        assert(false);
                                                    }
                                                }

                                                // Get the operand type.
                                                const auto& operand_type_iterator = std::find_if(
                                                    spec_data.operand_types.begin(), spec_data.operand_types.end(), [&](const OperandType& operand_type) {
                                                        return operand_type.name == operand.type;
                                                    });

                                                bool is_operand_retrieval_successful = is_field_found && operand_type_iterator != spec_data.operand_types.end();

                                                // Get the operand names.
                                                if (is_operand_retrieval_successful)
                                                {
                                                    instruction_info.instruction_operands.push_back(InstructionOperand());
                                                    auto& instruction_operand               = instruction_info.instruction_operands.back();
                                                    instruction_operand.is_input            = operand.input;
                                                    instruction_operand.is_output           = operand.output;
                                                    instruction_operand.operand_size        = operand.size;
                                                    instruction_operand.data_format         = operand.data_format;
                                                    instruction_operand.encoding_field_name = operand.encoding_field_name;

                                                    const auto& predefined_value_iterator = std::find_if(
                                                        operand_type_iterator->predefined_values.begin(),
                                                        operand_type_iterator->predefined_values.end(),
                                                        [&](const PredefinedValue& predefined_value) { return predefined_value.value == field_value; });

                                                    // Save branch offset if branch.
                                                    if (instruction_info.instruction_semantic_info.branch_info.is_branch)
                                                    {
                                                        if (instruction_info.instruction_semantic_info.branch_info.IsDirect())
                                                        {
                                                            instruction_info.instruction_semantic_info.branch_info.branch_offset =
                                                                static_cast<int16_t>(field_value);
                                                        }
                                                    }

                                                    if (field_name.find("LITERAL") != std::string::npos && encoding.name.find("LITERAL") != std::string::npos)
                                                    {
                                                        std::stringstream formatter;
                                                        formatter << std::hex << "lit(0x" << field_value << ")";
                                                        instruction_operand.operand_name = formatter.str();
                                                    }
                                                    else if (predefined_value_iterator != operand_type_iterator->predefined_values.end())
                                                    {
                                                        if (!predefined_value_iterator->name.empty())
                                                        {
                                                            // Get operand name. Expand the name to range format if operand size
                                                            // is greater than 32 bits.
                                                            const std::string operand_name   = predefined_value_iterator->name;
                                                            instruction_operand.operand_name = operand_name;
                                                            if (operand_name.length() > 1)
                                                            {
                                                                const bool kIsMimgEnc     = (encoding_ptr->name.find("MIMG") != std::string::npos);
                                                                const bool kIsVimageEnc   = (encoding_ptr->name.find("VIMAGE") != std::string::npos);
                                                                const bool kIsVsampleEnc  = (encoding_ptr->name.find("VSAMPLE") != std::string::npos);
                                                                const bool kIsVdataOperand = (operand.encoding_field_name.find("VDATA") != std::string::npos);
                                                                const bool kIsAddrOperand = (operand.encoding_field_name.find("VADDR") != std::string::npos) ||
                                                                                            (operand.encoding_field_name.empty() && (kIsVimageEnc || kIsVsampleEnc));
                                                                const bool kIsNsa         = (encoding_ptr->name.find("NSA") != std::string::npos);
                                                                const bool kIsListOperand = (kIsNsa || kIsVimageEnc || kIsVsampleEnc) && kIsAddrOperand;
                                                                const bool kIsWaveDependent = (operand.data_format == kWaveDependentFormat) ||
                                                                    (is_mubuf_encoding && operand.encoding_field_name == "VADDR");

                                                                const bool is_next_digit = std::isdigit(static_cast<uint8_t>(operand_name[1]));
                                                                const bool is_sgpr       = (operand_name[0] == 's') && (is_next_digit);
                                                                const bool is_vgpr       = (operand_name[0] == 'v') && (is_next_digit);
                                                                if (kIsListOperand)
                                                                {
                                                                    instruction_operand.operand_name = mimg_workaround::GetNameAsList(working_dwords, encoding.microcode_format,
                                                                        operand_type_iterator->predefined_values, api_impl_->GetMimgAcntTable(), GetArchitecture(),
                                                                        api_impl_->GetLog());
                                                                }
                                                                else if (kIsAddrOperand && !kIsNsa && kIsMimgEnc)
                                                                {
                                                                    int32_t acnt = mimg_workaround::GetAcnt(working_dwords,
                                                                                                            encoding.microcode_format,
                                                                                                            api_impl_->GetMimgAcntTable(),
                                                                                                            api_impl_->GetLog());
                                                                    if (acnt > 0)
                                                                    {
                                                                        instruction_operand.operand_name =
                                                                            GetNameAsRegisterRange(operand_name, (acnt + 1) * kDwordSize);
                                                                    }
                                                                    else
                                                                    {
                                                                        instruction_operand.operand_name = operand_name;
                                                                    }
                                                                }
                                                                else if (kIsVsampleEnc && kIsVdataOperand && has_dmask_field && is_vgpr)
                                                                {
                                                                    uint8_t dmask_bitcount = AmdIsaUtility::BitCount(dmask_value);
                                                                    instruction_operand.operand_size = dmask_bitcount * kDwordSize;
                                                                    instruction_operand.operand_name =
                                                                        GetNameAsRegisterRange(operand_name, instruction_operand.operand_size);
                                                                }
                                                                else if (kIsAddrOperand && has_operand_saddr && saddr_value != kSrcNull && is_vgpr)
                                                                {
                                                                    instruction_operand.operand_size = kDwordSize;
                                                                }
                                                                else if (!kIsWaveDependent && (instruction_operand.operand_size > kDwordSize) &&
                                                                    (is_sgpr || is_vgpr))
                                                                {
                                                                    instruction_operand.operand_name =
                                                                        GetNameAsRegisterRange(operand_name, instruction_operand.operand_size);
                                                                }
                                                            }
                                                            // Add constant offset modifier for SMEM encoding instructions.
                                                            if ((is_smem_encoding || is_mubuf_encoding) && field_name == "SOFFSET")
                                                            {
                                                                uint64_t    const_offset   = 0;
                                                                const auto& field_iterator = GetFieldIterator(
                                                                    working_dwords, "OFFSET", encoding.microcode_format, const_offset, api_impl_->GetLog());
                                                                if (const_offset > 0)
                                                                {
                                                                    std::stringstream formatter;
                                                                    formatter << predefined_value_iterator->name << " offset:0x" << std::hex << const_offset;
                                                                    instruction_operand.operand_name = formatter.str();
                                                                }
                                                            }
                                                        }
                                                        else
                                                        {
                                                            instruction_operand.operand_name = std::to_string(predefined_value_iterator->value);
                                                        }
                                                    }
                                                    else if (operand.type.find("VCC") != std::string::npos)
                                                    {
                                                        instruction_operand.operand_name = "vcc";
                                                    }
                                                    else if (operand.type.find("EXEC") != std::string::npos)
                                                    {
                                                        instruction_operand.operand_name = "exec";
                                                    }
                                                    else if (operand_type_iterator->is_partitioned)
                                                    {
                                                        assert(field_value <= UINT32_MAX);
                                                        instruction_operand.operand_name = GeneratePartitionedOperand(instruction_info.instruction_name,
                                                            operand_type_iterator->microcode_format, static_cast<uint32_t>(field_value), api_impl_->GetLog());
                                                        if (is_implied_literal)
                                                        {
                                                            is_prev_operand_lit   = true;
                                                            prev_lit_operand_name = instruction_operand.operand_name;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        // Return raw bit value if no predefined values were
                                                        if (!is_operand_literal)
                                                        {
                                                            std::stringstream formatter;
                                                            formatter << std::hex << "0x" << field_value;
                                                            instruction_operand.operand_name = formatter.str();
                                                        }
                                                    }

                                                    // Handle  the implied literal case and the source literal case.
                                                    if (is_operand_literal || instruction_operand.operand_name == "src_literal")
                                                    {
                                                        if (!is_operand_literal)
                                                        {
                                                            assert(field_value == 255);
                                                        }
                                                        if (is_vopd_encoding && opcode_count == 0 && is_prev_operand_lit)
                                                        {
                                                            instruction_operand.operand_name = prev_lit_operand_name;

                                                            // Set to false since the second iteration of decoding
                                                            // dual operations type instructions is complete.
                                                            // opcode_count == 0
                                                            is_prev_operand_lit = false;
                                                        }
                                                        else
                                                        {
                                                            std::stringstream formatter;
                                                            uint32_t          lit = working_dwords[working_dwords.size() - 1];
                                                            if (encoding.name.find("LITERAL") == std::string::npos)
                                                            {
                                                                lit = stream.GetNextDword(api_impl_->GetLog());
                                                            }
                                                            formatter << "lit(0x" << std::hex << lit << ")";
                                                            instruction_operand.operand_name = formatter.str();
                                                            is_prev_operand_lit              = true;
                                                            prev_lit_operand_name            = instruction_operand.operand_name;
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    is_decode_failed = true;
                                                    err_message      = kStringErrorFailedToDecodeOperands;
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        is_decode_failed = true;
                                        std::stringstream err_stream;
                                        err_stream << kStringErrorInstructionNotFoundInSpec;
                                        if (instruction_ptrs.instruction_encoding_ptr == nullptr)
                                        {
                                            err_stream << " " << kStringErrorEncodingNotFound;
                                        }
                                        err_stream << kStringErrorEncodingNotFound;
                                        for (const auto& kDword : working_dwords)
                                        {
                                            err_stream << std::hex << "0x" << kDword << " ";
                                        }
                                        err_message = err_stream.str();
                                    }

                                    // Apply modifiers on the assembly operands.
                                    uint32_t neg_modifier_value = 0;
                                    uint32_t abs_modifier_value = 0;
                                    uint32_t offset_value       = 0;
                                    for (const auto& operand_modifier : instruction_info.operand_modifiers)
                                    {
                                        // Negation.
                                        if (operand_modifier.modifier_name.find(kModifierNameNegation) != std::string::npos)
                                        {
                                            neg_modifier_value = operand_modifier.value;
                                        }
                                        else if (operand_modifier.modifier_name.find(kModifierNameAbsoluteValue) != std::string::npos)
                                        {
                                            abs_modifier_value = operand_modifier.value;
                                        }
                                        else if (operand_modifier.modifier_name.find(kModifierNameOffset) != std::string::npos)
                                        {
                                            offset_value = operand_modifier.value;
                                        }
                                    }

                                    uint32_t check_bit_pos = 1;
                                    for (uint32_t i = 0; i < instruction_info.instruction_operands.size(); i++)
                                    {
                                        InstructionOperand& current_operand = instruction_info.instruction_operands[i];
                                        if (current_operand.is_input)
                                        {
                                            bool is_neg_bit_set = (neg_modifier_value & check_bit_pos) > 0;
                                            bool is_abs_bit_set = (abs_modifier_value & check_bit_pos) > 0;
                                            if (is_neg_bit_set)
                                            {
                                                current_operand.operand_name = "-" + current_operand.operand_name;
                                            }
                                            if (is_abs_bit_set)
                                            {
                                                current_operand.operand_name = "abs(" + current_operand.operand_name + ")";
                                            }

                                            check_bit_pos <<= 1;
                                        }
                                    }
                                }
                                else
                                {
                                    is_decode_failed = true;
                                    err_message      = kStringErrorEncodingIdentifiersNotFound;
                                }
                            }
                            else
                            {
                                is_decode_failed = true;
                                err_message      = kStringErrorModiferFieldNotFound;
                            }
                        }
                        else
                        {
                            is_decode_failed = true;
                        }
                    }

                    // Pop the inserted InstructionGroup if the decode failed.
                    if (is_decode_failed)
                    {
                        instruction_info_stream.pop_back();
                    }
                }
                else
                {
                    is_decode_failed = true;
                    std::stringstream err_stream;
                    err_stream << kStringErrorEncodingNotFound;
                    for (const auto& kDword : working_dwords)
                    {
                        err_stream << std::hex << "0x" << kDword << " ";
                    }
                    err_message = err_stream.str();
                }
            }
        }
        else
        {
            err_message = kStringErrorSpecNotInitialized;
        }

        return !is_decode_failed;
    }

    bool IsaDecoder::DecodeInstruction(uint64_t machine_code, InstructionInfoBundle& instruction_info_bundle, std::string& err_message) const
    {
        // Pack into stream.
        std::vector<uint32_t> machine_code_stream;
        while (machine_code > 0)
        {
            machine_code_stream.push_back(machine_code & kDwordMask);
            machine_code = machine_code >> 32;
        }

        // Decode the stream.
        std::vector<InstructionInfoBundle> instruction_info_stream;
        bool                               is_decoded = DecodeInstructionStream(machine_code_stream, instruction_info_stream, err_message);
        assert(is_decoded);
        if (is_decoded)
        {
            if (instruction_info_stream.empty())
            {
                is_decoded  = false;
                err_message = kStringErrorInstructionNotFoundInSpec;
            }
            else
            {
                instruction_info_bundle = instruction_info_stream[0];
            }
        }

        return is_decoded;
    }

    bool IsaDecoder::DecodeInstruction(const std::string& instruction_name, InstructionInfo& instruction_info, std::string& err_message) const
    {
        bool is_retrieval_failed = false;

        bool is_api_init = false;
        if (api_impl_ != nullptr)
        {
            is_api_init = api_impl_->IsInitialized();
        }
        else
        {
            is_retrieval_failed = true;
        }

        if (is_api_init)
        {
            const amdisa::IsaSpec& spec_data = api_impl_->GetSpec();

            // Convert to upper case.
            std::string instruction_name_all_caps = AmdIsaUtility::ToUpper(instruction_name);

            // Find the instruction by name.
            const auto& instructions_iterator = std::find_if(spec_data.instructions.begin(), spec_data.instructions.end(), [&](const Instruction& instruction) {
                return instruction.name == instruction_name_all_caps;
            });

            if (instructions_iterator != spec_data.instructions.end())
            {
                // Instruction found -- save return values.
                instruction_info.instruction_name        = instructions_iterator->name;
                instruction_info.instruction_description = instructions_iterator->description;
            }
            else
            {
                is_retrieval_failed = true;
                err_message         = kStringErrorInstructionNotFoundInSpec + instruction_name;
            }
        }
        else
        {
            err_message = kStringErrorSpecNotInitialized;
        }

        return !is_retrieval_failed;
    }

    std::vector<std::string> IsaDecoder::GetDebugLog() const
    {
        return api_impl_->GetLog();
    }

    GpuArchitecture IsaDecoder::GetArchitecture() const
    {
        return GetArchitectureWithId(api_impl_->GetSpec().architecture.id);
    }

    IsaDecoder::~IsaDecoder()
    {
        if (api_impl_ != nullptr)
        {
            delete api_impl_;
            api_impl_ = nullptr;
        }
    }

    // Decoder manager implementation.
    struct DecodeManager::DecodeManagerImpl
    {
        std::map<GpuArchitecture, std::shared_ptr<IsaDecoder>> arch_to_decoder;
    };

    bool DecodeManager::Initialize(const std::vector<std::string>& input_spec_file_paths, std::string& err_message)
    {
        bool should_abort = false;

        if (manager_impl_ == nullptr)
        {
            manager_impl_ = new DecodeManagerImpl();
        }
        else
        {
            delete manager_impl_;
            manager_impl_ = new DecodeManagerImpl();
        }

        if (manager_impl_ == nullptr)
        {
            should_abort = true;
            err_message  = kStringErrorManagerImplAllocationFailed;
        }

        // Initialize IsaDecoder for each XML spec paths provided.
        for (auto path_iter = input_spec_file_paths.begin(); !should_abort && path_iter != input_spec_file_paths.end(); path_iter++)
        {
            std::string                 init_err_message;
            std::shared_ptr<IsaDecoder> decoder         = std::make_shared<IsaDecoder>();
            bool                        is_decoder_init = decoder->Initialize(*path_iter, init_err_message);
            if (is_decoder_init)
            {
                GpuArchitecture architecture = decoder->GetArchitecture();
                if (architecture != GpuArchitecture::kUnknown)
                {
                    manager_impl_->arch_to_decoder[architecture] = decoder;
                }
                else
                {
                    should_abort = true;
                    err_message  = kStringErrorDecodeManagerUnknownArch;
                }
            }
            else
            {
                should_abort = true;
                std::stringstream error_ss;
                error_ss << kStringErrorDecodeManagerInitFailed << *path_iter;
                err_message = error_ss.str();
            }
        }

        return !should_abort;
    }

    std::shared_ptr<IsaDecoder> DecodeManager::GetDecoder(GpuArchitecture architecture) const
    {
        std::shared_ptr<IsaDecoder> ret = nullptr;

        // Get the decoder from the map.
        auto decoder_iter = manager_impl_->arch_to_decoder.find(architecture);
        if (decoder_iter != manager_impl_->arch_to_decoder.end())
        {
            ret = decoder_iter->second;
        }
        return ret;
    }

    DecodeManager::~DecodeManager()
    {
        if (manager_impl_ != nullptr)
        {
            delete manager_impl_;
            manager_impl_ = nullptr;
        }
    }

    bool BranchInfo::IsDirect() const
    {
        return !is_indirect;
    }

}  // namespace amdisa
