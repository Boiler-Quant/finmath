#include "finmath/Helper/simd_helper.h"
#include <cmath>
#include <algorithm>

namespace finmath {
namespace simd {

const char* get_simd_backend() {
#ifdef FINMATH_USE_AVX
    return "AVX";
#elif defined(FINMATH_USE_SSE)
    return "SSE";
#elif defined(FINMATH_USE_NEON)
    return "NEON";
#else
    return "Scalar";
#endif
}

// ============================================================================
// Vector Addition: result[i] = a[i] + b[i]
// ============================================================================

void vector_add(const double* a, const double* b, double* result, size_t size) {
    if (!a || !b || !result || size == 0) return;
    size_t i = 0;

#ifdef FINMATH_USE_AVX
    // AVX: Process 4 doubles at a time (256-bit vectors)
    for (; i + 4 <= size; i += 4) {
        __m256d va = _mm256_loadu_pd(&a[i]);
        __m256d vb = _mm256_loadu_pd(&b[i]);
        __m256d vr = _mm256_add_pd(va, vb);
        _mm256_storeu_pd(&result[i], vr);
    }
#elif defined(FINMATH_USE_SSE)
    // SSE: Process 2 doubles at a time (128-bit vectors)
    for (; i + 2 <= size; i += 2) {
        __m128d va = _mm_loadu_pd(&a[i]);
        __m128d vb = _mm_loadu_pd(&b[i]);
        __m128d vr = _mm_add_pd(va, vb);
        _mm_storeu_pd(&result[i], vr);
    }
#elif defined(FINMATH_USE_NEON)
    // ARM NEON: Process 2 doubles at a time (128-bit vectors)
    for (; i + 2 <= size; i += 2) {
        float64x2_t va = vld1q_f64(&a[i]);
        float64x2_t vb = vld1q_f64(&b[i]);
        float64x2_t vr = vaddq_f64(va, vb);
        vst1q_f64(&result[i], vr);
    }
#endif

    // Scalar fallback for remaining elements
    for (; i < size; ++i) {
        result[i] = a[i] + b[i];
    }
}

// ============================================================================
// Vector Subtraction: result[i] = a[i] - b[i]
// ============================================================================

void vector_sub(const double* a, const double* b, double* result, size_t size) {
    if (!a || !b || !result || size == 0) return;
    size_t i = 0;

#ifdef FINMATH_USE_AVX
    for (; i + 4 <= size; i += 4) {
        __m256d va = _mm256_loadu_pd(&a[i]);
        __m256d vb = _mm256_loadu_pd(&b[i]);
        __m256d vr = _mm256_sub_pd(va, vb);
        _mm256_storeu_pd(&result[i], vr);
    }
#elif defined(FINMATH_USE_SSE)
    for (; i + 2 <= size; i += 2) {
        __m128d va = _mm_loadu_pd(&a[i]);
        __m128d vb = _mm_loadu_pd(&b[i]);
        __m128d vr = _mm_sub_pd(va, vb);
        _mm_storeu_pd(&result[i], vr);
    }
#elif defined(FINMATH_USE_NEON)
    for (; i + 2 <= size; i += 2) {
        float64x2_t va = vld1q_f64(&a[i]);
        float64x2_t vb = vld1q_f64(&b[i]);
        float64x2_t vr = vsubq_f64(va, vb);
        vst1q_f64(&result[i], vr);
    }
#endif

    for (; i < size; ++i) {
        result[i] = a[i] - b[i];
    }
}

// ============================================================================
// Vector Multiplication: result[i] = a[i] * b[i]
// ============================================================================

void vector_mul(const double* a, const double* b, double* result, size_t size) {
    if (!a || !b || !result || size == 0) return;
    size_t i = 0;

#ifdef FINMATH_USE_AVX
    for (; i + 4 <= size; i += 4) {
        __m256d va = _mm256_loadu_pd(&a[i]);
        __m256d vb = _mm256_loadu_pd(&b[i]);
        __m256d vr = _mm256_mul_pd(va, vb);
        _mm256_storeu_pd(&result[i], vr);
    }
#elif defined(FINMATH_USE_SSE)
    for (; i + 2 <= size; i += 2) {
        __m128d va = _mm_loadu_pd(&a[i]);
        __m128d vb = _mm_loadu_pd(&b[i]);
        __m128d vr = _mm_mul_pd(va, vb);
        _mm_storeu_pd(&result[i], vr);
    }
#elif defined(FINMATH_USE_NEON)
    for (; i + 2 <= size; i += 2) {
        float64x2_t va = vld1q_f64(&a[i]);
        float64x2_t vb = vld1q_f64(&b[i]);
        float64x2_t vr = vmulq_f64(va, vb);
        vst1q_f64(&result[i], vr);
    }
#endif

    for (; i < size; ++i) {
        result[i] = a[i] * b[i];
    }
}

// ============================================================================
// Dot Product: sum(a[i] * b[i])
// ============================================================================

double dot_product(const double* a, const double* b, size_t size) {
    if (!a || !b || size == 0) return 0.0;
    double sum = 0.0;
    size_t i = 0;

#ifdef FINMATH_USE_AVX
    __m256d vsum = _mm256_setzero_pd();
    for (; i + 4 <= size; i += 4) {
        __m256d va = _mm256_loadu_pd(&a[i]);
        __m256d vb = _mm256_loadu_pd(&b[i]);
        __m256d vproduct = _mm256_mul_pd(va, vb);
        vsum = _mm256_add_pd(vsum, vproduct);
    }
    // Horizontal sum of 4 doubles
    double temp[4];
    _mm256_storeu_pd(temp, vsum);
    sum = temp[0] + temp[1] + temp[2] + temp[3];
#elif defined(FINMATH_USE_SSE)
    __m128d vsum = _mm_setzero_pd();
    for (; i + 2 <= size; i += 2) {
        __m128d va = _mm_loadu_pd(&a[i]);
        __m128d vb = _mm_loadu_pd(&b[i]);
        __m128d vproduct = _mm_mul_pd(va, vb);
        vsum = _mm_add_pd(vsum, vproduct);
    }
    // Horizontal sum of 2 doubles
    double temp[2];
    _mm_storeu_pd(temp, vsum);
    sum = temp[0] + temp[1];
#elif defined(FINMATH_USE_NEON)
    float64x2_t vsum = vdupq_n_f64(0.0);
    for (; i + 2 <= size; i += 2) {
        float64x2_t va = vld1q_f64(&a[i]);
        float64x2_t vb = vld1q_f64(&b[i]);
        float64x2_t vproduct = vmulq_f64(va, vb);
        vsum = vaddq_f64(vsum, vproduct);
    }
    // Horizontal sum of 2 doubles
    sum = vgetq_lane_f64(vsum, 0) + vgetq_lane_f64(vsum, 1);
#endif

    // Scalar fallback for remaining elements
    for (; i < size; ++i) {
        sum += a[i] * b[i];
    }

    return sum;
}

// ============================================================================
// Vector Sum: sum(a[i])
// ============================================================================

double vector_sum(const double* a, size_t size) {
    if (!a || size == 0) return 0.0;
    double sum = 0.0;
    size_t i = 0;

#ifdef FINMATH_USE_AVX
    __m256d vsum = _mm256_setzero_pd();
    for (; i + 4 <= size; i += 4) {
        __m256d va = _mm256_loadu_pd(&a[i]);
        vsum = _mm256_add_pd(vsum, va);
    }
    double temp[4];
    _mm256_storeu_pd(temp, vsum);
    sum = temp[0] + temp[1] + temp[2] + temp[3];
#elif defined(FINMATH_USE_SSE)
    __m128d vsum = _mm_setzero_pd();
    for (; i + 2 <= size; i += 2) {
        __m128d va = _mm_loadu_pd(&a[i]);
        vsum = _mm_add_pd(vsum, va);
    }
    double temp[2];
    _mm_storeu_pd(temp, vsum);
    sum = temp[0] + temp[1];
#elif defined(FINMATH_USE_NEON)
    float64x2_t vsum = vdupq_n_f64(0.0);
    for (; i + 2 <= size; i += 2) {
        float64x2_t va = vld1q_f64(&a[i]);
        vsum = vaddq_f64(vsum, va);
    }
    sum = vgetq_lane_f64(vsum, 0) + vgetq_lane_f64(vsum, 1);
#endif

    for (; i < size; ++i) {
        sum += a[i];
    }

    return sum;
}

// ============================================================================
// Vector Mean: sum(a[i]) / size
// ============================================================================

double vector_mean(const double* a, size_t size) {
    if (size == 0) return 0.0;
    return vector_sum(a, size) / static_cast<double>(size);
}

// ============================================================================
// Vector Variance: sum((a[i] - mean)^2) / size
// ============================================================================

double vector_variance(const double* a, size_t size) {
    if (size == 0) return 0.0;
    
    double mean = vector_mean(a, size);
    double sum_sq = 0.0;
    size_t i = 0;

#ifdef FINMATH_USE_AVX
    __m256d vmean = _mm256_set1_pd(mean);
    __m256d vsum_sq = _mm256_setzero_pd();
    
    for (; i + 4 <= size; i += 4) {
        __m256d va = _mm256_loadu_pd(&a[i]);
        __m256d vdiff = _mm256_sub_pd(va, vmean);
        __m256d vsq = _mm256_mul_pd(vdiff, vdiff);
        vsum_sq = _mm256_add_pd(vsum_sq, vsq);
    }
    
    double temp[4];
    _mm256_storeu_pd(temp, vsum_sq);
    sum_sq = temp[0] + temp[1] + temp[2] + temp[3];
#elif defined(FINMATH_USE_SSE)
    __m128d vmean = _mm_set1_pd(mean);
    __m128d vsum_sq = _mm_setzero_pd();
    
    for (; i + 2 <= size; i += 2) {
        __m128d va = _mm_loadu_pd(&a[i]);
        __m128d vdiff = _mm_sub_pd(va, vmean);
        __m128d vsq = _mm_mul_pd(vdiff, vdiff);
        vsum_sq = _mm_add_pd(vsum_sq, vsq);
    }
    
    double temp[2];
    _mm_storeu_pd(temp, vsum_sq);
    sum_sq = temp[0] + temp[1];
#elif defined(FINMATH_USE_NEON)
    float64x2_t vmean = vdupq_n_f64(mean);
    float64x2_t vsum_sq = vdupq_n_f64(0.0);
    
    for (; i + 2 <= size; i += 2) {
        float64x2_t va = vld1q_f64(&a[i]);
        float64x2_t vdiff = vsubq_f64(va, vmean);
        float64x2_t vsq = vmulq_f64(vdiff, vdiff);
        vsum_sq = vaddq_f64(vsum_sq, vsq);
    }
    
    sum_sq = vgetq_lane_f64(vsum_sq, 0) + vgetq_lane_f64(vsum_sq, 1);
#endif

    for (; i < size; ++i) {
        double diff = a[i] - mean;
        sum_sq += diff * diff;
    }

    return sum_sq / static_cast<double>(size);
}

// ============================================================================
// Vector Standard Deviation: sqrt(variance)
// ============================================================================

double vector_stddev(const double* a, size_t size) {
    return std::sqrt(vector_variance(a, size));
}

// ============================================================================
// Vector Scalar Multiplication: result[i] = a[i] * scalar
// ============================================================================

void vector_mul_scalar(const double* a, double scalar, double* result, size_t size) {
    if (!a || !result || size == 0) return;
    size_t i = 0;

#ifdef FINMATH_USE_AVX
    __m256d vscalar = _mm256_set1_pd(scalar);
    for (; i + 4 <= size; i += 4) {
        __m256d va = _mm256_loadu_pd(&a[i]);
        __m256d vr = _mm256_mul_pd(va, vscalar);
        _mm256_storeu_pd(&result[i], vr);
    }
#elif defined(FINMATH_USE_SSE)
    __m128d vscalar = _mm_set1_pd(scalar);
    for (; i + 2 <= size; i += 2) {
        __m128d va = _mm_loadu_pd(&a[i]);
        __m128d vr = _mm_mul_pd(va, vscalar);
        _mm_storeu_pd(&result[i], vr);
    }
#elif defined(FINMATH_USE_NEON)
    float64x2_t vscalar = vdupq_n_f64(scalar);
    for (; i + 2 <= size; i += 2) {
        float64x2_t va = vld1q_f64(&a[i]);
        float64x2_t vr = vmulq_f64(va, vscalar);
        vst1q_f64(&result[i], vr);
    }
#endif

    for (; i < size; ++i) {
        result[i] = a[i] * scalar;
    }
}

// ============================================================================
// Vector Scalar Addition: result[i] = a[i] + scalar
// ============================================================================

void vector_add_scalar(const double* a, double scalar, double* result, size_t size) {
    if (!a || !result || size == 0) return;
    size_t i = 0;

#ifdef FINMATH_USE_AVX
    __m256d vscalar = _mm256_set1_pd(scalar);
    for (; i + 4 <= size; i += 4) {
        __m256d va = _mm256_loadu_pd(&a[i]);
        __m256d vr = _mm256_add_pd(va, vscalar);
        _mm256_storeu_pd(&result[i], vr);
    }
#elif defined(FINMATH_USE_SSE)
    __m128d vscalar = _mm_set1_pd(scalar);
    for (; i + 2 <= size; i += 2) {
        __m128d va = _mm_loadu_pd(&a[i]);
        __m128d vr = _mm_add_pd(va, vscalar);
        _mm_storeu_pd(&result[i], vr);
    }
#elif defined(FINMATH_USE_NEON)
    float64x2_t vscalar = vdupq_n_f64(scalar);
    for (; i + 2 <= size; i += 2) {
        float64x2_t va = vld1q_f64(&a[i]);
        float64x2_t vr = vaddq_f64(va, vscalar);
        vst1q_f64(&result[i], vr);
    }
#endif

    for (; i < size; ++i) {
        result[i] = a[i] + scalar;
    }
}

// ============================================================================
// Vector Division: result[i] = a[i] / b[i]
// ============================================================================

void vector_div(const double* a, const double* b, double* result, size_t size) {
    if (!a || !b || !result || size == 0) return;
    size_t i = 0;

#ifdef FINMATH_USE_AVX
    for (; i + 4 <= size; i += 4) {
        __m256d va = _mm256_loadu_pd(&a[i]);
        __m256d vb = _mm256_loadu_pd(&b[i]);
        __m256d vr = _mm256_div_pd(va, vb);
        _mm256_storeu_pd(&result[i], vr);
    }
#elif defined(FINMATH_USE_SSE)
    for (; i + 2 <= size; i += 2) {
        __m128d va = _mm_loadu_pd(&a[i]);
        __m128d vb = _mm_loadu_pd(&b[i]);
        __m128d vr = _mm_div_pd(va, vb);
        _mm_storeu_pd(&result[i], vr);
    }
#elif defined(FINMATH_USE_NEON)
    for (; i + 2 <= size; i += 2) {
        float64x2_t va = vld1q_f64(&a[i]);
        float64x2_t vb = vld1q_f64(&b[i]);
        float64x2_t vr = vdivq_f64(va, vb);
        vst1q_f64(&result[i], vr);
    }
#endif

    for (; i < size; ++i) {
        result[i] = a[i] / b[i];
    }
}

// ============================================================================
// Vector Maximum: max(a[i])
// ============================================================================

double vector_max(const double* a, size_t size) {
    if (size == 0) return 0.0;
    
    double max_val = a[0];
    size_t i = 1;

#ifdef FINMATH_USE_AVX
    __m256d vmax = _mm256_set1_pd(max_val);
    for (; i + 4 <= size; i += 4) {
        __m256d va = _mm256_loadu_pd(&a[i]);
        vmax = _mm256_max_pd(vmax, va);
    }
    double temp[4];
    _mm256_storeu_pd(temp, vmax);
    max_val = std::max({temp[0], temp[1], temp[2], temp[3]});
#elif defined(FINMATH_USE_SSE)
    __m128d vmax = _mm_set1_pd(max_val);
    for (; i + 2 <= size; i += 2) {
        __m128d va = _mm_loadu_pd(&a[i]);
        vmax = _mm_max_pd(vmax, va);
    }
    double temp[2];
    _mm_storeu_pd(temp, vmax);
    max_val = std::max(temp[0], temp[1]);
#elif defined(FINMATH_USE_NEON)
    float64x2_t vmax = vdupq_n_f64(max_val);
    for (; i + 2 <= size; i += 2) {
        float64x2_t va = vld1q_f64(&a[i]);
        vmax = vmaxq_f64(vmax, va);
    }
    max_val = std::max(vgetq_lane_f64(vmax, 0), vgetq_lane_f64(vmax, 1));
#endif

    for (; i < size; ++i) {
        if (a[i] > max_val) {
            max_val = a[i];
        }
    }

    return max_val;
}

// ============================================================================
// Vector Minimum: min(a[i])
// ============================================================================

double vector_min(const double* a, size_t size) {
    if (size == 0) return 0.0;
    
    double min_val = a[0];
    size_t i = 1;

#ifdef FINMATH_USE_AVX
    __m256d vmin = _mm256_set1_pd(min_val);
    for (; i + 4 <= size; i += 4) {
        __m256d va = _mm256_loadu_pd(&a[i]);
        vmin = _mm256_min_pd(vmin, va);
    }
    double temp[4];
    _mm256_storeu_pd(temp, vmin);
    min_val = std::min({temp[0], temp[1], temp[2], temp[3]});
#elif defined(FINMATH_USE_SSE)
    __m128d vmin = _mm_set1_pd(min_val);
    for (; i + 2 <= size; i += 2) {
        __m128d va = _mm_loadu_pd(&a[i]);
        vmin = _mm_min_pd(vmin, va);
    }
    double temp[2];
    _mm_storeu_pd(temp, vmin);
    min_val = std::min(temp[0], temp[1]);
#elif defined(FINMATH_USE_NEON)
    float64x2_t vmin = vdupq_n_f64(min_val);
    for (; i + 2 <= size; i += 2) {
        float64x2_t va = vld1q_f64(&a[i]);
        vmin = vminq_f64(vmin, va);
    }
    min_val = std::min(vgetq_lane_f64(vmin, 0), vgetq_lane_f64(vmin, 1));
#endif

    for (; i < size; ++i) {
        if (a[i] < min_val) {
            min_val = a[i];
        }
    }

    return min_val;
}

// ============================================================================
// Vector Conditional Sum: sum(a[i] where condition is true)
// ============================================================================

double vector_conditional_sum(const double* a, size_t size, bool positive) {
    if (!a || size == 0) return 0.0;
    double sum = 0.0;
    size_t i = 0;

#ifdef FINMATH_USE_AVX
    __m256d vsum = _mm256_setzero_pd();
    __m256d vzero = _mm256_setzero_pd();
    
    for (; i + 4 <= size; i += 4) {
        __m256d va = _mm256_loadu_pd(&a[i]);
        if (positive) {
            // Sum only positive values: max(0, a[i])
            __m256d vpos = _mm256_max_pd(vzero, va);
            vsum = _mm256_add_pd(vsum, vpos);
        } else {
            // Sum absolute of negative values: max(0, -a[i])
            __m256d vneg = _mm256_sub_pd(vzero, va);
            __m256d vabs_neg = _mm256_max_pd(vzero, vneg);
            vsum = _mm256_add_pd(vsum, vabs_neg);
        }
    }
    
    double temp[4];
    _mm256_storeu_pd(temp, vsum);
    sum = temp[0] + temp[1] + temp[2] + temp[3];
#elif defined(FINMATH_USE_SSE)
    __m128d vsum = _mm_setzero_pd();
    __m128d vzero = _mm_setzero_pd();
    
    for (; i + 2 <= size; i += 2) {
        __m128d va = _mm_loadu_pd(&a[i]);
        if (positive) {
            __m128d vpos = _mm_max_pd(vzero, va);
            vsum = _mm_add_pd(vsum, vpos);
        } else {
            __m128d vneg = _mm_sub_pd(vzero, va);
            __m128d vabs_neg = _mm_max_pd(vzero, vneg);
            vsum = _mm_add_pd(vsum, vabs_neg);
        }
    }
    
    double temp[2];
    _mm_storeu_pd(temp, vsum);
    sum = temp[0] + temp[1];
#elif defined(FINMATH_USE_NEON)
    float64x2_t vsum = vdupq_n_f64(0.0);
    float64x2_t vzero = vdupq_n_f64(0.0);
    
    for (; i + 2 <= size; i += 2) {
        float64x2_t va = vld1q_f64(&a[i]);
        if (positive) {
            float64x2_t vpos = vmaxq_f64(vzero, va);
            vsum = vaddq_f64(vsum, vpos);
        } else {
            float64x2_t vneg = vsubq_f64(vzero, va);
            float64x2_t vabs_neg = vmaxq_f64(vzero, vneg);
            vsum = vaddq_f64(vsum, vabs_neg);
        }
    }
    
    sum = vgetq_lane_f64(vsum, 0) + vgetq_lane_f64(vsum, 1);
#endif

    for (; i < size; ++i) {
        if (positive) {
            if (a[i] > 0) {
                sum += a[i];
            }
        } else {
            if (a[i] < 0) {
                sum += (-a[i]);
            }
        }
    }

    return sum;
}

} // namespace simd
} // namespace finmath

