#include <cmath>
#include "finmath/Helper/helper.h"


// Standard normal cumulative distribution function
inline double normal_cdf(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2));
}

// approximated CDF: refer to https://en.wikipedia.org/wiki/Error_function (Abramowitz and Stegun)
inline double normal_cdf_approx(double x) noexcept {
    constexpr double p = 0.3275911;
    constexpr double a1 = 0.254829592;
    constexpr double a2 = -0.284496736;
    constexpr double a3 = 1.421413741;
    constexpr double a4 = -1.453152027;
    constexpr double a5 = 1.061405429;

    double sign = (x < 0) ? -1.0 : 1.0;
    x = std::fabs(x);

    double t = 1.0 / (1.0 + p * x);
    double erf_approx = 1.0 - (t * (a1 + t * (a2 + t * (a3 + t * (a4 + t * a5)))) * std::exp(-x * x);

    return 0.5 * (1.0 + sign * erf_approx);
}


// Standard normal probability density function
inline double normal_pdf(double x) {
    static constexpr double inv_sqrt_2pi = 1.0 / std::sqrt(2.0 * M_PI);
    return inv_sqrt_2pi * std::exp(-0.5 * x * x);
}

long long combinations(long n, long k) {
    // Ensure k <= n - k to minimize operations
    if (k > n - k) {
        k = n - k;
    }

    long long result = 1;  // Use long long to handle large numbers

    // Efficiently compute the binomial coefficient
    for (long i = 0; i < k; ++i) {
        result *= (n - i);    // Multiply by (n - i)
        result /= (i + 1);    // Divide by (i + 1)
    }

    return result;
}

