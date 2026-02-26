#ifndef SIMPLE_MOVING_AVERAGE_SIMD_H
#define SIMPLE_MOVING_AVERAGE_SIMD_H

#include <vector>
#include <pybind11/numpy.h>

namespace py = pybind11;

/**
 * @brief SIMD-optimized simple moving average calculation
 * 
 * Computes moving average using cross-platform SIMD optimizations.
 * Automatically selects the best SIMD backend (AVX, SSE, NEON, or scalar fallback).
 * 
 * @param data_arr NumPy array of data (zero-copy, no data duplication)
 * @param window_size Rolling window size
 * @return Vector of moving average values
 */
std::vector<double> simple_moving_average_simd(py::array_t<double> data_arr, size_t window_size);

#endif // SIMPLE_MOVING_AVERAGE_SIMD_H

