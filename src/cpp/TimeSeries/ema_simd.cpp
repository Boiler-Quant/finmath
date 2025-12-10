#include "finmath/TimeSeries/ema_simd.h"
#include <cmath>
#include <stdexcept>
#include <vector>

std::vector<double> compute_ema_simd(py::array_t<double> prices_arr, size_t window)
{
    if (window == 0) {
        throw std::runtime_error("EMA window cannot be zero.");
    }
    
    double multiplier = 2.0 / (static_cast<double>(window) + 1.0);
    return compute_ema_with_smoothing_simd(prices_arr, multiplier);
}

std::vector<double> compute_ema_with_smoothing_simd(py::array_t<double> prices_arr, double smoothing_factor)
{
    // Get buffer info for zero-copy access
    py::buffer_info buf_info = prices_arr.request();

    // Validate input
    if (buf_info.ndim != 1) {
        throw std::runtime_error("Input must be a 1-dimensional array");
    }

    size_t num_prices = static_cast<size_t>(buf_info.shape[0]);
    
    if (smoothing_factor <= 0 || smoothing_factor >= 1) {
        throw std::runtime_error("EMA smoothing factor must be between 0 and 1 (exclusive).");
    }
    
    if (num_prices == 0) {
        return {};
    }

    // Zero-copy access to NumPy data
    const double* prices_ptr = static_cast<const double*>(buf_info.ptr);
    
    if (!prices_ptr) {
        throw std::runtime_error("Invalid buffer pointer from NumPy array");
    }

    // EMA calculation: ema[i] = (prices[i] - ema[i-1]) * smoothing_factor + ema[i-1]
    // This can be rewritten as: ema[i] = prices[i] * smoothing_factor + ema[i-1] * (1 - smoothing_factor)
    std::vector<double> ema(num_prices, 0.0);
    ema[0] = prices_ptr[0]; // Initialize the first EMA value

    // Pre-compute (1 - smoothing_factor) for efficiency
    double one_minus_smoothing = 1.0 - smoothing_factor;

    // Sequential calculation (EMA is inherently sequential, but we can optimize the arithmetic)
    for (size_t i = 1; i < num_prices; ++i) {
        // ema[i] = prices[i] * smoothing_factor + ema[i-1] * (1 - smoothing_factor)
        ema[i] = prices_ptr[i] * smoothing_factor + ema[i - 1] * one_minus_smoothing;
    }

    // Note: EMA is inherently sequential (each value depends on the previous),
    // so we can't fully parallelize it. However, we could use SIMD for batch processing
    // if we process multiple time series in parallel. For now, we keep it sequential
    // but use optimized arithmetic operations.

    return ema;
}

