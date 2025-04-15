#include <cmath>
#include "finmath/Helper/helper.h"
#include <xsimd/xsimd.hpp>

// Standard normal cumulative distribution function
double normal_cdf(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2));
}

// approximated CDF: refer to https://en.wikipedia.org/wiki/Error_function (Abramowitz and Stegun)
double normal_cdf_approx(double x) noexcept {
    constexpr double p = 0.3275911;
    constexpr double a1 = 0.254829592;
    constexpr double a2 = -0.284496736;
    constexpr double a3 = 1.421413741;
    constexpr double a4 = -1.453152027;
    constexpr double a5 = 1.061405429;

    double sign = (x < 0) ? -1.0 : 1.0;
    x = std::fabs(x);

    double t = 1.0 / (1.0 + p * x);
    double erf_approx = 1.0 - (t * (a1 + t * (a2 + t * (a3 + t * (a4 + t * a5))))) * std::exp(-x * x);

    return 0.5 * (1.0 + sign * erf_approx);
}

xsimd::batch<double> normal_cdf_approx(const xsimd::batch<double>& x) noexcept {
    constexpr double p = 0.3275911;
    constexpr double a1 = 0.254829592;
    constexpr double a2 = -0.284496736;
    constexpr double a3 = 1.421413741;
    constexpr double a4 = -1.453152027;
    constexpr double a5 = 1.061405429;

    // Use SIMD equivalents for fabs and exp
    
    
    xsimd::batch<double> sign = xsimd::select(x < 0.0, xsimd::batch<double>(-1.0), xsimd::batch<double>(1.0));

    xsimd::batch<double> abs_x = xsimd::fabs(x);  // Absolute value

    // Polynomial approximation
    xsimd::batch<double> t = 1.0 / (1.0 + p * abs_x);
    xsimd::batch<double> polynomial = a1 + t * (a2 + t * (a3 + t * (a4 + t * a5)));

    xsimd::batch<double> exp_term = xsimd::exp(-abs_x * abs_x);  // Exponential term
    xsimd::batch<double> erf_approx = 1.0 - (t * polynomial) * exp_term;

    // CDF approximation
    return 0.5 * (1.0 + sign * erf_approx);  // Final normal CDF approximation
}


// Standard normal probability density function
double normal_pdf(double x) {
    static const double inv_sqrt_2pi = 1.0 / std::sqrt(2.0 * M_PI);
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

