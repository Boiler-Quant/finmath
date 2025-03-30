cmake_minimum_required(VERSION 3.15...4.0)

project(finmath LANGUAGES CXX)

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
    "include/finmath/TimeSeries/ema.h")

# Test executables
add_executable(black_scholes_test test/OptionPricing/black_scholes_test.cpp)
target_link_libraries(black_scholes_test finmath_library)

add_executable(binomial_option_pricing_test test/OptionPricing/binomial_option_pricing_test.cpp)
target_link_libraries(binomial_option_pricing_test finmath_library)

add_executable(compound_interest_test test/InterestAndAnnuities/compound_interest_test.cpp)
target_link_libraries(compound_interest_test finmath_library)

add_executable(rsi_test test/TimeSeries/rsi_test.cpp)
target_link_libraries(rsi_test finmath_library)

# Test runner
add_executable(run_all_tests test/test_runner.cpp)

# Enable testing
enable_testing()

# Define individual tests
add_test(NAME BlackScholesTest COMMAND black_scholes_test)
add_test(NAME BinomialOptionPricingTest COMMAND binomial_option_pricing_test)
add_test(NAME CompoundInterestTest COMMAND compound_interest_test)
add_test(NAME RSITest COMMAND rsi_test)

# Add a custom target to run all tests
add_custom_target(build_and_test
    COMMAND ${CMAKE_COMMAND} --build . --target black_scholes_test
    COMMAND ${CMAKE_COMMAND} --build . --target binomial_option_pricing_test
    COMMAND ${CMAKE_COMMAND} --build . --target compound_interest_test
    COMMAND ${CMAKE_COMMAND} --build . --target rsi_test
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
FetchContent_MakeAvailable(pybind11)

# Create the Python bindings target
pybind11_add_module(finmath_bindings src/python_bindings.cpp ${SOURCES})

# Set the output name of the bindings to 'finmath' to match your desired module name
set_target_properties(finmath_bindings PROPERTIES OUTPUT_NAME "finmath")

# Link the Python bindings target with the C++ library
target_link_libraries(finmath_bindings PRIVATE finmath_library)

# Set install target for pip to find shared lib
install(TARGETS finmath_bindings finmath_library
        LIBRARY DESTINATION finmath
        ARCHIVE DESTINATION finmath
        RUNTIME DESTINATION finmath
)

if(APPLE)
    # For the Python bindings module
    set_target_properties(finmath_bindings PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "@loader_path"
    )
    # For the library itself (if needed)
    set_target_properties(finmath_library PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "@loader_path"
    )
endif()

message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")