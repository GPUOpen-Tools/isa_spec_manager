/*
 * Copyright (c) 2026 Advanced Micro Devices, Inc. All rights reserved.
 */
#include "xml_expression_tree_reader.h"

// C++ libraries.
#include <memory>
#include <set>

// Third party libraries.
#include <tinyxml2.h>

// Local libraries.
#include "amdisa_structures.h"

namespace amdisa
{
    // XML element names
    static const char* const kXmlElemId      = "id";
    static const char* const kXmlElemLit     = "lit";
    static const char* const kXmlElemType    = "type";
    static const char* const kXmlElemT       = "t";
    static const char* const kXmlElemArr     = "arr";
    static const char* const kXmlElemLambda  = "lambda";
    static const char* const kXmlElemParams  = "params";
    static const char* const kXmlElemRet     = "ret";
    static const char* const kXmlElemRec     = "rec";
    static const char* const kXmlElemF       = "f";
    static const char* const kXmlElemOp      = "op";
    static const char* const kXmlElemTy      = "ty";

    // XML attribute names
    static const char* const kXmlAttrVal     = "val";
    static const char* const kXmlAttrBase    = "base";
    static const char* const kXmlAttrSize    = "size";
    static const char* const kXmlAttrName    = "name";
    static const char* const kXmlAttrType    = "type";

    // Node types
    static const char* const kNodeIdentifier = "identifier";
    static const char* const kNodeLiteral    = "literal";
    static const char* const kNodeType       = "type";
    static const char* const kNodeTypeInfo   = "type_info";

    // Type system constants
    static const char* const kTypeBaseType     = "base_type";
    static const char* const kTypeSize         = "size";
    static const char* const kTypeArrayType    = "array_type";
    static const char* const kTypeArraySize    = "array_size";
    static const char* const kTypeLambdaType   = "lambda_type";
    static const char* const kTypeLambdaParams = "lambda_params";
    static const char* const kTypeLambdaReturn = "lambda_return";
    static const char* const kTypeRecordType   = "record_type";
    static const char* const kTypeRecordFields = "record_fields";
    static const char* const kTypeField        = "field";

    // Base type names
    static const char* const kBaseB  = "B";   // Bits
    static const char* const kBaseU  = "U";   // Unsigned
    static const char* const kBaseI  = "I";   // Signed/Integer
    static const char* const kBaseF  = "F";   // Float
    static const char* const kBaseFP = "FP";  // Floating Point (special precision)
    static const char* const kBaseBF = "BF";  // BFloat16
    static const char* const kBaseS  = "S";   // String

    // Known base types
    static const std::set<std::string> kKnownBaseTypes = {
        kBaseB,   // Bits
        kBaseU,   // Unsigned
        kBaseI,   // Signed/Integer
        kBaseF,   // Float
        kBaseFP,  // Floating Point (special precision)
        kBaseBF,  // BFloat16
        kBaseS    // String
    };

    // Common operators
    static const char* const kOpPragma         = ":pragma";
    static const char* const kOpSeq            = ":seq";
    static const char* const kOpIf             = ":if";
    static const char* const kOpFor            = ":for";
    static const char* const kOpWhile          = ":while";
    static const char* const kOpBreak          = ":break";
    static const char* const kOpDeclare        = ":declare";
    static const char* const kOpComment        = ":comment";
    static const char* const kOpReturn         = ":return";
    static const char* const kOpEval           = ":eval";
    static const char* const kOpInstOperand    = ".instoperand";
    static const char* const kOpCast           = ".cast";
    static const char* const kOpCall           = ".call";
    static const char* const kOpLambda         = ".lambda";
    static const char* const kOpArraySlice     = ".arrayslice";
    static const char* const kOpArrayDeref     = ".arrayderef";
    static const char* const kOpBitCat         = ".bitcat";
    static const char* const kOpSign           = ".sign";
    static const char* const kOpExponent       = ".exponent";
    static const char* const kOpMantissa       = ".mantissa";
    static const char* const kOpAbs            = ".abs";
    static const char* const kOpUMinus         = ".uminus";
    static const char* const kOpUPlus          = ".uplus";
    static const char* const kOpLdexp          = ".ldexp";
    static const char* const kOpFieldDeref     = ".fieldderef";
    static const char* const kOpWithin         = ".within";
    static const char* const kOpNotWithin      = ".notwithin";
    static const char* const kOpBitNeg         = ".bitneg";
    static const char* const kOpSignExt        = ".signext";
    static const char* const kOpSignExtFromBit = ".signext_from_bit";
    static const char* const kOpArraySliceSize = ".arrayslicesize";
    static const char* const kOpFloor          = ".floor";
    static const char* const kOpTrunc          = ".trunc";
    static const char* const kOpPow            = ".pow";
    static const char* const kOpLog2           = ".log2";
    static const char* const kOpSqrt           = ".sqrt";
    static const char* const kOpSin            = ".sin";
    static const char* const kOpCos            = ".cos";
    static const char* const kOpFma            = ".fma";
    static const char* const kOpFract          = ".fract";
    static const char* const kOpBoolNeg        = ".boolneg";
    static const char* const kOpConsArray      = ".cons_array";

