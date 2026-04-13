#include "finmath/MonteCarlo/monte_carlo.h"

#include <cmath>
#include <limits>
#include <random>

namespace {

constexpr double kTwoPi = 6.283185307179586476925286766559;

// Box-Muller for standard normal
double normal_rand(std::mt19937_64& gen)
{
    std::uniform_real_distribution<double> u(0.0, 1.0);
    double x = u(gen);
    double y = u(gen);
    return std::sqrt(-2.0 * std::log(x)) * std::cos(kTwoPi * y);
}

}  // namespace

double monte_carlo_european(OptionType type, double S0, double K, double T, double r, double q, double sigma,
                            size_t n_paths, uint64_t seed)
{
    if (S0 <= 0.0 || K <= 0.0 || T <= 0.0 || sigma < 0.0 || n_paths == 0) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    std::mt19937_64 gen(seed);
    const double drift = (r - q - 0.5 * sigma * sigma) * T;
    const double vol = sigma * std::sqrt(T);
    const double disc = std::exp(-r * T);

    double sum = 0.0;
    for (size_t i = 0; i < n_paths; ++i) {
        const double z = normal_rand(gen);
        const double ST = S0 * std::exp(drift + vol * z);
        double payoff = 0.0;
        if (type == OptionType::CALL) {
            payoff = std::max(ST - K, 0.0);
        } else {
            payoff = std::max(K - ST, 0.0);
        }
        sum += disc * payoff;
    }

    return sum / static_cast<double>(n_paths);
}
