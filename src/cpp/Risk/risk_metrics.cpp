#include "finmath/Risk/risk_metrics.h"

#include "finmath/Helper/helper.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace {

double inverse_normal_cdf(double p)
{
    if (p <= 0.0 || p >= 1.0) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    double lo = -10.0;
    double hi = 10.0;
    for (int i = 0; i < 80; ++i) {
        const double mid = 0.5 * (lo + hi);
        if (normal_cdf(mid) < p) {
            lo = mid;
        } else {
            hi = mid;
        }
    }
    return 0.5 * (lo + hi);
}

}  // namespace

double historical_var(const std::vector<double>& returns, double alpha)
{
    if (alpha <= 0.0 || alpha >= 1.0) {
        throw std::invalid_argument("historical_var: alpha must be in (0,1)");
    }
    if (returns.size() < 2) {
        throw std::invalid_argument("historical_var: need at least 2 observations");
    }
    std::vector<double> sorted = returns;
    std::sort(sorted.begin(), sorted.end());
    const size_t n = sorted.size();
    const size_t idx = static_cast<size_t>(std::floor(alpha * static_cast<double>(n - 1)));
    return -sorted[idx];
}

double historical_cvar(const std::vector<double>& returns, double alpha)
{
    if (alpha <= 0.0 || alpha >= 1.0) {
        throw std::invalid_argument("historical_cvar: alpha must be in (0,1)");
    }
    if (returns.empty()) {
        throw std::invalid_argument("historical_cvar: empty returns");
    }
    std::vector<double> sorted = returns;
    std::sort(sorted.begin(), sorted.end());
    const size_t n = sorted.size();
    const size_t k = std::max<size_t>(1, static_cast<size_t>(std::ceil(alpha * static_cast<double>(n))));
    double sum = 0.0;
    for (size_t i = 0; i < k; ++i) {
        sum += sorted[i];
    }
    const double mean_tail = sum / static_cast<double>(k);
    return -mean_tail;
}

double parametric_var(double mean, double std_dev, double alpha)
{
    if (alpha <= 0.0 || alpha >= 1.0) {
        throw std::invalid_argument("parametric_var: alpha must be in (0,1)");
    }
    if (std_dev < 0.0) {
        throw std::invalid_argument("parametric_var: std_dev must be >= 0");
    }
    const double z = inverse_normal_cdf(alpha);
    return -(mean + z * std_dev);
}
