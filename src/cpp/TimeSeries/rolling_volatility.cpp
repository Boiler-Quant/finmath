#include "finmath/TimeSeries/rolling_volatility.h"
#include <pybind11/numpy.h>    // Include numpy header
#include <pybind11/pybind11.h> // Include core pybind11 header for exceptions

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

// Function to compute the logarithmic returns
std::vector<double> compute_log_returns(const std::vector<double> &prices)
{
    std::vector<double> log_returns;
    for (size_t i = 1; i < prices.size(); ++i)
    {
        log_returns.push_back(std::log(prices[i] / prices[i - 1]));
    }
    return log_returns;
}

// Function to compute the standard deviation of a vector
double compute_std(const std::vector<double> &data)
{
    double mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    double sq_sum = std::inner_product(data.begin(), data.end(), data.begin(), 0.0);
    return std::sqrt(sq_sum / data.size() - mean * mean);
}

// Function to compute rolling volatility
std::vector<double> rolling_volatility(const std::vector<double> &prices, size_t window_size)
{
    std::vector<double> volatilities;

    // Compute log returns
    std::vector<double> log_returns = compute_log_returns(prices);

    // Check if window size is valid relative to log returns size
    if (window_size == 0)
    {
        throw std::runtime_error("Window size cannot be zero.");
    }
    if (log_returns.empty() || log_returns.size() < window_size)
    {
        // Cannot compute volatility if not enough log returns for the window
        // Option 1: Throw error
        throw std::runtime_error("Window size is too large for the number of price returns.");
        // Option 2: Return empty vector
        // return {};
    }

    // Rolling window calculation
    volatilities.reserve(log_returns.size() - window_size + 1); // Reserve space
    for (size_t i = 0; i <= log_returns.size() - window_size; ++i)
    {
        // Get the window of log returns
        std::vector<double> window(log_returns.begin() + i, log_returns.begin() + i + window_size);

        // Compute the standard deviation
        double std_dev = compute_std(window);

        // Annualize the standard deviation (multiply by sqrt(252))
        double annualized_vol = std_dev * std::sqrt(252);

        // Store the result
        volatilities.push_back(annualized_vol);
    }

    return volatilities;
}

// Implementation for the NumPy array version
std::vector<double> rolling_volatility_np(py::array_t<double> prices_arr, size_t window_size)
{
    // Request buffer information from the NumPy array
    py::buffer_info buf_info = prices_arr.request();

    // Check dimensions (should be 1D)
    if (buf_info.ndim != 1)
    {
        throw std::runtime_error("Input array must be 1-dimensional.");
    }

    // Get size after checking dimension
    size_t num_prices = buf_info.size;

    // Check if window size and input size are valid *before* accessing pointer or calculating reserves
    if (window_size == 0)
    {
        throw std::runtime_error("Window size cannot be zero.");
    }
    if (num_prices < 2)
    {
        // Handle cases with 0 or 1 price: cannot compute returns/volatility
        // Option 1: Throw error (Restoring this)
        throw std::runtime_error("Insufficient data: requires at least 2 prices.");
        // Option 2: Return empty vector (did not fix segfault)
        // return {};
    }
    if (window_size >= num_prices)
    {
        throw std::runtime_error("Window size must be smaller than the number of prices.");
    }

    // Get pointer to the data only after size checks pass
    const double *prices_ptr = static_cast<const double *>(buf_info.ptr);

    std::vector<double> volatilities;
    // Now it's safe to calculate reserve size: num_prices >= 2, window_size >= 1, num_prices > window_size
    // Log returns size will be num_prices - 1. Result size will be (num_prices - 1) - window_size + 1
    size_t expected_result_size = num_prices - window_size;
    volatilities.reserve(expected_result_size);

    // 1. Compute log returns
    std::vector<double> log_returns;
    log_returns.reserve(num_prices - 1);
    for (size_t i = 1; i < num_prices; ++i)
    {
        if (prices_ptr[i - 1] <= 0)
            throw std::runtime_error("Price must be positive for log return calculation.");
        log_returns.push_back(std::log(prices_ptr[i] / prices_ptr[i - 1]));
    }

    // This check might be redundant now given the earlier checks, but keep for safety
    if (log_returns.size() < window_size)
    {
        throw std::runtime_error("Window size is larger than the number of log returns.");
    }

    // 2. Rolling window calculation using the existing compute_std
    for (size_t i = 0; i <= log_returns.size() - window_size; ++i)
    {
        // Create a temporary window vector
        std::vector<double> window(log_returns.begin() + i, log_returns.begin() + i + window_size);

        // Compute the standard deviation
        double std_dev = compute_std(window);

        // Annualize the standard deviation
        double annualized_vol = std_dev * std::sqrt(252);

        // Store the result
        volatilities.push_back(annualized_vol);
    }

    return volatilities;
}