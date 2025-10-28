#!/bin/bash

# Debug script for finmath project
# Usage: ./scripts/debug.sh [test_name] [gdb|lldb]

set -e  # Exit on any error

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"
TEST_NAME="${1:-run_all_tests}"
DEBUGGER="${2:-gdb}"

echo "Debugging finmath project..."
echo "Project root: $PROJECT_ROOT"
echo "Test: $TEST_NAME"
echo "Debugger: $DEBUGGER"

# Check if build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "Build directory not found. Building first..."
    "$PROJECT_ROOT/scripts/build.sh" debug
fi

cd "$BUILD_DIR"

# Check if the test executable exists
if [ ! -f "$TEST_NAME" ]; then
    echo "Test executable '$TEST_NAME' not found in build directory."
    echo "Available executables:"
    find . -type f -executable | sort
    exit 1
fi

# Check if debugger is available
case "$DEBUGGER" in
    "gdb")
        if ! command -v gdb &> /dev/null; then
            echo "GDB not found. Please install it with: brew install gdb"
            exit 1
        fi
        echo "Starting GDB debugger..."
        gdb ./"$TEST_NAME"
        ;;
    "lldb")
        if ! command -v lldb &> /dev/null; then
            echo "LLDB not found. It should be available with Xcode command line tools."
            exit 1
        fi
        echo "Starting LLDB debugger..."
        lldb ./"$TEST_NAME"
        ;;
    *)
        echo "Unknown debugger: $DEBUGGER"
        echo "Available debuggers: gdb, lldb"
        exit 1
        ;;
esac

