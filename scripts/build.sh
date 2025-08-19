#!/bin/bash

# Build script for Advanced Image Editor

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Configuration
BUILD_TYPE=${BUILD_TYPE:-Release}
BUILD_DIR="build"
INSTALL_DIR="install"
JOBS=${JOBS:-$(nproc)}

echo -e "${GREEN}Building Advanced Image Editor...${NC}"
echo "Build type: $BUILD_TYPE"
echo "Build directory: $BUILD_DIR"
echo "Install directory: $INSTALL_DIR"
echo "Jobs: $JOBS"

# Create build directory
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# Configure with CMake
echo -e "${YELLOW}Configuring with CMake...${NC}"
cmake .. \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_INSTALL_PREFIX=../$INSTALL_DIR \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DBUILD_TESTS=ON \
    -DBUILD_DOCS=ON

# Build
echo -e "${YELLOW}Building...${NC}"
make -j$JOBS

# Install
echo -e "${YELLOW}Installing...${NC}"
make install

echo -e "${GREEN}Build completed successfully!${NC}"
echo "Executable: $INSTALL_DIR/bin/image-editor"
