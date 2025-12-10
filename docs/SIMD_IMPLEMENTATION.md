# Cross-Platform SIMD Implementation

## Overview

The `finmath` library includes a cross-platform SIMD (Single Instruction, Multiple Data) implementation that automatically detects and uses the best available CPU instructions for your platform. This provides significant performance improvements for computationally intensive financial calculations.

## Supported Architectures

### ARM (NEON)
- **Apple Silicon** (M1, M2, M3, M4)
- **ARM64 servers** (AWS Graviton, Ampere Altra)
- **Vector width**: 128-bit (2 doubles)
- **Expected speedup**: 1.5-2.5x

### x86_64 (SSE/AVX)
- **Intel processors** (Core i3/i5/i7/i9, Xeon)
- **AMD processors** (Ryzen, EPYC, Threadripper)
- **Vector widths**:
  - SSE2: 128-bit (2 doubles) - baseline
  - AVX: 256-bit (4 doubles) - if available
- **Expected speedup**: 
  - SSE2: 1.5-2x
  - AVX: 2-4x

### Scalar Fallback
- Any other architecture
- Automatically used when SIMD is not available
- Ensures code runs everywhere

## How It Works

### Compile-Time Detection

The build system automatically detects your CPU architecture and enables the appropriate SIMD instructions:

```cmake
# CMakeLists.txt
if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
    if(COMPILER_SUPPORTS_AVX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx")
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|ARM64)")
    # NEON is standard on ARMv8
    message(STATUS "ARM NEON support enabled")
endif()
```

### Runtime Abstraction

The SIMD helper provides a unified API that works across all architectures:

```cpp
#include "finmath/Helper/simd_helper.h"

// Works on ARM (NEON), x86_64 (SSE/AVX), or scalar fallback
double mean = finmath::simd::vector_mean(data, size);
double variance = finmath::simd::vector_variance(data, size);
double dot = finmath::simd::dot_product(a, b, size);
```

### Example: Vector Addition

The same code compiles to different instructions depending on the target:

**ARM NEON:**
```cpp
float64x2_t va = vld1q_f64(&a[i]);      // Load 2 doubles
float64x2_t vb = vld1q_f64(&b[i]);      // Load 2 doubles
float64x2_t vr = vaddq_f64(va, vb);     // Add 2 doubles in parallel
vst1q_f64(&result[i], vr);              // Store 2 doubles
```

**x86_64 AVX:**
```cpp
__m256d va = _mm256_loadu_pd(&a[i]);    // Load 4 doubles
__m256d vb = _mm256_loadu_pd(&b[i]);    // Load 4 doubles
__m256d vr = _mm256_add_pd(va, vb);     // Add 4 doubles in parallel
_mm256_storeu_pd(&result[i], vr);       // Store 4 doubles
```

**x86_64 SSE2:**
```cpp
__m128d va = _mm_loadu_pd(&a[i]);       // Load 2 doubles
__m128d vb = _mm_loadu_pd(&b[i]);       // Load 2 doubles
__m128d vr = _mm_add_pd(va, vb);        // Add 2 doubles in parallel
_mm_storeu_pd(&result[i], vr);          // Store 2 doubles
```

**Scalar Fallback:**
```cpp
result[i] = a[i] + b[i];                // Add 1 double at a time
```

## Available SIMD Functions

### Basic Operations
```cpp
// Element-wise operations
void vector_add(const double* a, const double* b, double* result, size_t size);
void vector_sub(const double* a, const double* b, double* result, size_t size);
void vector_mul(const double* a, const double* b, double* result, size_t size);
```

### Reductions
```cpp
// Aggregate operations
double vector_sum(const double* a, size_t size);
double vector_mean(const double* a, size_t size);
double dot_product(const double* a, const double* b, size_t size);
```

### Statistical Functions
```cpp
// Statistics
double vector_variance(const double* a, size_t size);
double vector_stddev(const double* a, size_t size);
```

### Backend Detection
```cpp
// Runtime information
const char* get_simd_backend();  // Returns "AVX", "SSE", "NEON", or "Scalar"
```

## SIMD-Optimized Functions

### Rolling Volatility (SIMD)

The SIMD-optimized rolling volatility function combines:
- **Zero-copy NumPy integration** (no data duplication)
- **SIMD-accelerated calculations** (parallel processing)
- **Cross-platform compatibility** (works everywhere)

```python
import finmath
import numpy as np

prices = np.random.randn(100000) + 100
volatility = finmath.rolling_volatility_simd(prices, window=252)

# Check which SIMD backend is being used
print(f"Using: {finmath.get_simd_backend()}")
```

**Performance comparison:**
```
Dataset: 100,000 prices, window=100

Baseline (list):              15.23 ms
Zero-copy (NumPy):             8.45 ms (1.8x faster)
SIMD-optimized (NumPy):        3.21 ms (4.7x faster)
```

## Building with SIMD Support

### Standard Build (Auto-detect)
```bash
mkdir build && cd build
cmake ..
make
```

CMake will automatically detect your CPU and enable the best SIMD instructions.

### Force Specific SIMD Backend

