# Development Setup Guide

This guide explains how to set up and use the C++ development environment for the finmath project.

## Prerequisites

- macOS (tested on macOS Sequoia)
- Homebrew package manager
- Xcode Command Line Tools
- VS Code (recommended) or any C++ IDE

## Installed Tools

The following tools have been installed and configured:

- **CMake 4.1.1** - Build system generator
- **GDB 16.3** - GNU Debugger for C++ debugging
- **LLDB** - LLVM Debugger (comes with Xcode)
- **Clang++** - C++ compiler

## Project Structure

```
finmath/
├── build/                 # Build output directory
├── scripts/              # Helper scripts
│   ├── build.sh         # Build script
│   ├── test.sh          # Test runner script
│   ├── debug.sh         # Debug script
│   └── run.sh           # Quick run script
├── .vscode/             # VS Code configuration
│   ├── launch.json      # Debug configurations
│   ├── tasks.json       # Build tasks
│   └── settings.json    # Editor settings
├── include/             # Header files
├── src/                 # Source files
└── test/                # Test files
```

## Quick Start

### 1. Build the Project

```bash
# Debug build (default)
./scripts/build.sh debug

# Release build
./scripts/build.sh release

# Clean build directory
./scripts/build.sh clean
```

### 2. Run Tests

```bash
# Run all tests with CTest
./scripts/test.sh all

# Run tests individually
./scripts/test.sh unit

# Run a specific test
./scripts/test.sh RSITest
```

### 3. Debug

```bash
# Debug with GDB
./scripts/debug.sh run_all_tests gdb

# Debug with LLDB
./scripts/debug.sh run_all_tests lldb

# Debug a specific test
./scripts/debug.sh RSITest_executable lldb
```

### 4. Quick Run

```bash
# Run the main test suite
./scripts/run.sh run_all_tests

# Run a specific test
./scripts/run.sh RSITest_executable
```

## VS Code Integration

### Debugging

1. Open the project in VS Code
2. Go to the Debug panel (Ctrl+Shift+D)
3. Select one of the debug configurations:
   - **Debug All Tests** - Debug the complete test suite
   - **Debug RSI Test** - Debug RSI calculations
   - **Debug Black Scholes Test** - Debug option pricing
   - **Debug Compound Interest Test** - Debug interest calculations
   - And more...

4. Set breakpoints in your code
5. Press F5 to start debugging

### Building

Use the Command Palette (Ctrl+Shift+P) and run:
- `Tasks: Run Task` → `build-debug` (for debug builds)
- `Tasks: Run Task` → `build-release` (for optimized builds)
- `Tasks: Run Task` → `clean` (to clean build directory)

### Testing

Use the Command Palette and run:
- `Tasks: Run Task` → `test-all` (run all tests with CTest)
- `Tasks: Run Task` → `test-unit` (run tests individually)

## Available Test Executables

After building, the following test executables are available in the `build/` directory:

- `run_all_tests` - Comprehensive test runner
- `RSITest_executable` - RSI calculation tests
- `BlackScholesTest_executable` - Black-Scholes option pricing tests
- `CompoundInterestTest_executable` - Compound interest tests
- `RollingVolatilityTest_executable` - Rolling volatility tests
- `SimpleMovingAverageTest_executable` - Simple moving average tests
- `EMATest_executable` - Exponential moving average tests

## Build Types

### Debug Build
- Includes debug symbols (`-g`)
- No optimization (`-O0`)
- All warnings enabled (`-Wall -Wextra -Wpedantic`)
- Default build type

### Release Build
- Optimized (`-O3`)
- No debug symbols by default
- `NDEBUG` macro defined

### RelWithDebInfo Build
- Optimized (`-O2`)
- Includes debug symbols (`-g`)
- `NDEBUG` macro defined

## GDB Setup on macOS

GDB requires special permissions on macOS. If you encounter issues:

1. Create a certificate for code signing:
   ```bash
   # Create a self-signed certificate in Keychain Access
   # Name: gdb-cert
   # Type: Code Signing
   ```

2. Sign the GDB binary:
   ```bash
   codesign -s gdb-cert /opt/homebrew/bin/gdb
   ```

3. Allow GDB to control other processes:
   ```bash
   sudo spctl --master-disable
   ```

## Troubleshooting

### Build Issues
- Ensure CMake is installed: `brew install cmake`
- Clean and rebuild: `./scripts/build.sh clean && ./scripts/build.sh debug`

### Debug Issues
- Use LLDB instead of GDB if you encounter permission issues
- Ensure debug symbols are included in the build

### Test Issues
- Check that all dependencies are installed
- Verify Python development headers are available for pybind11

## Development Workflow

1. **Make changes** to source files
2. **Build** with `./scripts/build.sh debug`
3. **Test** with `./scripts/test.sh all`
4. **Debug** if needed with `./scripts/debug.sh [test_name]`
5. **Iterate** until tests pass

## Additional Resources

- [CMake Documentation](https://cmake.org/documentation/)
- [GDB Manual](https://www.gnu.org/software/gdb/documentation/)
- [LLDB Documentation](https://lldb.llvm.org/)
- [VS Code C++ Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)

