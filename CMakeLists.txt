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

# Markov chain sources only (this branch)
set(SOURCES
    "src/cpp/MarkovChains/markov_chain.cpp"
    "src/cpp/finmath.cpp"
)

# Create the main C++ library target
add_library(finmath_library SHARED ${SOURCES})

# Enable testing
enable_testing()

# Helper macro to add a test executable and link it
macro(add_cpp_test test_name source_file)
    message(STATUS "Adding C++ test: ${test_name} from ${source_file}")
    add_executable(${test_name}_executable ${source_file})
    target_link_libraries(${test_name}_executable PRIVATE finmath_library)
    add_test(NAME ${test_name} COMMAND ${test_name}_executable)
endmacro()

# Markov chain test only
add_cpp_test(MarkovChainTest test/MarkovChains/C++/markov_chain_test.cpp)
