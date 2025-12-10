#include "finmath/TimeSeries/simple_moving_average_simd.h"
#include "finmath/Helper/simd_helper.h"
#include <cmath>
#include <stdexcept>

std::vector<double> simple_moving_average_simd(py::array_t<double> data_arr, size_t window_size)
{
    // Get buffer info for zero-copy access
    py::buffer_info buf_info = data_arr.request();

    // Validate input
    if (buf_info.ndim != 1) {
        throw std::runtime_error("Input must be a 1-dimensional array");
    }

    size_t num_data = static_cast<size_t>(buf_info.shape[0]);
    
    if (window_size == 0) {
        throw std::runtime_error("Window size must be greater than 0");
    }

    if (num_data < window_size) {
        return {};
    }

    // Zero-copy access to NumPy data
    const double* data_ptr = static_cast<const double*>(buf_info.ptr);
    
    if (!data_ptr) {
        throw std::runtime_error("Invalid buffer pointer from NumPy array");
    }

    // Calculate moving averages using SIMD
    std::vector<double> averages;
    size_t num_windows = num_data - window_size + 1;
    averages.reserve(num_windows);

    // Use SIMD-accelerated sum for each window
    for (size_t i = 0; i < num_windows; ++i) {
        // Get pointer to current window
        const double* window_data = &data_ptr[i];
        
        // SIMD-accelerated sum calculation
        double sum = finmath::simd::vector_sum(window_data, window_size);
        
        // Compute average
        double avg = sum / static_cast<double>(window_size);
        averages.push_back(avg);
    }

    return averages;
}

