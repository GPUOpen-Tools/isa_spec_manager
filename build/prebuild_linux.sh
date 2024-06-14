#!/bin/bash
# Set some defaults.
BUILD_DIR="linux"

# Create build directory.
if [ ! -d $BUILD_DIR ]; then
    mkdir $BUILD_DIR
fi

# Run cmake.
cd $BUILD_DIR
cmake ../../