    // Arithmetic operators
    static const char* const kOpPlus           = "+";
    static const char* const kOpSum            = ".sum";
    static const char* const kOpMinus          = "-";
    static const char* const kOpMultiply       = "*";
    static const char* const kOpProd           = ".prod";
    static const char* const kOpDivide         = "/";
    static const char* const kOpModulo         = "%";
    static const char* const kOpPower          = "**";
    static const char* const kOpPlusAssign     = "+=";
    static const char* const kOpMinusAssign    = "-=";
    static const char* const kOpMultiplyAssign = "*=";
    static const char* const kOpDivideAssign   = "/=";
    static const char* const kOpShiftRight     = ">>";
    static const char* const kOpShiftLeft      = "<<";
    static const char* const kOpBitwiseAnd     = "&";
    static const char* const kOpBitwiseOr      = "|";
    static const char* const kOpBitwiseXor     = "^";
    static const char* const kOpBitwiseNot     = "~";

    // Comparison operators
    static const char* const kOpEqual        = "==";
    static const char* const kOpNotEqual     = "!=";
    static const char* const kOpLess         = "<";
    static const char* const kOpGreater      = ">";
    static const char* const kOpLessEqual    = "<=";
    static const char* const kOpGreaterEqual = ">=";
    static const char* const kOpNotEqualAlt  = "<>";

    // Logical operators
    static const char* const kOpLogicalAnd = "&&";
    static const char* const kOpLogicalOr  = "||";
    static const char* const kOpLogicalNot = "!";

    // Assignment
    static const char* const kOpAssign = "=";

    // Ternary
    static const char* const kOpTernary = "?:";

    // Error messages
    static const char* const kStringErrorNullXmlElement = "Error: Null XML element";
    static const char* const kStringErrorUnknownBaseType = "Error: Unknown base type: ";
    static const char* const kStringErrorMissingTypeAttribute = "Error: Operator node missing 'type' attribute";
    static const char* const kStringErrorUnknownOperator = "Error: Unknown operator: ";
    static const char* const kStringErrorUnknownXmlElement = "Error: Unknown XML element: ";
    static const char* const kStringErrorNullXmlElementProvided = "Error: Null XML element provided";
    static const char* const kStringErrorNoChildElement = "Error: No child element found in XML";

