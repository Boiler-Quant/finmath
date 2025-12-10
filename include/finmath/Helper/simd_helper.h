#ifndef SIMD_HELPER_H
#define SIMD_HELPER_H

#include <cstddef>
#include <vector>

// Platform detection and SIMD intrinsics
#if defined(__ARM_NEON) || defined(__aarch64__)
    #define FINMATH_USE_NEON
    #include <arm_neon.h>
#elif defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    #define FINMATH_USE_SSE
    #include <emmintrin.h>  // SSE2
    #include <xmmintrin.h>  // SSE
    #ifdef __AVX__
        #define FINMATH_USE_AVX
        #include <immintrin.h>  // AVX
    #endif
#else
    #define FINMATH_USE_SCALAR
#endif

namespace finmath {
namespace simd {

/**
 * @brief Cross-platform SIMD vector addition
 * Computes: result[i] = a[i] + b[i]
 * 
 * @param a First input vector
 * @param b Second input vector
 * @param result Output vector (must be pre-allocated)
 * @param size Number of elements
 */
void vector_add(const double* a, const double* b, double* result, size_t size);

/**
 * @brief Cross-platform SIMD vector subtraction
 * Computes: result[i] = a[i] - b[i]
 * 
 * @param a First input vector
 * @param b Second input vector
 * @param result Output vector (must be pre-allocated)
 * @param size Number of elements
 */
void vector_sub(const double* a, const double* b, double* result, size_t size);

/**
 * @brief Cross-platform SIMD vector multiplication
 * Computes: result[i] = a[i] * b[i]
 * 
 * @param a First input vector
 * @param b Second input vector
 * @param result Output vector (must be pre-allocated)
 * @param size Number of elements
 */
void vector_mul(const double* a, const double* b, double* result, size_t size);

/**
 * @brief Cross-platform SIMD dot product
 * Computes: sum(a[i] * b[i])
 * 
 * @param a First input vector
 * @param b Second input vector
 * @param size Number of elements
 * @return Dot product result
 */
double dot_product(const double* a, const double* b, size_t size);

/**
 * @brief Cross-platform SIMD sum
 * Computes: sum(a[i])
 * 
 * @param a Input vector
 * @param size Number of elements
 * @return Sum of all elements
 */
double vector_sum(const double* a, size_t size);

/**
 * @brief Cross-platform SIMD mean calculation
 * Computes: sum(a[i]) / size
 * 
 * @param a Input vector
 * @param size Number of elements
 * @return Mean of all elements
 */
double vector_mean(const double* a, size_t size);

/**
 * @brief Cross-platform SIMD variance calculation
 * Computes: sum((a[i] - mean)^2) / size
 * 
 * @param a Input vector
 * @param size Number of elements
 * @return Variance of all elements
 */
double vector_variance(const double* a, size_t size);

/**
 * @brief Cross-platform SIMD standard deviation calculation
 * Computes: sqrt(variance)
 * 
 * @param a Input vector
 * @param size Number of elements
 * @return Standard deviation of all elements
 */
double vector_stddev(const double* a, size_t size);

/**
 * @brief Cross-platform SIMD scalar multiplication
 * Computes: result[i] = a[i] * scalar
 * 
 * @param a Input vector
 * @param scalar Scalar multiplier
 * @param result Output vector (must be pre-allocated)
 * @param size Number of elements
 */
void vector_mul_scalar(const double* a, double scalar, double* result, size_t size);

/**
 * @brief Cross-platform SIMD scalar addition
 * Computes: result[i] = a[i] + scalar
 * 
 * @param a Input vector
 * @param scalar Scalar to add
 * @param result Output vector (must be pre-allocated)
 * @param size Number of elements
 */
void vector_add_scalar(const double* a, double scalar, double* result, size_t size);

/**
 * @brief Cross-platform SIMD element-wise division
 * Computes: result[i] = a[i] / b[i]
 * 
 * @param a First input vector (numerator)
 * @param b Second input vector (denominator)
 * @param result Output vector (must be pre-allocated)
 * @param size Number of elements
 */
void vector_div(const double* a, const double* b, double* result, size_t size);

/**
 * @brief Cross-platform SIMD maximum element
 * Computes: max(a[i])
 * 
 * @param a Input vector
 * @param size Number of elements
 * @return Maximum element
 */
double vector_max(const double* a, size_t size);

/**
 * @brief Cross-platform SIMD minimum element
 * Computes: min(a[i])
 * 
 * @param a Input vector
 * @param size Number of elements
 * @return Minimum element
 */
double vector_min(const double* a, size_t size);

/**
 * @brief Cross-platform SIMD conditional sum (sum where condition is true)
 * Computes: sum(a[i] where a[i] > 0) or sum(a[i] where a[i] < 0)
 * 
 * @param a Input vector
 * @param size Number of elements
 * @param positive If true, sum positive elements; if false, sum absolute of negative elements
 * @return Conditional sum
 */
double vector_conditional_sum(const double* a, size_t size, bool positive);

/**
 * @brief Get the SIMD implementation being used
 * @return String description of SIMD backend ("NEON", "AVX", "SSE", "Scalar")
 */
const char* get_simd_backend();

} // namespace simd
} // namespace finmath

#endif // SIMD_HELPER_H

