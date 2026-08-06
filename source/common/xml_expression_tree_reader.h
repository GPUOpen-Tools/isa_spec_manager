/*
 * Copyright (c) 2026 Advanced Micro Devices, Inc. All rights reserved.
 */
#ifndef AMDISA_XML_EXPRESSION_TREE_READER_H_
#define AMDISA_XML_EXPRESSION_TREE_READER_H_

// C++ libraries.
#include <string>

// Forward declarations.
namespace tinyxml2
{
    class XMLElement;
}

namespace amdisa
{
    // Forward declarations.
    struct GenericExpressionNode;

    // Handles reading expression trees from XML parsed by tinyxml2.
    class XmlExpressionTreeReader
    {
    public:
        // Reads an expression tree from XML element.
        // Inputs xml_elem, the XML element containing the expression tree.
        // Outputs expression_root, the root node of the parsed expression tree.
        // Also outputs err_message, the error message if parsing fails.
        // Returns true if successful, false otherwise.
        static bool ReadExpressionTree(const tinyxml2::XMLElement* xml_elem,
                                       GenericExpressionNode& expression_root,
                                       std::string& err_message);
    };
}  // namespace amdisa

#endif  // AMDISA_XML_EXPRESSION_TREE_READER_H_
