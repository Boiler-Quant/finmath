#include "finmath/TimeSeries/returns.h"

#include <cmath>
#include <stdexcept>

std::vector<double> log_returns(const std::vector<double>& prices)
{
    if (prices.empty()) {
        return {};
    }

    std::vector<double> out(prices.size(), 0.0);
    for (size_t i = 1; i < prices.size(); ++i) {
        const double prev = prices[i - 1];
        const double curr = prices[i];
        if (prev <= 0.0 || curr <= 0.0) {
            throw std::invalid_argument("log_returns requires all prices > 0");
        }
        out[i] = std::log(curr / prev);
    }
    return out;
}

std::vector<double> pct_returns(const std::vector<double>& prices)
{
    if (prices.empty()) {
        return {};
    }

    std::vector<double> out(prices.size(), 0.0);
    for (size_t i = 1; i < prices.size(); ++i) {
        const double prev = prices[i - 1];
        const double curr = prices[i];
        if (prev == 0.0) {
            throw std::invalid_argument("pct_returns requires previous price != 0");
        }
        out[i] = (curr / prev) - 1.0;
    }
    return out;
}