    // Gather all operators into a set.
    static const std::set<std::string> kKnownOperators = {
        // Control flow
        kOpPragma,
        kOpSeq,
        kOpIf,
        kOpFor,
        kOpDeclare,
        kOpComment,
        kOpReturn,
        kOpEval,
        kOpWhile,
        kOpBreak,

        // Member access and operations
        kOpInstOperand,
        kOpCast,
        kOpCall,
        kOpLambda,
        kOpArraySlice,
        kOpArrayDeref,
        kOpBitCat,
        kOpSign,
        kOpExponent,
        kOpMantissa,
        kOpAbs,
        kOpUMinus,
        kOpUPlus,
        kOpLdexp,
        kOpFieldDeref,
        kOpWithin,
        kOpNotWithin,
        kOpBitNeg,
        kOpSignExt,
        kOpSignExtFromBit,
        kOpArraySliceSize,

        // Math functions
        kOpFloor,
        kOpTrunc,
        kOpPow,
        kOpLog2,
        kOpSqrt,
        kOpSin,
        kOpCos,
        kOpFma,
        kOpFract,

        // Other operators
        kOpBoolNeg,
        kOpConsArray,

        // Arithmetic operators
        kOpPlus,
        kOpSum,
        kOpMinus,
        kOpMultiply,
        kOpProd,
        kOpDivide,
        kOpModulo,
        kOpPower,
        kOpPlusAssign,
        kOpMinusAssign,
        kOpMultiplyAssign,
        kOpDivideAssign,
        kOpShiftRight,
        kOpShiftLeft,
        kOpBitwiseAnd,
        kOpBitwiseOr,
        kOpBitwiseXor,
        kOpBitwiseNot,

        // Comparison operators
        kOpEqual,
        kOpNotEqual,
        kOpLess,
        kOpGreater,
        kOpLessEqual,
        kOpGreaterEqual,
        kOpNotEqualAlt,

        // Logical operators
        kOpLogicalAnd,
        kOpLogicalOr,
        kOpLogicalNot,

        // Assignment
        kOpAssign,

        // Ternary
        kOpTernary};

    // Forward declaration
    static std::shared_ptr<GenericExpressionNode> ParseXmlElement(const tinyxml2::XMLElement* element, std::string& err_message);

