/*
 * Copyright (c) 2024 Advanced Micro Devices, Inc. All rights reserved.
 */
#include "amdisa_utility.h"

// C++ libraries.
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>

// Local libraries.
#include "amdisa_structures.h"

namespace amdisa
{
    // Warning string constants.
    static const char* kStringWarningCouldNotConvertStrToUnsigned = "Warning: could not convert string to integer: ";

    bool AmdIsaUtility::GetRange(const Field& field, Range& range, uint32_t range_order)
    {
        bool is_range_retrieved = false;
        if (!field.ranges.empty() && range_order < field.ranges.size())
        {
            range              = field.ranges[range_order];
            is_range_retrieved = true;
        }

        return is_range_retrieved;
    }

    void AmdIsaUtility::BitMapToString(const MicrocodeFormat& microcode_format, const std::vector<uint32_t>& machine_code, std::string& string_bitmap)
    {
        std::stringstream ret;
        for (const auto& field : microcode_format.bit_map)
        {
            Range range;
            if (GetRange(field, range, 0))
            {
                uint32_t bit_offset  = range.bit_offset;
                uint32_t bit_count   = range.bit_count;
                uint32_t upper       = bit_offset + bit_count - 1;
                uint32_t lower       = bit_offset;
                uint32_t dword_index = bit_offset / 32;

                ret << "    " << std::left << std::setw(13) << field.name << "[" << std::right << std::dec << std::setw(2) << upper << ":" << std::setw(2)
                    << lower << "]";

                uint32_t dword = 0;
                if (dword_index < machine_code.size())
                {
                    dword = machine_code[dword_index];
                }

                uint64_t field_val = (dword >> (bit_offset - 32 * dword_index)) & ((1 << bit_count) - 1);
                ret << " = " << std::hex << field_val;
                ret << std::endl;
            }
        }

        string_bitmap = ret.str();
    }

    std::string AmdIsaUtility::ToUpper(const std::string& str)
    {
        std::string ustr = str;
        std::transform(ustr.begin(), ustr.end(), ustr.begin(), [](const char& c) { return std::toupper(c); });
        return ustr;
    }

    std::string AmdIsaUtility::ToLower(const std::string& str)
    {
        std::string lstr = str;
        std::transform(lstr.begin(), lstr.end(), lstr.begin(), [](const char& c) { return std::tolower(c); });
        return lstr;
    }

    std::string AmdIsaUtility::Strip(const std::string& str)
    {
        auto start = std::find_if(str.begin(), str.end(), [](char c)
            {
                return !(c == ' ' || c == '\n' || c == '\r' || c == '\t');
            });

        auto end = std::find_if(str.rbegin(), str.rend(), [](char c)
            {
                return !(c == ' ' || c == '\n' || c == '\r' || c == '\t');
            }).base();

        std::string stripped_str;
        if (start < end)
        {
            stripped_str = std::string(start, end);
        }
        return stripped_str;
    }

    uint64_t AmdIsaUtility::StringToUnsignedInt(const std::string& str)
    {
        uint64_t result = 0;
        try
        {
            result = std::stoull(str);
        }
        catch (std::exception& err)
        {
            std::cout << kStringWarningCouldNotConvertStrToUnsigned << str << ". Exception: " << err.what() << std::endl;
        }
        return result;
    }

    uint64_t AmdIsaUtility::PositionValueToField(uint64_t value, const MicrocodeFormat& microcode_format, const std::string& field_name)
    {
        uint64_t ret = 0;
        uint32_t order = 0;
        uint32_t current_pos = 0;
        std::vector<uint64_t> range_values;

        const auto& field_iterator =
            std::find_if(microcode_format.bit_map.begin(), microcode_format.bit_map.end(),
                [&](const Field& field) { return field.name == field_name; });
        assert(field_iterator->range_count > 0);

        for (const auto& range : field_iterator->ranges)
        {
            // Check that the order in XML are sequential.
            assert(order == range.order);

            // Construct the mask.
            uint32_t mask = (1ULL << range.bit_count) - 1;
            uint32_t positioned_mask = mask << current_pos;

            // Get the value.
            uint64_t positioned_value = (value & positioned_mask) >> current_pos;

            // Save the retrieved value to the final result.
            order++;
            current_pos += range.bit_count;
            range_values.push_back(positioned_value << range.bit_offset);
        }

        for (const auto& range_value : range_values)
        {
            ret |= range_value;
        }

        return ret;
    }

    uint8_t AmdIsaUtility::BitCount(uint64_t n)
    {
        n = n - ((n >> 1) & 0x5555555555555555ULL);
        n = (n & 0x3333333333333333ULL) + ((n >> 2) & 0x3333333333333333ULL);
        n = (n + (n >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
        n = n + (n >> 8);
        n = n + (n >> 16);
        n = n + (n >> 32);
        return n & 0x7F;  // 7 bits for max 64
    }

}  // namespace amdisa
