#ifndef EMA_SIMD_H
#define EMA_SIMD_H

#include <vector>
#include <pybind11/numpy.h>

namespace py = pybind11;

/**
 * @brief SIMD-optimized exponential moving average calculation
 * 
 * Computes EMA using cross-platform SIMD optimizations.
 * Automatically selects the best SIMD backend (AVX, SSE, NEON, or scalar fallback).
 * 
 * @param prices_arr NumPy array of prices (zero-copy, no data duplication)
 * @param window Window size for EMA calculation
 * @return Vector of EMA values
 */
std::vector<double> compute_ema_simd(py::array_t<double> prices_arr, size_t window);

/**
 * @brief SIMD-optimized exponential moving average with smoothing factor
 * 
 * @param prices_arr NumPy array of prices (zero-copy, no data duplication)
 * @param smoothing_factor Smoothing factor (typically 2.0 / (window + 1))
 * @return Vector of EMA values
 */
std::vector<double> compute_ema_with_smoothing_simd(py::array_t<double> prices_arr, double smoothing_factor);

#endif // EMA_SIMD_H

