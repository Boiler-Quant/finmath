#ifndef ROLLING_VOLATILITY_SIMD_H
#define ROLLING_VOLATILITY_SIMD_H

#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

/**
 * @brief SIMD-optimized rolling volatility calculation
 * 
 * Computes annualized rolling volatility using cross-platform SIMD optimizations.
 * Automatically selects the best SIMD backend (AVX, SSE, NEON, or scalar fallback).
 * 
 * Uses log returns: ln(P_t / P_{t-1})
 * Volatility = std_dev(returns) * sqrt(252)
 * 
 * @param prices_arr NumPy array of prices (zero-copy, no data duplication)
 * @param window_size Rolling window size
 * @return Vector of annualized volatility values
 */
std::vector<double> rolling_volatility_simd(py::array_t<double> prices_arr, size_t window_size);

#endif // ROLLING_VOLATILITY_SIMD_H

