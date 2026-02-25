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
    "include/finmath/TimeSeries/rsi.h"
    "include/finmath/TimeSeries/ema.h"
    "include/finmath/TimeSeries/rolling_std_dev.h")

# Link pybind11 headers to the main library (needed for numpy integration in C++ files)
target_link_libraries(finmath_library PUBLIC pybind11::headers)

# Also link Python libraries/headers (needed for Python.h)
find_package(Python COMPONENTS Interpreter Development REQUIRED)
target_link_libraries(finmath_library PUBLIC Python::Python)

# Enable testing
enable_testing()

# Helper macro to add a test executable and link it
macro(add_cpp_test test_name source_file)
    message(STATUS "Adding C++ test: ${test_name} from ${source_file}")
    add_executable(${test_name}_executable ${source_file})
    target_link_libraries(${test_name}_executable PRIVATE finmath_library)
    add_test(NAME ${test_name} COMMAND ${test_name}_executable)
endmacro()

# Core unit tests (from main + branch)
add_cpp_test(BlackScholesTest test/OptionPricing/black_scholes_test.cpp)
add_cpp_test(BlackScholesGreeksTest test/OptionPricing/black_scholes_greeks_test.cpp)
add_cpp_test(BinomialOptionPricingTest test/OptionPricing/binomial_option_pricing_test.cpp)
add_cpp_test(CompoundInterestTest test/InterestAndAnnuities/compound_interest_test.cpp)
add_cpp_test(SimpleInterestTest test/InterestAndAnnuities/simple_interest_test.cpp)
add_cpp_test(HelperTest test/Helper/helper_test.cpp)
add_cpp_test(RSITest test/TimeSeries/RSI/C++/rsi_test.cpp)
add_cpp_test(RollingStdDevTest test/TimeSeries/rolling_std_dev_test.cpp)
add_cpp_test(BellmanArbitrageTest test/GraphAlgos/bellman_arbitrage_test.cpp)
add_cpp_test(RollingVolatilityTest test/TimeSeries/RollingVolatility/C++/rolling_volatility_test.cpp)
add_cpp_test(SimpleMovingAverageTest test/TimeSeries/SimpleMovingAverage/C++/simple_moving_average_test.cpp)
add_cpp_test(EMATest test/TimeSeries/EMA/C++/ema_test.cpp)

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
