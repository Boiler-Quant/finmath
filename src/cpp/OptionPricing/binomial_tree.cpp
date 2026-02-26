#include <cmath>
#include <algorithm>
#include <limits>
#include <vector>
#include "finmath/OptionPricing/binomial_tree.h"
#include "finmath/Helper/helper.h"

double binomial_option_pricing(OptionType type, double S0, double K, double T, double r, double sigma, long N) {
    if (S0 <= 0.0 || K <= 0.0 || T <= 0.0 || sigma < 0.0 || N <= 0) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    const double dt = T / static_cast<double>(N);
    const double u = std::exp(sigma * std::sqrt(dt));
    const double d = std::exp(-sigma * std::sqrt(dt));
    const double p = (std::exp(r * dt) - d) / (u - d);
    double value = 0.0;

    // Start with the initial binomial coefficient C(N, 0) = 1
    double binomial_coeff = 1.0;

    for (long i = 0; i <= N; ++i) {
        double node_prob = binomial_coeff * std::pow(p, i) * std::pow(1 - p, N - i);
        double ST = S0 * std::pow(u, i) * std::pow(d, N - i);

        if (type == OptionType::CALL) {
            value += std::max(ST - K, 0.0) * node_prob;
        } else if (type == OptionType::PUT) {
            value += std::max(K - ST, 0.0) * node_prob;
        }

        // Update binomial coefficient for next iteration: C(N, i+1)
        if (i < N) {
            binomial_coeff *= (N - i) / double(i + 1);
        }
    }

    return value * std::exp(-r * T);
}

namespace Binom {
    double compute_delta(OptionType type, double S0, double K, double T, double r, double sigma, long N, double delta_S) {
        if (delta_S < 0) {
            delta_S = 0.001 * S0;
        }
    
        double orig_option_price = binomial_option_pricing(type, S0, K, T, r, sigma, N);
        double new_option_price = binomial_option_pricing(type, S0 + delta_S, K, T, r, sigma, N);
        return (new_option_price - orig_option_price) / delta_S;
    }
    
    double compute_gamma(OptionType type, double S0, double K, double T, double r, double sigma, long N, double delta_S) {
        if (delta_S < 0) {
            delta_S = 0.001 * S0;
        }
        double price_up = binomial_option_pricing(type, S0 + delta_S, K, T, r, sigma, N);
        double price_base = binomial_option_pricing(type, S0, K, T, r, sigma, N);
        double price_down = binomial_option_pricing(type, S0 - delta_S, K, T, r, sigma, N);
        return (price_up - 2 * price_base + price_down) / (delta_S * delta_S);
    }
    
    double compute_vega(OptionType type, double S0, double K, double T, double r, double sigma, long N, double delta_sig) {
        if (delta_sig == -1) {
            delta_sig = 0.001 * sigma;
        }
        double price_up = binomial_option_pricing(type, S0, K, T, r, sigma + delta_sig, N);
        double price_base = binomial_option_pricing(type, S0, K, T, r, sigma, N);
        return 0.01 * (price_up - price_base) / delta_sig;
    }
    
    double compute_theta(OptionType type, double S0, double K, double T, double r, double sigma, long N, double delta_T) {
        if (delta_T == -1) {
            delta_T = 0.001 * T;
        }
    
        double price_up = binomial_option_pricing(type, S0, K, T + delta_T, r, sigma, N);
        double price_base = binomial_option_pricing(type, S0, K, T, r, sigma, N);
        return (price_up - price_base) / delta_T;
    }
    
    double compute_rho(OptionType type, double S0, double K, double T, double r, double sigma, long N, double delta_r) {
        if (delta_r == -1) {
            delta_r = 0.001 * r;
        }
    
        double price_up = binomial_option_pricing(type, S0, K, T, r + delta_r, sigma, N);
        double price_base = binomial_option_pricing(type, S0, K, T, r, sigma, N);
        return 0.01 * (price_up - price_base) / delta_r;
    }
}
