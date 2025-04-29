#include "finmath/TimeSeries/ema.h"
#include <pybind11/numpy.h>    // Include numpy header
#include <pybind11/pybind11.h> // Include core pybind11 header for exceptions

// Compute EMA using window size (list version)
std::vector<double> compute_ema(const std::vector<double> &prices, size_t window)
{
    if (window == 0)
    {
        throw std::runtime_error("EMA window cannot be zero.");
    }
    if (prices.empty())
    {
        return {};
    }
    double multiplier = 2.0 / (static_cast<double>(window) + 1.0);
    return compute_ema_with_smoothing(prices, multiplier);
}

// Compute EMA using a specified smoothing factor (list version)
std::vector<double> compute_ema_with_smoothing(const std::vector<double> &prices, double smoothing_factor)
{
    if (smoothing_factor <= 0 || smoothing_factor >= 1)
    {
        throw std::runtime_error("EMA smoothing factor must be between 0 and 1 (exclusive).");
    }
    if (prices.empty())
    {
        return {};
    }
    std::vector<double> ema(prices.size(), 0.0);
    ema[0] = prices[0]; // Initialize the first EMA value

    for (size_t i = 1; i < prices.size(); ++i)
    {
        ema[i] = ((prices[i] - ema[i - 1]) * smoothing_factor) + ema[i - 1];
    }

    return ema;
}

// --- NumPy Versions ---

// Compute EMA using window size (NumPy version)
std::vector<double> compute_ema_np(py::array_t<double> prices_arr, size_t window)
{
    if (window == 0)
    {
        throw std::runtime_error("EMA window cannot be zero.");
    }
    double multiplier = 2.0 / (static_cast<double>(window) + 1.0);
    // Delegate to the smoothing factor NumPy version
    return compute_ema_with_smoothing_np(prices_arr, multiplier);
}

// Compute EMA using a specified smoothing factor (NumPy version)
std::vector<double> compute_ema_with_smoothing_np(py::array_t<double> prices_arr, double smoothing_factor)
{
    py::buffer_info buf_info = prices_arr.request();
    if (buf_info.ndim != 1)
    {
        throw std::runtime_error("Input array must be 1-dimensional.");
    }
    size_t num_prices = buf_info.size;

    if (smoothing_factor <= 0 || smoothing_factor >= 1)
    {
        throw std::runtime_error("EMA smoothing factor must be between 0 and 1 (exclusive).");
    }
    if (num_prices == 0)
    {
        return {};
    }

    const double *prices_ptr = static_cast<const double *>(buf_info.ptr);
    std::vector<double> ema(num_prices, 0.0);
    ema[0] = prices_ptr[0]; // Initialize the first EMA value

    for (size_t i = 1; i < num_prices; ++i)
    {
        ema[i] = ((prices_ptr[i] - ema[i - 1]) * smoothing_factor) + ema[i - 1];
    }

    return ema;
}
