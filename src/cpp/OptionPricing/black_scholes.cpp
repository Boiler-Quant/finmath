#include <cmath> 
#include "finmath/OptionPricing/black_scholes.h"
#include "finmath/Helper/helper.h"

double black_scholes(OptionType type, double strike, double price, double time, double rate, double volatility){
    double d1 = (std::log(price / strike) + ((rate + volatility*volatility/2) * time)) / (volatility * std::sqrt(time));
    double d2 = d1 - (volatility * std::sqrt(time));

    if (type == OptionType::CALL) {
        return price * normal_cdf(d1) - std::exp(-rate * time) * strike * normal_cdf(d2);
    } else {
        return strike * std::exp(-rate * time) * normal_cdf(-d2) - price * normal_cdf(-d1);
    }
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

