#include "finmath/TimeSeries/rsi_simd.h"
#include "finmath/Helper/simd_helper.h"
#include <cmath>
#include <stdexcept>
#include <vector>
#include <limits>

std::vector<double> compute_smoothed_rsi_simd(py::array_t<double> prices_arr, size_t window_size)
{
    // Get buffer info for zero-copy access
    py::buffer_info buf_info = prices_arr.request();

    // Validate input
    if (buf_info.ndim != 1) {
        throw std::runtime_error("Input must be a 1-dimensional array");
    }

    size_t num_prices = static_cast<size_t>(buf_info.shape[0]);
    
    if (num_prices <= window_size) {
        return {};
    }
    
    if (window_size < 1) {
        throw std::runtime_error("Window size must be at least 1.");
    }

    // Zero-copy access to NumPy data
    const double* prices_ptr = static_cast<const double*>(buf_info.ptr);
    
    if (!prices_ptr) {
        throw std::runtime_error("Invalid buffer pointer from NumPy array");
    }

    // Pre-compute price changes
    // Note: Cannot use vector_sub directly due to offset (prices[i+1] - prices[i])
    // This is a sequential operation, but we'll use SIMD for the gain/loss calculations
    std::vector<double> price_changes(num_prices - 1);
    for (size_t i = 0; i < num_prices - 1; ++i) {
        price_changes[i] = prices_ptr[i + 1] - prices_ptr[i];
    }

    // Compute initial average gain and loss using SIMD
    const double* initial_window = price_changes.data();
    
    // SIMD-accelerated conditional sums
    double initial_gain = finmath::simd::vector_conditional_sum(initial_window, window_size, true);
    double initial_loss = finmath::simd::vector_conditional_sum(initial_window, window_size, false);
    
    double avg_gain = initial_gain / static_cast<double>(window_size);
    double avg_loss = initial_loss / static_cast<double>(window_size);

    // Calculate first RSI value
    std::vector<double> rsi_values;
    rsi_values.reserve(num_prices - window_size);
    
    double rs = (avg_loss == 0) ? std::numeric_limits<double>::infinity() : avg_gain / avg_loss;
    double rsi = (avg_loss == 0) ? 100.0 : 100.0 - (100.0 / (1.0 + rs));
    rsi_values.push_back(rsi);

    // Compute subsequent smoothed RSI values
    // Note: The smoothing calculation is sequential (each depends on previous),
    // but we've optimized the initial gain/loss calculation with SIMD
    for (size_t i = window_size; i < price_changes.size(); ++i) {
        double change = price_changes[i];

        // Smoothed average: new_avg = (old_avg * (n-1) + new_value) / n
        avg_gain = (avg_gain * (window_size - 1) + (change > 0 ? change : 0)) / window_size;
        avg_loss = (avg_loss * (window_size - 1) + (change < 0 ? -change : 0)) / window_size;

        if (avg_loss == 0) {
            rsi_values.push_back(100.0);
            continue;
        }

        rs = avg_gain / avg_loss;
        rsi = 100.0 - (100.0 / (1.0 + rs));
        rsi_values.push_back(rsi);
    }

    return rsi_values;
}

