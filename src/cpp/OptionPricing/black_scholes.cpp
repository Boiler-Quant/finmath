#include <cmath>
#include <limits>
#include "finmath/OptionPricing/black_scholes.h"
#include "finmath/Helper/helper.h"

double black_scholes(OptionType type, double strike, double price, double time, double rate, double volatility) {
    if (strike <= 0.0 || price <= 0.0 || time <= 0.0 || volatility < 0.0) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    const double d1 = (std::log(price / strike) + ((rate + volatility * volatility / 2) * time)) / (volatility * std::sqrt(time));
    const double d2 = d1 - (volatility * std::sqrt(time));

    if (type == OptionType::CALL) {
        return price * normal_cdf(d1) - std::exp(-rate * time) * strike * normal_cdf(d2);
    } else {
        return strike * std::exp(-rate * time) * normal_cdf(-d2) - price * normal_cdf(-d1);
    }
}

namespace BlackScholes {

    // S0 = underlying price, K = strike, t = time to expiry (fraction of year)
    // r = risk-free rate, q = dividend yield, sigma = volatility
    // Reference: https://www.macroption.com/black-scholes-formula/

    double compute_delta(OptionType type, double S0, double K, double t, double r, double q, double sigma) {
        if (S0 <= 0.0 || K <= 0.0 || t <= 0.0 || sigma < 0.0) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        const double d1 = (std::log(S0 / K) + (r - q + 0.5 * sigma * sigma) * t) / (sigma * std::sqrt(t));
        return (type == OptionType::CALL) ? std::exp(-q * t) * normal_cdf(d1) : std::exp(-q * t) * (normal_cdf(d1) - 1);
    }

    double compute_gamma(double S0, double K, double t, double r, double q, double sigma) {
        if (S0 <= 0.0 || K <= 0.0 || t <= 0.0 || sigma < 0.0) { return std::numeric_limits<double>::quiet_NaN(); }
        const double d1 = (std::log(S0 / K) + (r - q + 0.5 * sigma * sigma) * t) / (sigma * std::sqrt(t));
        return (std::exp(-q * t) * normal_pdf(d1)) / (S0 * sigma * std::sqrt(t));
    }

    double compute_vega(double S0, double K, double t, double r, double q, double sigma) {
        if (S0 <= 0.0 || K <= 0.0 || t <= 0.0 || sigma < 0.0) { return std::numeric_limits<double>::quiet_NaN(); }
        const double d1 = (std::log(S0 / K) + (r - q + 0.5 * sigma * sigma) * t) / (sigma * std::sqrt(t));
        return 0.01 * S0 * std::exp(-q * t) * std::sqrt(t) * normal_pdf(d1);
    }

    double compute_theta(OptionType type, double S0, double K, double t, double T, double r, double q, double sigma) {
        if (S0 <= 0.0 || K <= 0.0 || t <= 0.0 || sigma < 0.0 || T <= 0.0) { return std::numeric_limits<double>::quiet_NaN(); }
        const double d1 = (std::log(S0 / K) + (r - q + 0.5 * sigma * sigma) * t) / (sigma * std::sqrt(t));
        const double d2 = d1 - sigma * std::sqrt(t);
        const double term1 = -1 * (S0 * sigma * std::exp(-q * t) * normal_pdf(d1)) / (2 * std::sqrt(t));
        const double term2 = r * K * std::exp(-r * t);
        const double term3 = q * S0 * std::exp(-q * t);
        return (type == OptionType::CALL) ? 1 / T * (term1 - term2 * normal_cdf(d2) + term3 * normal_cdf(d1)) : 1 / T * (term1 + term2 * normal_cdf(-d2) - term3 * normal_cdf(-d1));
    }

    double compute_rho(OptionType type, double S0, double K, double t, double r, double q, double sigma) {
        if (S0 <= 0.0 || K <= 0.0 || t <= 0.0 || sigma < 0.0) { return std::numeric_limits<double>::quiet_NaN(); }
        const double d1 = (std::log(S0 / K) + (r - q + 0.5 * sigma * sigma) * t) / (sigma * std::sqrt(t));
        const double d2 = d1 - sigma * std::sqrt(t);
        return (type == OptionType::CALL) ? 0.01 * K * t * std::exp(-r * t) * normal_cdf(d2) : -0.01 * K * t * std::exp(-r * t) * normal_cdf(-d2);
    }
}

