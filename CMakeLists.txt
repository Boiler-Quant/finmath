cmake_minimum_required(VERSION 3.15)

project(finmath)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add debugging symbols (set to Release for optimized performance if needed)
set(CMAKE_BUILD_TYPE Release)

# Enable SIMD optimizations based on architecture
if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    # x86/x86_64: Enable SSE2 (baseline) and detect AVX
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
    
    # Try to enable AVX if available
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-mavx" COMPILER_SUPPORTS_AVX)
    if(COMPILER_SUPPORTS_AVX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx")
        message(STATUS "AVX support enabled")
    else()
        message(STATUS "AVX not supported, using SSE2")
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|ARM64)")
    # ARM64: NEON is standard on ARMv8
    message(STATUS "ARM NEON support enabled (ARMv8)")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
    # ARM32: Try to enable NEON
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=neon")
    message(STATUS "ARM NEON support enabled (ARMv7)")
else()
    message(STATUS "Using scalar fallback (no SIMD)")
endif()

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
    "include/finmath/TimeSeries/rsi.h"
    "include/finmath/TimeSeries/ema.h")

# Link pybind11 headers to the main library (needed for numpy integration in C++ files)
target_link_libraries(finmath_library PUBLIC pybind11::headers)

# Also link Python libraries/headers (needed for Python.h)
find_package(Python COMPONENTS Interpreter Development REQUIRED)
target_link_libraries(finmath_library PUBLIC Python::Python)

# Enable testing
enable_testing()

# Test include dir for test_common.h (optional use by tests)
set(FINMATH_TEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test/include)

# Helper macro: add a C++ test executable, link to library, register with CTest, set labels
macro(add_cpp_test test_name source_file)
    message(STATUS "Adding C++ test: ${test_name} from ${source_file}")
    add_executable(${test_name}_executable ${source_file})
    target_include_directories(${test_name}_executable PRIVATE ${FINMATH_TEST_INCLUDE_DIR})
    target_link_libraries(${test_name}_executable PRIVATE finmath_library)
    add_test(NAME ${test_name} COMMAND ${test_name}_executable)
endmacro()

# Helper: add test and assign CTest labels (e.g. for ctest -L SIMD)
macro(add_cpp_test_labeled test_name source_file labels)
    add_cpp_test(${test_name} ${source_file})
    set_tests_properties(${test_name} PROPERTIES LABELS "${labels}")
endmacro()

# --- SIMD and helper tests (label: SIMD) ---
add_cpp_test_labeled(SIMDHelperTest test/Helper/C++/simd_helper_test.cpp "SIMD;Helper;Unit")
add_cpp_test_labeled(SimpleMovingAverageSIMDTest test/TimeSeries/SimpleMovingAverage/C++/simple_moving_average_simd_test.cpp "SIMD;TimeSeries;Unit")
add_cpp_test_labeled(RSISIMDTest test/TimeSeries/RSI/C++/rsi_simd_test.cpp "SIMD;TimeSeries;Unit")
add_cpp_test_labeled(EMASIMDTest test/TimeSeries/EMA/C++/ema_simd_test.cpp "SIMD;TimeSeries;Unit")

# --- Core unit tests ---
add_cpp_test_labeled(CompoundInterestTest test/InterestAndAnnuities/compound_interest_test.cpp "InterestAndAnnuities;Unit")
add_cpp_test_labeled(BlackScholesTest test/OptionPricing/black_scholes_test.cpp "OptionPricing;Unit")
add_cpp_test_labeled(BinomialOptionPricingTest test/OptionPricing/binomial_option_pricing_test.cpp "OptionPricing;Unit")
add_cpp_test_labeled(RSITest test/TimeSeries/rsi_test.cpp "TimeSeries;Unit")
add_cpp_test_labeled(RollingStdDevTest test/TimeSeries/rolling_std_dev_test.cpp "TimeSeries;Unit")

# Add pybind11 for Python bindings
include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG v2.11.1  # Use a stable version of pybind11
)
FetchContent_MakeAvailable(pybind11)

# Create the Python bindings target
pybind11_add_module(finmath_bindings src/python_bindings.cpp ${SOURCES})

# Set the output name of the bindings to 'finmath' to match your desired module name
set_target_properties(finmath_bindings PROPERTIES OUTPUT_NAME "finmath")

# Set the library output directory to be alongside the source bindings file
set_target_properties(finmath_bindings PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/src")

# Link the Python bindings target with the C++ library
target_link_libraries(finmath_bindings PRIVATE finmath_library)
