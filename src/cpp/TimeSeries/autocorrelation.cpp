#include "finmath/TimeSeries/autocorrelation.h"

#include <cmath>
#include <numeric>
#include <stdexcept>

std::vector<double> autocorrelation(const std::vector<double>& values, int max_lag)
{
    if (max_lag < 0) {
        throw std::invalid_argument("autocorrelation: max_lag must be >= 0");
    }
    const int n = static_cast<int>(values.size());
    if (n == 0) {
        return {};
    }
    if (max_lag >= n) {
        throw std::invalid_argument("autocorrelation: max_lag must be < number of observations");
    }

    double mean = std::accumulate(values.begin(), values.end(), 0.0) / static_cast<double>(n);
    double denom = 0.0;
    for (double v : values) {
        const double d = v - mean;
        denom += d * d;
    }
    if (denom == 0.0) {
        throw std::invalid_argument("autocorrelation: zero variance");
    }

    std::vector<double> out(static_cast<size_t>(max_lag + 1), 0.0);
    for (int lag = 0; lag <= max_lag; ++lag) {
        double num = 0.0;
        for (int t = 0; t < n - lag; ++t) {
            num += (values[static_cast<size_t>(t)] - mean) * (values[static_cast<size_t>(t + lag)] - mean);
        }
        out[static_cast<size_t>(lag)] = num / denom;
    }
    return out;
}
