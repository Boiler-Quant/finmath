#!/bin/bash

# Test runner script for finmath project
# Usage: ./scripts/test.sh [all|unit|specific_test_name]

set -e  # Exit on any error

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"
TEST_MODE="${1:-all}"

echo "Running finmath tests..."
echo "Project root: $PROJECT_ROOT"
echo "Test mode: $TEST_MODE"

# Check if build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "Build directory not found. Building first..."
    "$PROJECT_ROOT/scripts/build.sh" debug
fi

cd "$BUILD_DIR"

case "$TEST_MODE" in
    "all")
        echo "Running all tests with CTest..."
        ctest --output-on-failure --verbose
        ;;
    "unit")
        echo "Running unit tests individually..."
        # Run individual test executables
        for test_exe in *_test_executable; do
            if [ -f "$test_exe" ]; then
                echo "Running $test_exe..."
                ./"$test_exe"
            fi
        done
        
        # Run the comprehensive test runner
        if [ -f "run_all_tests" ]; then
            echo "Running comprehensive test runner..."
            ./run_all_tests
        fi
        ;;
    *)
        # Try to run a specific test
        if [ -f "${TEST_MODE}_executable" ]; then
            echo "Running specific test: ${TEST_MODE}_executable"
            ./"${TEST_MODE}_executable"
        elif [ -f "$TEST_MODE" ]; then
            echo "Running specific test: $TEST_MODE"
            ./"$TEST_MODE"
        else
            echo "Test '$TEST_MODE' not found."
            echo "Available tests:"
            find . -name "*_test_executable" -o -name "*_test" | sort
            exit 1
        fi
        ;;
esac

echo "Tests completed!"