**AVX (if supported):**
```bash
cmake .. -DCMAKE_CXX_FLAGS="-mavx"
make
```

**SSE2 only:**
```bash
cmake .. -DCMAKE_CXX_FLAGS="-msse2"
make
```

**Disable SIMD (scalar only):**
```bash
cmake .. -DCMAKE_CXX_FLAGS="-mno-sse -mno-avx"
make
```

## Verifying SIMD Usage

### Python
```python
import finmath
print(f"SIMD Backend: {finmath.get_simd_backend()}")
```

### C++ Test
```bash
cd build
./SIMDHelperTest_executable
```

Output:
```
Starting SIMD Helper Tests...
SIMD Backend: NEON

✓ Test 1 (Vector Addition) Passed
✓ Test 2 (Vector Subtraction) Passed
✓ Test 3 (Vector Multiplication) Passed
...
All SIMD Helper Tests Passed! ✅
```

## Performance Tips

### 1. Use Contiguous Memory
SIMD works best with contiguous arrays:
```python
# Good: NumPy arrays are contiguous by default
prices = np.array([100, 101, 102, ...])
volatility = finmath.rolling_volatility_simd(prices, 50)

# Bad: Python lists require conversion (slower)
prices = [100, 101, 102, ...]
volatility = finmath.rolling_volatility(prices, 50)
```

### 2. Align Data to Cache Lines
For best performance, ensure data is aligned to 64-byte cache lines (handled automatically by NumPy).

### 3. Use Appropriate Data Sizes
SIMD is most effective with:
- **Minimum**: 100+ elements
- **Optimal**: 1,000-1,000,000+ elements
- **Small datasets** (<100 elements): Scalar overhead may dominate

### 4. Batch Processing
Process data in batches rather than one element at a time:
```python
# Good: Batch processing
for batch in market_data_batches:
    volatility = finmath.rolling_volatility_simd(batch, window)

# Bad: Element-by-element
for price in prices:
    # Process one at a time
```

## Use Cases

### High-Frequency Trading (HFT)
- **Real-time risk metrics** with microsecond latency
- **Streaming volatility** calculations
- **Order book analytics**

### Quantitative Research
- **Large-scale backtesting** (millions of data points)
- **Monte Carlo simulations** (parallel sampling)
- **Statistical arbitrage** (fast correlation calculations)

### Risk Management
- **Real-time VaR** (Value at Risk)
- **Portfolio optimization** (covariance matrix operations)
- **Stress testing** (rapid scenario analysis)

## Benchmarking

Run the comprehensive performance demo:

```bash
cd build
python3 ../demos/simd_performance_demo.py
```

This will:
1. Display your system's SIMD capabilities
2. Benchmark SIMD vs non-SIMD implementations
3. Show memory efficiency of zero-copy approach
4. Simulate real-time market analysis throughput

## Implementation Details

### Memory Access Patterns

**Aligned vs Unaligned Loads:**
- We use **unaligned loads** (`_mm256_loadu_pd`, `vld1q_f64`) for flexibility
- NumPy doesn't guarantee alignment, so unaligned is safer
- Performance penalty is minimal on modern CPUs (~1-2%)

### Horizontal Reductions

For operations like `sum` or `dot_product`, we accumulate into SIMD registers and then perform a horizontal reduction:

```cpp
// AVX: Sum 4 doubles
__m256d vsum = _mm256_setzero_pd();
for (size_t i = 0; i + 4 <= size; i += 4) {
    __m256d va = _mm256_loadu_pd(&a[i]);
    vsum = _mm256_add_pd(vsum, va);
}
// Horizontal sum
double temp[4];
_mm256_storeu_pd(temp, vsum);
double result = temp[0] + temp[1] + temp[2] + temp[3];
```

### Tail Handling

We process data in SIMD-sized chunks and handle remaining elements with scalar code:

```cpp
size_t i = 0;
// SIMD: Process 4 at a time (AVX)
for (; i + 4 <= size; i += 4) {
    // SIMD operations
}
// Scalar: Handle remaining 0-3 elements
for (; i < size; ++i) {
    // Scalar operations
}
```

## Future Enhancements

### Planned Features
- [ ] **AVX-512** support (Intel Skylake-X and later)
- [ ] **FMA** (Fused Multiply-Add) for better accuracy
- [ ] **SIMD-optimized SMA, EMA, RSI** functions
- [ ] **Auto-vectorization hints** for compiler optimization
- [ ] **Cache-oblivious algorithms** for large datasets

### Wish List
- [ ] **GPU acceleration** via CUDA/ROCm for massive parallelism
- [ ] **Multi-threading** with SIMD (thread-level + data-level parallelism)
- [ ] **Custom memory allocators** with guaranteed alignment

## References

- [Intel Intrinsics Guide](https://software.intel.com/sites/landingpage/IntrinsicsGuide/)
- [ARM NEON Intrinsics](https://developer.arm.com/architectures/instruction-sets/intrinsics/)
- [Agner Fog's Optimization Manuals](https://www.agner.org/optimize/)

## License

Same as the main `finmath` library.

