#ifndef FINMATH_MONTE_CARLO_MONTE_CARLO_H
#define FINMATH_MONTE_CARLO_MONTE_CARLO_H

#include <cstddef>
#include <cstdint>

#include "finmath/OptionPricing/options_pricing_types.h"

// European option under GBM; antithetic pairs per path count. `n_paths` is number of simulated paths.
double monte_carlo_european(OptionType type, double S0, double K, double T, double r, double q, double sigma,
                            size_t n_paths, uint64_t seed);

#endif  // FINMATH_MONTE_CARLO_MONTE_CARLO_H
