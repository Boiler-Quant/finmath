#ifndef RSI_SIMD_H
#define RSI_SIMD_H

#include <vector>
#include <pybind11/numpy.h>

namespace py = pybind11;

/**
 * @brief SIMD-optimized Relative Strength Index calculation
 * 
 * Computes RSI using cross-platform SIMD optimizations.
 * Automatically selects the best SIMD backend (AVX, SSE, NEON, or scalar fallback).
 * 
 * @param prices_arr NumPy array of prices (zero-copy, no data duplication)
 * @param window_size Window size for RSI calculation
 * @return Vector of RSI values
 */
std::vector<double> compute_smoothed_rsi_simd(py::array_t<double> prices_arr, size_t window_size);

#endif // RSI_SIMD_H

