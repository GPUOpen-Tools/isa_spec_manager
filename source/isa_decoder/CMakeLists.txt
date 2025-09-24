# Copyright (c) 2024 Advanced Micro Devices, Inc. All rights reserved.
cmake_minimum_required(VERSION 3.10)

# Setting project directories.
set (SRC_DIR ../../source)
set (INC_DIR ../../include)
set (THIRD_PARTY_DIR ../third_party)

# Control the path of tinyxml2.
set (TINYXML_DIR ${THIRD_PARTY_DIR}/tinyxml2)
if (TINYXML_SRC_PATH)
    message("-- IsaDecoder: Overwritting default tinyxml2 source files with ${TINYXML_SRC_PATH}")
    set (TINYXML_DIR ${TINYXML_SRC_PATH})
endif()

# Redirect to single build output location
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

# Local target: IsaDecoder.
# Setting all header files for this project.
set (ALL_H
    # Top interface header.
    ${INC_DIR}/amdisa/isa_decoder.h
    ${INC_DIR}/amdisa/api_version.h

    # Local library headers.
    ${SRC_DIR}/common/amdisa_expression_tree_consts.h
    ${SRC_DIR}/common/amdisa_structures.h
    ${SRC_DIR}/common/amdisa_xml_element_consts.h
    ${SRC_DIR}/common/amdisa_utility.h
    ${SRC_DIR}/common/isa_xml_reader.h

    # Third party library headers.
    ${TINYXML_DIR}/tinyxml2.h
)

# Setting all source files for this project.
set (ALL_CPP
    # Top interface source file.
    ./isa_decoder.cpp
    ./encoding_condition_handler.hpp

    # Local library source files.
    ${SRC_DIR}/common/amdisa_utility.cpp
    ${SRC_DIR}/common/amdisa_structures.cpp
    ${SRC_DIR}/common/isa_xml_reader.cpp

    # Third party source files.
    ${TINYXML_DIR}/tinyxml2.cpp
)

# Adding target.
# Static Library
add_library(isa_decoder STATIC ${ALL_CPP})
target_include_directories(isa_decoder PRIVATE
    ${SRC_DIR}/common
    ${TINYXML_DIR}
    PUBLIC
    ${INC_DIR}
)

# Dynamic Library
add_library(isa_decoder_dynamic SHARED ${ALL_CPP})
target_include_directories(isa_decoder_dynamic PRIVATE
    ${SRC_DIR}/common
    ${TINYXML_DIR}
    PUBLIC
    ${INC_DIR}
)

# install public headers
install(DIRECTORY ${INC_DIR}/amdisa DESTINATION include COMPONENT dev)

install(TARGETS isa_decoder EXPORT isa_decoderTargets
        ARCHIVE DESTINATION lib COMPONENT binary
        LIBRARY DESTINATION lib COMPONENT binary
)
install(TARGETS isa_decoder_dynamic EXPORT isa_decoder_dynamicTargets

        ARCHIVE DESTINATION lib COMPONENT binary
        LIBRARY DESTINATION lib COMPONENT binary
)


# Group these projects under an Examples folder.
set_target_properties(isa_decoder isa_decoder_dynamic PROPERTIES FOLDER "libs")
