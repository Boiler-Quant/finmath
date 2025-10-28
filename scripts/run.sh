#!/bin/bash

# Quick run script for finmath project
# Usage: ./scripts/run.sh [test_name]

set -e  # Exit on any error

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"
TEST_NAME="${1:-run_all_tests}"

echo "Running finmath executable..."
echo "Project root: $PROJECT_ROOT"
echo "Executable: $TEST_NAME"

# Check if build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "Build directory not found. Building first..."
    "$PROJECT_ROOT/scripts/build.sh" debug
fi

cd "$BUILD_DIR"

# Check if the executable exists
if [ ! -f "$TEST_NAME" ]; then
    echo "Executable '$TEST_NAME' not found in build directory."
    echo "Available executables:"
    find . -type f -executable | sort
    exit 1
fi

echo "Running $TEST_NAME..."
./"$TEST_NAME"

