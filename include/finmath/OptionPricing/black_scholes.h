#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H

#include "options_pricing_types.h"

// Spot/strike convention: `spot` is underlying, `strike` is K. `dividend_yield` is continuous q (default 0).
double black_scholes(OptionType type, double strike, double spot, double time, double rate, double volatility,
                     double dividend_yield = 0.0);

namespace BlackScholes {
    double compute_delta(OptionType type, double S0, double K, double t, double r, double q, double sigma);
    double compute_gamma(double S0, double K, double t, double r, double q, double sigma);
    double compute_vega(double S0, double K, double t, double r, double q, double sigma);
    double compute_theta(OptionType type, double S0, double K, double t, double T, double r, double q, double sigma);
    double compute_rho(OptionType type, double S0, double K, double t, double r, double q, double sigma);
}

#endif  // BLACK_SCHOLES_H