    // Helper function to parse XML element into expression node
    static std::shared_ptr<GenericExpressionNode> ParseXmlElement(const tinyxml2::XMLElement* element, std::string& err_message)
    {
        if (!element)
        {
            err_message = kStringErrorNullXmlElement;
            return nullptr;
        }

        auto node = std::make_shared<GenericExpressionNode>();
        std::string elem_name = element->Name();

        // Handle identifier nodes: <id val="varname"/>
        if (elem_name == kXmlElemId)
        {
            node->expression_operator = kNodeIdentifier;
            const char* val = element->Attribute(kXmlAttrVal);
            if (val)
            {
                auto id_node = std::make_shared<GenericExpressionNode>();
                id_node->expression_operator = val;
                node->children.push_back(id_node);
            }

            // Parse type info if present
            const tinyxml2::XMLElement* ty_elem = element->FirstChildElement(kXmlElemTy);
            if (ty_elem)
            {
                auto type_info_node = std::make_shared<GenericExpressionNode>();
                type_info_node->expression_operator = kNodeTypeInfo;

                const tinyxml2::XMLElement* type_child = ty_elem->FirstChildElement();
                if (type_child)
                {
                    auto type_node = ParseXmlElement(type_child, err_message);
                    if (!type_node)
                    {
                        return nullptr;
                    }
                    type_node->is_ret_type_node = true;
                    type_info_node->children.push_back(type_node);
                }

                node->children.push_back(type_info_node);
            }
        }
        // Handle literal nodes: <lit val="123"/>
        else if (elem_name == kXmlElemLit)
        {
            node->expression_operator = kNodeLiteral;
            const char* val = element->Attribute(kXmlAttrVal);
            if (val)
            {
                auto lit_node = std::make_shared<GenericExpressionNode>();
                lit_node->expression_operator = val;
                node->children.push_back(lit_node);
            }

            // Parse type info if present
            const tinyxml2::XMLElement* ty_elem = element->FirstChildElement(kXmlElemTy);
            if (ty_elem)
            {
                auto type_info_node = std::make_shared<GenericExpressionNode>();
                type_info_node->expression_operator = kNodeTypeInfo;

                const tinyxml2::XMLElement* type_child = ty_elem->FirstChildElement();
                if (type_child)
                {
                    auto type_node = ParseXmlElement(type_child, err_message);
                    if (!type_node)
                    {
                        return nullptr;
                    }
                    type_node->is_ret_type_node = true;
                    type_info_node->children.push_back(type_node);
                }

                node->children.push_back(type_info_node);
            }
        }
        // Handle type expression nodes: <type>
        else if (elem_name == kXmlElemType)
        {
            node->expression_operator = kNodeType;
            node->is_ret_type_node = true;

            // Parse the child type element
            const tinyxml2::XMLElement* child = element->FirstChildElement();
            if (child)
            {
                auto type_node = ParseXmlElement(child, err_message);
                if (!type_node)
                {
                    return nullptr;
                }
                type_node->is_ret_type_node = true;
                node->children.push_back(type_node);
            }
        }
        // Handle base type nodes: <t base="U" size="32"/>
        else if (elem_name == kXmlElemT)
        {
            node->expression_operator = kTypeBaseType;
            node->is_ret_type_node = true;

            const char* base = element->Attribute(kXmlAttrBase);
            if (base)
            {
                // Validate base type
                if (kKnownBaseTypes.find(base) == kKnownBaseTypes.end())
                {
                    err_message = kStringErrorUnknownBaseType + std::string(base);
                    return nullptr;
                }

                auto base_node = std::make_shared<GenericExpressionNode>();
                base_node->expression_operator = base;
                node->children.push_back(base_node);
            }

            const char* size = element->Attribute(kXmlAttrSize);
            if (size)
            {
                auto size_node = std::make_shared<GenericExpressionNode>();
                size_node->expression_operator = kTypeSize;
                auto size_val = std::make_shared<GenericExpressionNode>();
                size_val->expression_operator = size;
                size_node->children.push_back(size_val);
                node->children.push_back(size_node);
            }
        }
        // Handle array type nodes: <arr size="10">...</arr>
        else if (elem_name == kXmlElemArr)
        {
            node->expression_operator = kTypeArrayType;
            node->is_ret_type_node = true;

            // Parse element type (first child)
            const tinyxml2::XMLElement* child = element->FirstChildElement();
            if (child)
            {
                auto elem_type = ParseXmlElement(child, err_message);
                if (!elem_type)
                {
                    return nullptr;
                }
                elem_type->is_ret_type_node = true;
                node->children.push_back(elem_type);
            }

            // Parse array size
            const char* size = element->Attribute(kXmlAttrSize);
            if (size)
            {
                auto size_node = std::make_shared<GenericExpressionNode>();
                size_node->expression_operator = kTypeArraySize;
                auto size_val = std::make_shared<GenericExpressionNode>();
                size_val->expression_operator = size;
                size_node->children.push_back(size_val);
                node->children.push_back(size_node);
            }
        }
        // Handle lambda type nodes: <lambda>
        else if (elem_name == kXmlElemLambda)
        {
            node->expression_operator = kTypeLambdaType;
            node->is_ret_type_node = true;

            // Parse params and return type
            for (const tinyxml2::XMLElement* child = element->FirstChildElement();
                 child; child = child->NextSiblingElement())
            {
                std::string child_name = child->Name();

                if (child_name == kXmlElemParams)
                {
                    auto params_node = std::make_shared<GenericExpressionNode>();
                    params_node->expression_operator = kTypeLambdaParams;

                    // Parse each parameter
                    for (const tinyxml2::XMLElement* param = child->FirstChildElement();
                         param; param = param->NextSiblingElement())
                    {
                        auto param_type = ParseXmlElement(param, err_message);
                        if (!param_type)
                        {
                            return nullptr;
                        }
                        param_type->is_ret_type_node = true;
                        params_node->children.push_back(param_type);
                    }

                    node->children.push_back(params_node);
                }
                else if (child_name == kXmlElemRet)
                {
                    auto ret_node = std::make_shared<GenericExpressionNode>();
                    ret_node->expression_operator = kTypeLambdaReturn;

                    // Parse return type
                    const tinyxml2::XMLElement* ret_type_elem = child->FirstChildElement();
                    if (ret_type_elem)
                    {
                        auto ret_type = ParseXmlElement(ret_type_elem, err_message);
                        if (!ret_type)
                        {
                            return nullptr;
                        }
                        ret_type->is_ret_type_node = true;
                        ret_node->children.push_back(ret_type);
                    }

                    node->children.push_back(ret_node);
                }
            }
        }
        // Handle record type nodes: <rec>
        else if (elem_name == kXmlElemRec)
        {
            node->expression_operator = kTypeRecordType;
            node->is_ret_type_node = true;

            auto fields_node = std::make_shared<GenericExpressionNode>();
            fields_node->expression_operator = kTypeRecordFields;

            // Parse fields
            for (const tinyxml2::XMLElement* field = element->FirstChildElement(kXmlElemF);
                 field; field = field->NextSiblingElement(kXmlElemF))
            {
                auto field_node = std::make_shared<GenericExpressionNode>();
                field_node->expression_operator = kTypeField;

                // Field name
                const char* name = field->Attribute(kXmlAttrName);
                if (name)
                {
                    auto name_node = std::make_shared<GenericExpressionNode>();
                    name_node->expression_operator = name;
                    field_node->children.push_back(name_node);
                }

                // Field type
                const tinyxml2::XMLElement* field_type_elem = field->FirstChildElement();
                if (field_type_elem)
                {
                    auto field_type = ParseXmlElement(field_type_elem, err_message);
                    if (!field_type)
                    {
                        return nullptr;
                    }
                    field_type->is_ret_type_node = true;
                    field_node->children.push_back(field_type);
                }

                fields_node->children.push_back(field_node);
            }

            node->children.push_back(fields_node);
        }
        // Handle operator nodes: <op type="operator">
        else if (elem_name == kXmlElemOp)
        {
            const char* op_type = element->Attribute(kXmlAttrType);
            if (!op_type)
            {
                err_message = kStringErrorMissingTypeAttribute;
                return nullptr;
            }

            // Validate operator
            if (kKnownOperators.find(op_type) == kKnownOperators.end())
            {
                err_message = kStringErrorUnknownOperator + std::string(op_type);
                return nullptr;
            }

            node->expression_operator = op_type;

            // Parse children (but skip <ty> elements for now)
            for (const tinyxml2::XMLElement* child = element->FirstChildElement();
                 child; child = child->NextSiblingElement())
            {
                std::string child_name = child->Name();
                if (child_name != kXmlElemTy)
                {
                    auto child_node = ParseXmlElement(child, err_message);
                    if (!child_node)
                    {
                        return nullptr;
                    }
                    node->children.push_back(child_node);
                }
            }

            // Now parse type info if present
            const tinyxml2::XMLElement* ty_elem = element->FirstChildElement(kXmlElemTy);
            if (ty_elem)
            {
                auto type_info_node = std::make_shared<GenericExpressionNode>();
                type_info_node->expression_operator = kNodeTypeInfo;

                const tinyxml2::XMLElement* type_child = ty_elem->FirstChildElement();
                if (type_child)
                {
                    auto type_node = ParseXmlElement(type_child, err_message);
                    if (!type_node)
                    {
                        return nullptr;
                    }
                    type_node->is_ret_type_node = true;
                    type_info_node->children.push_back(type_node);
                }

                node->children.push_back(type_info_node);
            }
        }
        else
        {
            err_message = kStringErrorUnknownXmlElement + elem_name;
            return nullptr;
        }

        return node;
    }

    bool XmlExpressionTreeReader::ReadExpressionTree(const tinyxml2::XMLElement* xml_elem,
                                                     GenericExpressionNode& expression_root,
                                                     std::string& err_message)
    {
        bool success = false;

        if (!xml_elem)
        {
            err_message = kStringErrorNullXmlElementProvided;
        }
        else
        {
            // The xml_elem should contain the expression tree as its first child
            const tinyxml2::XMLElement* root_elem = xml_elem->FirstChildElement();
            if (!root_elem)
            {
                err_message = kStringErrorNoChildElement;
            }
            else
            {
                // Parse the XML into expression tree
                auto parsed_node = ParseXmlElement(root_elem, err_message);
                if (parsed_node)
                {
                    expression_root = *parsed_node;
                    success = true;
                }
            }
        }

        return success;
    }
}  // namespace amdisa
