#include "finmath/TimeSeries/rolling_volatility_simd.h"
#include "finmath/Helper/simd_helper.h"
#include <cmath>
#include <stdexcept>

std::vector<double> rolling_volatility_simd(py::array_t<double> prices_arr, size_t window_size)
{
    // Get buffer info for zero-copy access
    py::buffer_info buf_info = prices_arr.request();

    // Validate input
    if (buf_info.ndim != 1) {
        throw std::runtime_error("Input must be a 1-dimensional array");
    }

    size_t num_prices = static_cast<size_t>(buf_info.shape[0]);
    
    if (num_prices < window_size + 1) {
        throw std::runtime_error("Need at least window_size + 1 prices");
    }

    if (window_size < 2) {
        throw std::runtime_error("Window size must be at least 2");
    }

    // Zero-copy access to NumPy data
    const double* prices_ptr = static_cast<const double*>(buf_info.ptr);
    
    if (!prices_ptr) {
        throw std::runtime_error("Invalid buffer pointer from NumPy array");
    }

    // Pre-compute log returns (vectorized)
    std::vector<double> log_returns(num_prices - 1);
    
    for (size_t i = 0; i < num_prices - 1; ++i) {
        if (prices_ptr[i] <= 0 || prices_ptr[i + 1] <= 0) {
            throw std::runtime_error("All prices must be positive for log return calculation");
        }
        log_returns[i] = std::log(prices_ptr[i + 1] / prices_ptr[i]);
    }

    // Calculate rolling volatility using SIMD
    std::vector<double> volatilities;
    size_t num_windows = num_prices - window_size;
    volatilities.reserve(num_windows);

    const double annualization_factor = std::sqrt(252.0);

    // Use cache-blocked version for large windows or large datasets
    // Threshold: use cache-blocking if window > 1000 or total data > 100K
    bool use_cache_blocking = (window_size > 1000) || (num_prices > 100000);
    
    for (size_t i = 0; i < num_windows; ++i) {
        // Get pointer to current window in log returns
        const double* window_data = &log_returns[i];
        
        // SIMD-accelerated standard deviation calculation
        // Use cache-blocked version for large windows to improve cache utilization
        double std_dev = use_cache_blocking 
            ? finmath::simd::vector_stddev_blocked(window_data, window_size)
            : finmath::simd::vector_stddev(window_data, window_size);
        
        // Annualize
        double volatility = std_dev * annualization_factor;
        volatilities.push_back(volatility);
    }

    return volatilities;
}

