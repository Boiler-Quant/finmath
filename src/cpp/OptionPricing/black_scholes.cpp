#include <cmath> 
#include "finmath/OptionPricing/black_scholes.h"
#include "finmath/Helper/helper.h"
#include <vector>
#include <xsimd/xsimd.hpp>
#include <omp.h>


double black_scholes(OptionType type, double strike, double price, double time, double rate, double volatility) {
    double sqrt_time = std::sqrt(time);
    double vol_sqrt_time = volatility * sqrt_time;
    double rate_time = rate * time;

    double d1 = (std::log(price / strike) + ((rate + volatility * volatility/2) * time)) / volatility_sqrt_time;
    double d2 = d1 - volatility_sqrt_time;

    double disc_factor = std::exp(-rate_time);
    double cdf_d1 = normal_cdf_approx(d1);
    double cdf_d2 = normal_cdf_approx(d2);


    if (type == OptionType::CALL) {
        return price * cdf_d1 - disc_factor * strike * cdf_d2;
    } else {
        return strike * disc_factor * (1 - cdf_d2) - price * (1 - cdf_d1);
    }
}

// Black-Scholes options pricing with simd
inline xsimd::batch<double> black_scholes_simd(
    xsimd::batch<double> price, xsimd::batch<double> strike, xsimd::batch<double> time,
    xsimd::batch<double> rate, xsimd::batch<double> volatility, xsimd::batch<double>& call_result, xsimd::batch<double>& put_result) {

    xsimd::batch<double> sqrt_time = xsimd::sqrt(time);
    xsimd::batch<double> vol_sqrt_time = volatility * sqrt_time;
    xsimd::batch<double> rate_time = rate * time;

    xsimd::batch<double> d1 = (xsimd::log(price / strike) +
                               (rate + xsimd::batch<double>(0.5) * volatility * volatility) * time)
                              / vol_sqrt_time;
    xsimd::batch<double> d2 = d1 - vol_sqrt_time;

    xsimd::batch<double> disc_factor = xsimd::exp(-rate_time);
    xsimd::batch<double> cdf_d1 = normal_cdf_approx(d1);
    xsimd::batch<double> cdf_d2 = normal_cdf_approx(d2);

    call_result = price * cdf_d1 - disc_factor * strike * cdf_d2;
    put_result = strike * disc_factor * (1 - cdf_d2) - price * (1 - cdf_d1);
}

// Ensure memory is aligned when using simd
template <typename T>
using AlignedVector = std::vector<T, xsimd::aligned_allocator<T>>;


// Efficient method to price multiple options (uses omp for multi threading, and simd for parallel computation in a thread)
AlignedVector<double> black_scholes_multiple(
    const std::vector<OptionType>& types,
    const AlignedVector<double>& strikes,
    const AlignedVector<double>& prices,
    const AlignedVector<double>& times,
    const AlignedVector<double>& rates,
    const AlignedVector<double>& volatilities) {

    size_t num_options = types.size();
    AlignedVector<double> results(num_options);

    // SIMD batch size (depends on cpu architecture)
    constexpr size_t simd_width = xsimd::batch<double>::size;

    #pragma omp parallel for
    for (size_t i = 0; i + simd_width - 1 < num_options; i += simd_width) {
        xsimd::batch<double> price_batch(&prices[i]);
        xsimd::batch<double> strike_batch(&strikes[i]);
        xsimd::batch<double> time_batch(&times[i]);
        xsimd::batch<double> rate_batch(&rates[i]);
        xsimd::batch<double> volatility_batch(&volatilities[i]);

        xsimd::batch<double> call_result, put_result;
        black_scholes_simd(price_batch, strike_batch, time_batch, rate_batch, volatility_batch, call_result, put_result);

        for (size_t j = 0; j < simd_width; ++j) {
            results[i + j] = (types[i + j] == OptionType::CALL) ? call_result.get(j) : put_result.get(j);
        }
    }

    // note that num_options & ~(simd_width - 1) rounds down num_options to largest multiple of simd_width without exceeding num_options
    for (size_t i = num_options & ~(simd_width - 1); i < num_options; ++i) {
        results[i] = black_scholes(types[i], strikes[i], prices[i], times[i], rates[i], volatilities[i]);
    }

    return results;
}


namespace BlackScholes {
    
    // S0 is underlying price, K = strike price, t = time until expiration (percent of year)
    // r = continuously compounded risk-free interest rate, q = continuously compounded dividend rate, T = unit of time (can be calendar days, trading days, etc.)
    // referred to: https://www.macroption.com/black-scholes-formula/

    double compute_delta(OptionType type, double S0, double K, double t, double r, double q, double sigma) {
        double d1 = (std::log(S0 / K) + (r - q + 0.5 * sigma * sigma) * t) / (sigma * std::sqrt(t));
        return (type == OptionType::CALL) ? std::exp(-q * t) * normal_cdf(d1) : std::exp(-q * t) * (normal_cdf(d1) - 1);
    }

    double compute_gamma(double S0, double K, double t, double r, double q, double sigma) {
        double d1 = (std::log(S0 / K) + (r - q + 0.5 * sigma * sigma) * t) / (sigma * std::sqrt(t));
        return (std::exp(-q * t) * normal_pdf(d1)) / (S0 * sigma * std::sqrt(t));
    }

    double compute_vega(double S0, double K, double t, double r, double q, double sigma) {
        double d1 = (std::log(S0 / K) + (r - q + 0.5 * sigma * sigma) * t) / (sigma * std::sqrt(t));
        return 0.01 * S0 * std::exp(-q * t) * std::sqrt(t) * normal_pdf(d1);
    }

    double compute_theta(OptionType type, double S0, double K, double t, double T, double r, double q, double sigma) {
        double d1 = (std::log(S0 / K) + (r - q + 0.5 * sigma * sigma) * t) / (sigma * std::sqrt(t));
        double d2 = d1 - sigma * std::sqrt(t);
        double term1 = -1 * (S0 * sigma * std::exp(-q * t) * normal_pdf(d1)) / (2 * std::sqrt(t));
        double term2 = r * K * std::exp(-r * t);
        double term3 = q * S0 * std::exp(-q * t);
        return (type == OptionType::CALL) ? 1 / T * (term1 - term2 * normal_cdf(d2) + term3 * normal_cdf(d1)) : 1 / T * (term1 + term2 * normal_cdf(-d2) - term3 * normal_cdf(-d1));
    }

    double compute_rho(OptionType type, double S0, double K, double t, double r, double q, double sigma) {
        double d1 = (std::log(S0 / K) + (r - q + 0.5 * sigma * sigma) * t) / (sigma * std::sqrt(t));
        double d2 = d1 - sigma * std::sqrt(t);
        return (type == OptionType::CALL) ? 0.01 * K * t * std::exp(-r * t) * normal_cdf(d2) : -0.01 * K * t * std::exp(-r * t) * normal_cdf(-d2);
    }
}

