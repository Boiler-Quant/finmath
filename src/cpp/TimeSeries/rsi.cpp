#include "finmath/TimeSeries/rsi.h"
#include <pybind11/numpy.h>    // Include numpy header
#include <pybind11/pybind11.h> // Include core pybind11 header for exceptions

#include <numeric>
#include <cmath>

namespace py = pybind11;

double compute_avg_gain(const std::vector<double> &price_changes, size_t start, size_t window_size)
{
    double total_gain = 0.0;

    for (size_t i = start; i < start + window_size; i++)
    {
        double price_change = price_changes[i];

        if (price_change > 0)
        {
            total_gain += price_change;
        }
    }
    return total_gain / window_size;
}

double compute_avg_loss(const std::vector<double> &price_changes, size_t start, size_t window_size)
{
    double total_loss = 0.0;

    for (size_t i = start; i < start + window_size; i++)
    {
        double price_change = price_changes[i];

        if (price_change < 0)
        {
            total_loss += (-1 * price_change);
        }
    }
    return total_loss / window_size;
}

std::vector<double> compute_smoothed_rsi(const std::vector<double> &prices, size_t window_size)
{
    if (prices.size() <= window_size)
    {   // Need > window_size prices for window_size changes
        // Return empty vector if not enough data
        // Could also throw: throw std::runtime_error("Insufficient data for the given window size.");
        return {};
    }
    if (window_size < 1)
    {
        throw std::runtime_error("Window size must be at least 1.");
    }

    std::vector<double> rsi_values;
    std::vector<double> price_changes;

    for (size_t i = 1; i < prices.size(); i++)
    {
        price_changes.push_back(prices[i] - prices[i - 1]);
    }

    size_t price_ch_window = window_size - 1;

    double avg_gain = compute_avg_gain(price_changes, 0, window_size);
    double avg_loss = compute_avg_loss(price_changes, 0, window_size);

    double rsi = 100;
    double rs;

    if (avg_loss != 0)
    {
        rs = avg_gain / avg_loss;
        rsi = 100.0 - (100.0 / (1.0 + rs));
    }

    rsi_values.push_back(rsi);

    for (size_t i = window_size - 1; i < price_changes.size(); i++)
    {
        double change = price_changes[i];

        avg_gain = (avg_gain * (window_size - 1) + (change > 0 ? change : 0)) / window_size;
        avg_loss = (avg_loss * (window_size - 1) - (change < 0 ? change : 0)) / window_size;

        if (avg_loss == 0)
        {
            rsi_values.push_back(100.0);
            continue;
        }

        rs = avg_gain / avg_loss;
        rsi = 100.0 - (100.0 / (1.0 + rs));
        rsi_values.push_back(rsi);
    }

    return rsi_values;
}

// Implementation for the NumPy array version
std::vector<double> compute_smoothed_rsi_np(py::array_t<double> prices_arr, size_t window_size)
{
    py::buffer_info buf_info = prices_arr.request();

    if (buf_info.ndim != 1)
    {
        throw std::runtime_error("Input array must be 1-dimensional.");
    }
    size_t num_prices = buf_info.size;

    if (num_prices <= window_size)
    { // Need > window_size prices for window_size changes
        // Return empty vector if not enough data
        // Could also throw: throw std::runtime_error("Insufficient data for the given window size.");
        return {};
    }
    if (window_size < 1)
    {
        throw std::runtime_error("Window size must be at least 1.");
    }

    const double *prices_ptr = static_cast<const double *>(buf_info.ptr);

    // --- Replicate logic using pointers ---
    std::vector<double> rsi_values;
    std::vector<double> price_changes;
    price_changes.reserve(num_prices - 1);

    for (size_t i = 1; i < num_prices; i++)
    {
        price_changes.push_back(prices_ptr[i] - prices_ptr[i - 1]);
    }

    // Note: The original compute_avg_gain/loss need modifying or
    //       we compute the initial gain/loss directly here.
    // Compute initial avg gain/loss directly from price_changes vector
    double initial_gain = 0.0;
    double initial_loss = 0.0;
    for (size_t i = 0; i < window_size; ++i)
    {
        if (price_changes[i] > 0)
            initial_gain += price_changes[i];
        else
            initial_loss += (-1 * price_changes[i]);
    }
    double avg_gain = initial_gain / window_size;
    double avg_loss = initial_loss / window_size;

    double rs = (avg_loss == 0) ? std::numeric_limits<double>::infinity() : avg_gain / avg_loss;
    double rsi = (avg_loss == 0) ? 100.0 : 100.0 - (100.0 / (1.0 + rs));

    rsi_values.push_back(rsi);
    rsi_values.reserve(num_prices - window_size); // Reserve estimated size

    // Compute subsequent smoothed RSI values
    for (size_t i = window_size; i < price_changes.size(); i++)
    {
        double change = price_changes[i];

        avg_gain = (avg_gain * (window_size - 1) + (change > 0 ? change : 0)) / window_size;
        avg_loss = (avg_loss * (window_size - 1) + (change < 0 ? -change : 0)) / window_size; // Fixed: was subtracting negative change

        if (avg_loss == 0)
        {
            rsi_values.push_back(100.0);
            continue;
        }

        rs = avg_gain / avg_loss;
        rsi = 100.0 - (100.0 / (1.0 + rs));
        rsi_values.push_back(rsi);
    }

    return rsi_values;
}
