#ifndef BINOMIAL_TREE_H
#define BINOMIAL_TREE_H

#include "options_pricing_types.h"

double binomial_option_pricing(OptionType type, double S0, double K, double T, double r, double sigma, long N);

namespace Binom {
    double compute_delta(OptionType type, double S0, double K, double T, double r, double sigma, long N, double delta_S = -1);
    double compute_gamma(OptionType type, double S0, double K, double T, double r, double sigma, long N, double delta_S = -1);
    double compute_vega(OptionType type, double S0, double K, double T, double r, double sigma, long N, double delta_sig = -1);
    double compute_theta(OptionType type, double S0, double K, double T, double r, double sigma, long N, double delta_T = -1);
    double compute_rho(OptionType type, double S0, double K, double T, double r, double sigma, long N, double delta_r = -1);
}

#endif
