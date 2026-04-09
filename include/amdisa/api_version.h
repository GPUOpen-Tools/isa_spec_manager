/*
 * Copyright (c) 2025 Advanced Micro Devices, Inc. All rights reserved.
 */
#ifndef API_VERSION_H_
#define API_VERSION_H_

// C++ libraries.
#include <string>
#include <sstream>

// IsaDecoder version.
#define AMDISA_DECODE_API_VERSION_MAJOR 1
#define AMDISA_DECODE_API_VERSION_MINOR 2
#define AMDISA_DECODE_API_VERSION_PATCH 0

// IsaExplorer version.
#define AMDISA_EXPLORE_API_VERSION_MAJOR 1
#define AMDISA_EXPLORE_API_VERSION_MINOR 0
#define AMDISA_EXPLORE_API_VERSION_PATCH 0

namespace amdisa
{
    // Compatibility check related code.
    static const char* kStringErrorNotCompatibleXml = "Error: Unsupported XML schema version. Check for compatible XML schema. Please refer to XML schema changelog or API release notes for more details.";
    static const char* kStringWarningPartiallyCompatibleXmlPrefix = "Warning: detected XML schema version ";
    static const char* kStringWarningPartiallyCompatibleXmlSuffix = " which does not support certain features that are supported by this API version. Please refer to XML schema changelog or API release notes for more details.";

    // Structure to return API version.
    class ApiVersion
    {
    public:
        static int GetMajor()
        {
            return AMDISA_DECODE_API_VERSION_MAJOR;
        }

        static int GetMinor()
        {
            return AMDISA_DECODE_API_VERSION_MINOR;
        }

        static int GetPatch()
        {
            return AMDISA_DECODE_API_VERSION_PATCH;
        }

        static std::string GetVersion()
        {
            std::stringstream api_version;
            api_version << GetMajor() << "." << GetMinor() << "." << GetPatch();
            return api_version.str();
        }

        static bool IsAtLeast(int major, int minor, int patch)
        {
            bool is_older = false;
            is_older =
                (major < GetMajor() || (major == GetMajor() && minor < GetMinor()) || (major == GetMajor() && minor == GetMinor() && patch < GetPatch()));
            return !is_older;
        }

        static bool IsCompatible(const std::string& xml_schema_version, std::string& err_message)
        {
            if (xml_schema_version < GetVersion())
            {
                std::stringstream err_message_ss;
                err_message_ss << kStringWarningPartiallyCompatibleXmlPrefix << xml_schema_version << kStringWarningPartiallyCompatibleXmlSuffix;
                err_message = err_message_ss.str();
            }
            return true;
        }
    };

    namespace explorer
    {
        // Structure to return API version.
        class ApiVersion
        {
        public:
            static int GetMajor()
            {
                return AMDISA_EXPLORE_API_VERSION_MAJOR;
            }

            static int GetMinor()
            {
                return AMDISA_EXPLORE_API_VERSION_MINOR;
            }

            static int GetPatch()
            {
                return AMDISA_EXPLORE_API_VERSION_PATCH;
            }

            static std::string GetVersion()
            {
                std::stringstream api_version;
                api_version << GetMajor() << "." << GetMinor() << "." << GetPatch();
                return api_version.str();
            }

            static bool IsAtLeast(int major, int minor, int patch)
            {
                bool is_older = false;
                is_older =
                    (major < GetMajor() || (major == GetMajor() && minor < GetMinor()) || (major == GetMajor() && minor == GetMinor() && patch < GetPatch()));
                return !is_older;
            }

            static bool IsCompatible(const std::string& xml_schema_version, std::string& err_message)
            {
                // IsaExplorer supports all currently released XML specifications.
                return true;
            }
        };
    }
}  // namespace amdisa

#endif  // API_VERSION_H_
