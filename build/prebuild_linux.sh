#!/bin/bash
# Set some defaults.
BUILD_DIR="linux"
TINYXML_SRC_PATH=""
EXCLUDE_DECODER=""
EXCLUDE_EXPLORER=""
EXCLUDE_CLI=""
EXCLUDE_EXAMPLES=""
EXCLUDE_TESTS=""

# Print help message.
print_help() {
    echo ""
    echo "This script generates a makefile for IsaSpecManager on Linux."
    echo ""
    echo "Usage:  prebuild_linux.sh [options]"
    echo ""
    echo "Options:"
    echo "   --tinyxml2_src_path=<path>  Path to TinyXML2 source. If not specified, the default TinyXML2 bundled with the project will be used."
    echo "   --no-decoder                Exclude the ISA decoder library from the build."
    echo "   --no-explorer               Exclude the ISA explorer library from the build."
    echo "   --no-cli                    Exclude the command line interface from the build."
    echo "   --no-examples               Exclude the examples from the build."
    echo "   --no-tests                  Exclude the tests from the build."
    echo ""
    echo "Examples:"
    echo "   ./prebuild_linux.sh"
    echo "   ./prebuild_linux.sh --tinyxml2_src_path=/path/to/tinyxml2"
    echo "   ./prebuild_linux.sh --no-explorer --no-cli --no-examples --no-tests"
    echo "   ./prebuild_linux.sh --no-decoder --no-cli --no-examples --no-tests"
}

# Parse arguments.
for arg in "$@"; do
    case $arg in
        -h|--help)
        print_help
        exit 0
        ;;
        --tinyxml2_src_path=*)
        TINYXML_SRC_PATH="${arg#*=}"
        ;;
        --no-decoder)
        EXCLUDE_DECODER="-DEXCLUDE_ISA_DECODER=ON"
        ;;
        --no-explorer)
        EXCLUDE_EXPLORER="-DEXCLUDE_ISA_EXPLORER=ON"
        ;;
        --no-cli)
        EXCLUDE_CLI="-DEXCLUDE_ISA_CLI=ON"
        ;;
        --no-examples)
        EXCLUDE_EXAMPLES="-DEXCLUDE_ISA_EXAMPLES=ON"
        ;;
        --no-tests)
        EXCLUDE_TESTS="-DEXCLUDE_ISA_TESTS=ON"
        ;;
        *)
        echo "Unknown option: $arg"
        print_help
        exit 1
        ;;
    esac
done

# Create build directory.
if [ ! -d $BUILD_DIR ]; then
    mkdir $BUILD_DIR
fi

# Go to the build directory.
cd $BUILD_DIR

# Build the cmake flags.
CMAKE_FLAGS="-DCMAKE_EXPORT_COMPILE_COMMANDS=1"
CMAKE_FLAGS="$CMAKE_FLAGS $EXCLUDE_DECODER $EXCLUDE_EXPLORER $EXCLUDE_CLI $EXCLUDE_EXAMPLES $EXCLUDE_TESTS"
if [ -n "$TINYXML_SRC_PATH" ]; then
    CMAKE_FLAGS="$CMAKE_FLAGS -DTINYXML_SRC_PATH=$TINYXML_SRC_PATH"
fi

cmake $CMAKE_FLAGS ../../
