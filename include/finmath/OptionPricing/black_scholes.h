#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H

#include "options_pricing_types.h"
#include <vector>
#include <xsimd/xsimd.hpp> 


double black_scholes(OptionType type, double strike, double price, double time, double rate, double volatility);

template <typename T>
using AlignedVector = std::vector<T, xsimd::aligned_allocator<T>>;

AlignedVector<double> black_scholes_multiple_calls(
    const AlignedVector<double>& strikes,
    const AlignedVector<double>& prices,
    const AlignedVector<double>& times,
    const AlignedVector<double>& rates,
    const AlignedVector<double>& volatilities);

namespace BlackScholes {
    double compute_delta(OptionType type, double S0, double K, double t, double r, double q, double sigma);
    double compute_gamma(double S0, double K, double t, double r, double q, double sigma);
    double compute_vega(double S0, double K, double t, double r, double q, double sigma);
    double compute_theta(OptionType type, double S0, double K, double t, double T, double r, double q, double sigma);
    double compute_rho(OptionType type, double S0, double K, double t, double r, double q, double sigma);
}

#endif //BLACK_SCHOLES_H
