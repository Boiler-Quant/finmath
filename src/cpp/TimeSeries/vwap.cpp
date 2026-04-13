#include "finmath/TimeSeries/vwap.h"

#include <stdexcept>

static void validate_inputs(const std::vector<double>& prices, const std::vector<double>& volumes, size_t window_size)
{
    if (window_size == 0) {
        throw std::invalid_argument("Window size must be greater than 0.");
    }
    if (prices.size() != volumes.size()) {
        throw std::invalid_argument("Prices and volumes must have the same length.");
    }
}

std::vector<double> vwap(const std::vector<double>& prices, const std::vector<double>& volumes, size_t window_size)
{
    validate_inputs(prices, volumes, window_size);
    if (prices.size() < window_size) {
        return {};
    }

    const size_t n = prices.size();
    std::vector<double> out;
    out.reserve(n - window_size + 1);

    double pv_sum = 0.0;
    double v_sum = 0.0;
    for (size_t i = 0; i < window_size; ++i) {
        pv_sum += prices[i] * volumes[i];
        v_sum += volumes[i];
    }

    auto emit = [&]() {
        if (v_sum == 0.0) {
            out.push_back(0.0);
        } else {
            out.push_back(pv_sum / v_sum);
        }
    };

    emit();
    for (size_t i = window_size; i < n; ++i) {
        pv_sum += prices[i] * volumes[i] - prices[i - window_size] * volumes[i - window_size];
        v_sum += volumes[i] - volumes[i - window_size];
        emit();
    }

    return out;
}

std::vector<double> rolling_dollar_volume(const std::vector<double>& prices, const std::vector<double>& volumes,
                                            size_t window_size)
{
    validate_inputs(prices, volumes, window_size);
    if (prices.size() < window_size) {
        return {};
    }

    const size_t n = prices.size();
    std::vector<double> out;
    out.reserve(n - window_size + 1);

    double sum = 0.0;
    for (size_t i = 0; i < window_size; ++i) {
        sum += prices[i] * volumes[i];
    }
    out.push_back(sum);

    for (size_t i = window_size; i < n; ++i) {
        sum += prices[i] * volumes[i] - prices[i - window_size] * volumes[i - window_size];
        out.push_back(sum);
    }

    return out;
}
