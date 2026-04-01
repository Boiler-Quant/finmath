#include "finmath/TimeSeries/rolling_volatility.h"
#include "finmath/Helper/simd_helper.h"

#include <cmath>
#include <stdexcept>
#include <vector>

namespace {

// Fixed-window rolling standard deviation using a sliding update (O(n)).
// Uses a Welford-style M2 accumulator for the initial window and a stable
// constant-time update for each slide.
std::vector<double> rolling_stddev_sliding(const std::vector<double>& data, size_t window_size) {
    if (window_size == 0) {
        throw std::runtime_error("Window size cannot be zero.");
    }
    if (data.size() < window_size) {
        throw std::runtime_error("Window size is too large for the input series.");
    }
    if (data.empty()) {
        return {};
    }

    const size_t out_size = data.size() - window_size + 1;
    std::vector<double> out;
    out.reserve(out_size);

    double mean = 0.0;
    double m2 = 0.0;
    for (size_t i = 0; i < window_size; ++i) {
        const double x = data[i];
        const double old_mean = mean;
        mean = mean + (x - mean) / static_cast<double>(i + 1);
        m2 = m2 + (x - old_mean) * (x - mean);
    }

    const double inv_w = 1.0 / static_cast<double>(window_size);
    out.push_back(std::sqrt(std::max(0.0, m2 * inv_w)));

    double x_out = data[0];
    for (size_t i = window_size; i < data.size(); ++i) {
        const double x_in = data[i];
        const double old_mean = mean;
        mean += (x_in - x_out) * inv_w;
        m2 += (x_in - mean) * (x_in - old_mean)
            - (x_out - mean) * (x_out - old_mean);

        out.push_back(std::sqrt(std::max(0.0, m2 * inv_w)));
        x_out = data[i - window_size + 1];
    }

    return out;
}

} // namespace

// Function to compute the logarithmic returns
std::vector<double> compute_log_returns(const std::vector<double> &prices)
{
    if (prices.size() < 2) return {};

    std::vector<double> log_returns;
    log_returns.reserve(prices.size() - 1);
    for (size_t i = 1; i < prices.size(); ++i) {
        const double p0 = prices[i - 1];
        const double p1 = prices[i];
        if (p0 <= 0.0 || p1 <= 0.0) {
            throw std::runtime_error("All prices must be positive for log return calculation.");
        }
        log_returns.push_back(std::log(p1 / p0));
    }
    return log_returns;
}

// Function to compute the standard deviation of a vector (using SIMD when possible)
double compute_std(const std::vector<double> &data)
{
    if (data.empty()) {
        return 0.0;
    }
    
    // Use SIMD-accelerated standard deviation calculation
    return finmath::simd::vector_stddev(data.data(), data.size());
}

// Function to compute rolling volatility
std::vector<double> rolling_volatility(const std::vector<double> &prices, size_t window_size)
{
    if (window_size == 0) {
        throw std::runtime_error("Window size cannot be zero.");
    }
    if (prices.size() < 2) {
        throw std::runtime_error("Insufficient data: requires at least 2 prices.");
    }

    const std::vector<double> log_returns = compute_log_returns(prices);
    if (log_returns.size() < window_size) {
        throw std::runtime_error("Window size is too large for the number of price returns.");
    }

    const double annualization_factor = std::sqrt(252.0);
    std::vector<double> stds = rolling_stddev_sliding(log_returns, window_size);
    for (double& s : stds) s *= annualization_factor;
    return stds;
}