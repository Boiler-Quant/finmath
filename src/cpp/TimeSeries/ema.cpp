#include "finmath/TimeSeries/ema.h"

std::vector<double> compute_ema(const std::vector<double>& prices, size_t window)
{
    std::vector<double> ema(prices.size(), 0.0);
    double multiplier = 2.0 / (window + 1);

    ema = compute_ema_with_smoothing(prices, multiplier);
    return ema;
}

// Compute EMA using a specified smoothing factor
std::vector<double> compute_ema_with_smoothing(const std::vector<double>& prices, double smoothing_factor)
{
    std::vector<double> ema(prices.size(), 0.0);
    ema[0] = prices[0]; // Initialize the first EMA value

    for (size_t i = 1; i < prices.size(); ++i) {
        ema[i] = ((prices[i] - ema[i - 1]) * smoothing_factor) + ema[i - 1];
    }

    return ema;
}
