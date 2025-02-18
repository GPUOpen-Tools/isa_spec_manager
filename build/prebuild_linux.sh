#!/bin/bash
# Set some defaults.
BUILD_DIR="linux"

# Create build directory.
if [ ! -d $BUILD_DIR ]; then
    mkdir $BUILD_DIR
fi

# Read the custom path for tinyxml.
for arg in "$@"; do
    case $arg in
        --tinyxml2_src_path=*)
        TINYXML_SRC_PATH="${arg#*=}" # Extract the value after '='
        shift # Remove the argument from the list
        ;;
        *)
        echo "Unknown option: $arg"
        exit 1
        ;;
    esac
done

# Go to the build directory.
cd $BUILD_DIR

# Check if tinyxml2 needs to be overwritten.
if [ -z "$TINYXML_SRC_PATH" ]; then
    # Use default tinyxml2.
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../../
else
    # Use the custom tinyxml2.
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DTINYXML_SRC_PATH=$TINYXML_SRC_PATH ../../
fi

