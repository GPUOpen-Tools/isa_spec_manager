/*
 * Copyright (c) 2024 Advanced Micro Devices, Inc. All rights reserved.
 */
#ifndef ISA_XML_READER_H_
#define ISA_XML_READER_H_

// C++ libraries.
#include <string>

namespace amdisa
{
    // Forward declarations.
    struct IsaSpec;

    class IsaXmlReader
    {
    public:
        /*
         * ReadSpec --
         *
         * Reads an ISA Spec XML from a filepath and loads the spec data into an IsaSpec.
         *
         * Returns true on success or false on error, in which case err_message is populated with an appropriate human-readable error string.
         */
        static bool ReadSpec(const std::string& path_to_input_xml, IsaSpec& spec_data, std::string& err_message);

        /*
         * ReadSpec --
         *
         * Reads an ISA Spec XML from a buffer in memory (of length datalen) and loads the spec data into an IsaSpec.
         *
         * Returns true on success or false on error, in which case err_message is populated with an appropriate human-readable error string.
         */
        static bool ReadSpec(const char *input_xml_data, const size_t datalen, IsaSpec& spec_data, std::string& err_message);

        // Explicitly remove the constructors and assignment operators.
        IsaXmlReader()                               = delete;
        IsaXmlReader(const IsaXmlReader&)            = delete;
        IsaXmlReader& operator=(const IsaXmlReader&) = delete;
    };
}  // namespace amdisa
#endif  // ISA_XML_READER_H_
