cmake_minimum_required(VERSION 3.10)

project(finmath)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add debugging symbols (set to Release for optimized performance if needed)
set(CMAKE_BUILD_TYPE Release)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
file(GLOB SOURCES "src/cpp/*/*.cpp")

# Create the main C++ library target with a unique name
add_library(finmath_library SHARED ${SOURCES}
    "src/cpp/InterestAndAnnuities/simple_interest.cpp"
    "include/finmath/InterestAndAnnuities/simple_interest.h"
    "include/finmath/OptionPricing/options_pricing.h"
    "include/finmath/OptionPricing/options_pricing_types.h"
    "include/finmath/TimeSeries/rolling_volatility.h"
    "include/finmath/TimeSeries/simple_moving_average.h"
    "include/finmath/TimeSeries/rsi.h")

# Test executable
add_executable(runTests test/test_finmath.cpp)
target_link_libraries(runTests finmath_library)

# Enable testing
enable_testing()

# Define tests
add_test(NAME UnitTests COMMAND runTests)

# Ensure 'make' builds the tests and then runs them
add_custom_target(build_and_test
    COMMAND ${CMAKE_COMMAND} --build . --target runTests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Make 'build_and_test' the default target
add_custom_target(default ALL DEPENDS build_and_test)

# Add pybind11 for Python bindings
include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG v2.10.0  # Use a stable version of pybind11
)

# Add xsimd
FetchContent_Declare(
    xsimd
    GIT_REPOSITORY https://github.com/xtensor-stack/xsimd.git
    GIT_TAG 11.0.0
)

FetchContent_MakeAvailable(pybind11 xsimd)

# Add xsimd include directories AFTER it's available
target_include_directories(finmath_library PRIVATE ${xsimd_SOURCE_DIR}/include)
target_include_directories(runTests PRIVATE ${xsimd_SOURCE_DIR}/include)

# Create the Python bindings target
pybind11_add_module(finmath_bindings src/python_bindings.cpp ${SOURCES})

# Add xsimd include directory to the bindings target AFTER it's created
target_include_directories(finmath_bindings PRIVATE ${xsimd_SOURCE_DIR}/include)

# Set the output name of the bindings to 'finmath' to match your desired module name
set_target_properties(finmath_bindings PROPERTIES OUTPUT_NAME "finmath")

# Link the Python bindings target with the C++ library
target_link_libraries(finmath_bindings PRIVATE finmath_library)
