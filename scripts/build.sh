#!/bin/bash

# Build script for finmath project
# Usage: ./scripts/build.sh [debug|release|clean]

set -e  # Exit on any error

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"
BUILD_TYPE="${1:-debug}"

echo "Building finmath project..."
echo "Project root: $PROJECT_ROOT"
echo "Build type: $BUILD_TYPE"

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

case "$BUILD_TYPE" in
    "clean")
        echo "Cleaning build directory..."
        rm -rf "$BUILD_DIR"/*
        echo "Build directory cleaned."
        exit 0
        ;;
    "debug")
        CMAKE_BUILD_TYPE="Debug"
        ;;
    "release")
        CMAKE_BUILD_TYPE="Release"
        ;;
    *)
        echo "Usage: $0 [debug|release|clean]"
        echo "  debug   - Build with debug symbols (default)"
        echo "  release - Build optimized version"
        echo "  clean   - Clean build directory"
        exit 1
        ;;
esac

# Configure with CMake
echo "Configuring with CMake..."
cmake -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" "$PROJECT_ROOT"

# Build the project
echo "Building..."
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo "Build completed successfully!"
echo "Executables are in: $BUILD_DIR"
echo ""
echo "Available test executables:"
find "$BUILD_DIR" -name "*_test_executable" -o -name "*_test" | sort

