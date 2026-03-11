#!/bin/bash

# FXBL Build Script
# Builds the entire project with CMake

set -e

echo "╔═══════════════════════════════════════════════════════╗"
echo "║   FXBL Build System                                   ║"
echo "╚═══════════════════════════════════════════════════════╝"
echo ""

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Configuration
BUILD_TYPE=${BUILD_TYPE:-Release}
ENABLE_AVX2=${ENABLE_AVX2:-OFF}
ENABLE_AVX512=${ENABLE_AVX512:-OFF}

echo -e "${YELLOW}[Build Configuration]${NC}"
echo "  Build Type: $BUILD_TYPE"
echo "  AVX2: $ENABLE_AVX2"
echo "  AVX-512: $ENABLE_AVX512"
echo ""

# Create build directory
echo -e "${YELLOW}[1/4] Creating build directory...${NC}"
mkdir -p build
cd build

# Run CMake
echo -e "${YELLOW}[2/4] Running CMake configuration...${NC}"
cmake .. \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DENABLE_AVX2=$ENABLE_AVX2 \
    -DENABLE_AVX512=$ENABLE_AVX512 \
    -DBUILD_TESTS=ON \
    -DBUILD_BENCHMARKS=ON \
    -DBUILD_EXAMPLES=ON

if [ $? -ne 0 ]; then
    echo -e "${RED}CMake configuration failed!${NC}"
    exit 1
fi

# Build
echo -e "${YELLOW}[3/4] Building NeuroCrypt-X...${NC}"
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi

# Test
echo -e "${YELLOW}[4/4] Running tests...${NC}"
ctest --output-on-failure

echo ""
echo -e "${GREEN}✓ Build completed successfully!${NC}"
echo ""
echo "Executables location: build/"
echo "  - fxbl-bin (main demo)"
echo "  - example_basic (basic usage)"
echo "  - example_homomorphic (homomorphic ops)"
echo ""
