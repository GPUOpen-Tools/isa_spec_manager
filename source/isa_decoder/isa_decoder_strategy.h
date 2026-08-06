/*
 * Copyright (c) 2026 Advanced Micro Devices, Inc. All rights reserved.
 */
#ifndef ISA_DECODER_STRATEGY_H_
#define ISA_DECODER_STRATEGY_H_

#include "amdisa/isa_decoder.h"

namespace amdisa
{
    struct IsaSpec;

    // Strategy interface for versioned decode logic.
    class IIsaDecoderStrategy
    {
    public:
        virtual ~IIsaDecoderStrategy() = 0;

        virtual bool InitializeFromSpec(IsaSpec&& spec, std::string& err) = 0;
        virtual GpuArchitecture GetArchitecture() const = 0;
        virtual bool DecodeInstruction(uint64_t machine_code, InstructionInfoBundle& info, std::string& err) const = 0;
        virtual bool DecodeInstruction(const std::string& name, InstructionInfo& info, std::string& err) const = 0;
        virtual bool DecodeInstructionStream(const std::vector<uint32_t>& stream,
                                             std::vector<InstructionInfoBundle>& info,
                                             std::string& err) const = 0;
        virtual bool DecodeShaderDisassemblyText(const std::string& text,
                                                  std::vector<InstructionInfoBundle>& info,
                                                  std::string& err,
                                                  bool resolve) const = 0;
        virtual bool DecodeShaderDisassemblyFile(const std::string& file,
                                                  std::vector<InstructionInfoBundle>& info,
                                                  std::string& err,
                                                  bool resolve) const = 0;
    };
    inline IIsaDecoderStrategy::~IIsaDecoderStrategy() = default;

}  // namespace amdisa
#endif  // ISA_DECODER_STRATEGY_H_
