#include "finmath/TimeSeries/rolling_zscore.h"

#include <cmath>
#include <stdexcept>

std::vector<double> rolling_mean(size_t window_size, const std::vector<double>& data)
{
    if (window_size == 0) {
        throw std::invalid_argument("Window size cannot be zero.");
    }

    std::vector<double> result(data.size(), 0.0);
    if (data.empty()) {
        return result;
    }

    size_t w = window_size;
    if (w > data.size()) {
        w = data.size();
    }

    double sum = 0.0;
    for (size_t i = 0; i < w; ++i) {
        sum += data[i];
    }
    result[w - 1] = sum / static_cast<double>(w);

    for (size_t i = w; i < data.size(); ++i) {
        sum += data[i] - data[i - w];
        result[i] = sum / static_cast<double>(w);
    }

    return result;
}

std::vector<double> rolling_zscore(size_t window_size, const std::vector<double>& data)
{
    if (window_size == 0) {
        throw std::invalid_argument("Window size cannot be zero.");
    }

    std::vector<double> result(data.size(), 0.0);
    if (data.empty()) {
        return result;
    }

    size_t w = window_size;
    if (w > data.size()) {
        w = data.size();
    }

    double sum = 0.0;
    double sumsq = 0.0;
    for (size_t i = 0; i < w; ++i) {
        sum += data[i];
        sumsq += data[i] * data[i];
    }

    auto write = [&](size_t idx) {
        const double mean = sum / static_cast<double>(w);
        const double ex2 = sumsq / static_cast<double>(w);
        double var = ex2 - mean * mean;
        if (var < 0.0) {
            var = 0.0;
        }
        const double std = std::sqrt(var);
        if (std == 0.0) {
            result[idx] = 0.0;
            return;
        }
        result[idx] = (data[idx] - mean) / std;
    };

    write(w - 1);
    for (size_t i = w; i < data.size(); ++i) {
        const double outv = data[i - w];
        const double inv = data[i];
        sum += inv - outv;
        sumsq += inv * inv - outv * outv;
        write(i);
    }

    return result;
}
