#include "finmath/TimeSeries/simple_moving_average.h"
#include <pybind11/numpy.h>    // Include numpy header
#include <pybind11/pybind11.h> // Include core pybind11 header for exceptions

#include <numeric>
#include <vector>

namespace py = pybind11;

std::vector<double> simple_moving_average(const std::vector<double> &data, size_t window_size)
{
    std::vector<double> averages;

    // Check for valid window size
    if (window_size == 0)
    {
        throw std::runtime_error("Window size must be greater than 0.");
    }

    if (data.size() < window_size)
    {
        return {};
    }

    // Compute moving averages using a sliding window
    for (size_t i = 0; i <= data.size() - window_size; ++i)
    {
        // Calculate the sum of the current window
        double sum = std::accumulate(data.begin() + i, data.begin() + i + window_size, 0.0);

        // Compute the average and store it
        double avg = sum / static_cast<double>(window_size);
        averages.push_back(avg);
    }

    return averages;
}

// Implementation for the NumPy array version
std::vector<double> simple_moving_average_np(py::array_t<double> data_arr, size_t window_size)
{
    py::buffer_info buf_info = data_arr.request();

    if (buf_info.ndim != 1)
    {
        throw std::runtime_error("Input array must be 1-dimensional.");
    }

    size_t num_data = buf_info.size;

    if (window_size == 0)
    {
        throw std::runtime_error("Window size must be greater than 0.");
    }

    if (num_data < window_size)
    {
        // Return empty vector if not enough data for one window
        // Alternatively, throw: throw std::runtime_error("Data size is smaller than the window size.");
        return {};
    }

    const double *data_ptr = static_cast<const double *>(buf_info.ptr);
    std::vector<double> averages;
    averages.reserve(num_data - window_size + 1);

    // Compute moving averages using a sliding window over the pointer
    double current_sum = std::accumulate(data_ptr, data_ptr + window_size, 0.0);
    averages.push_back(current_sum / static_cast<double>(window_size));

    for (size_t i = window_size; i < num_data; ++i)
    {
        current_sum += data_ptr[i] - data_ptr[i - window_size]; // More efficient sliding window sum
        double avg = current_sum / static_cast<double>(window_size);
        averages.push_back(avg);
    }

    return averages;
}
