#include <cassert>
#include <cmath>
#include <iostream>
#include "finmath/OptionPricing/options_pricing.h"
#include "finmath/OptionPricing/binomial_tree.h"
#include "finmath/OptionPricing/black_scholes.h"

// Helper: relative tolerance comparison
static bool almost_equal(double a, double b, double rel_tol)
{
    double scale = std::max(std::abs(a), std::abs(b));
    if (scale < 1e-12) return std::abs(a - b) <= rel_tol;
    return std::abs(a - b) <= rel_tol * scale;
}

// Helper: absolute tolerance (for values near zero)
static bool almost_equal_abs(double a, double b, double abs_tol = 1e-6)
{
    return std::abs(a - b) <= abs_tol;
}

int binomial_option_pricing_tests()
{
    const double rel_tol = 0.02;   // 2% for binomial vs BS convergence
    const double abs_tol = 1e-4;

    // ---- Price level and bounds ----
    // Test 1: Call - price in (0, S0), positive
    {
        double S0 = 100.0, K = 100.0, T = 1.0, r = 0.05, sigma = 0.2;
        double c = binomial_option_pricing(OptionType::CALL, S0, K, T, r, sigma, 100);
        assert(c > 0 && c < S0);
    }

    // Test 2: Put - price in (0, K*exp(-r*T)), positive
    {
        double S0 = 100.0, K = 100.0, T = 1.0, r = 0.05, sigma = 0.2;
        double p = binomial_option_pricing(OptionType::PUT, S0, K, T, r, sigma, 100);
        double max_put = K * std::exp(-r * T);
        assert(p > 0 && p <= max_put + 1e-6);
    }

    // Test 3: Deep ITM call (S0 >> K) approximates S0 - K*exp(-r*T)
    {
        double S0 = 100.0, K = 50.0, T = 1.0, r = 0.05, sigma = 0.2;
        double c = binomial_option_pricing(OptionType::CALL, S0, K, T, r, sigma, 200);
        double intrinsic = S0 - K * std::exp(-r * T);
        assert(c >= intrinsic - 1e-6 && c <= S0 + 1e-6);
    }

    // Test 4: Deep OTM call (S0 << K) is small positive
    {
        double S0 = 100.0, K = 200.0, T = 1.0, r = 0.05, sigma = 0.2;
        double c = binomial_option_pricing(OptionType::CALL, S0, K, T, r, sigma, 100);
        assert(c >= 0 && c < 5.0);
    }

    // Test 5: One-step tree (N=1) - call and put have closed form
    {
        double S0 = 100.0, K = 100.0, T = 1.0, r = 0.05, sigma = 0.3;
        double dt = T, u = std::exp(sigma * std::sqrt(dt)), d = 1.0 / u;
        double p = (std::exp(r * dt) - d) / (u - d);
        double c1 = std::exp(-r * T) * (p * std::max(S0 * u - K, 0.0) + (1 - p) * std::max(S0 * d - K, 0.0));
        double c = binomial_option_pricing(OptionType::CALL, S0, K, T, r, sigma, 1);
        assert(almost_equal(c, c1, 1e-10));
        double p1 = std::exp(-r * T) * (p * std::max(K - S0 * u, 0.0) + (1 - p) * std::max(K - S0 * d, 0.0));
        double put_val = binomial_option_pricing(OptionType::PUT, S0, K, T, r, sigma, 1);
        assert(almost_equal(put_val, p1, 1e-10));
    }

    // ---- Put-call parity: C - P = S0 - K*exp(-r*T) ----
    // Test 6: Put-call parity (same params)
    {
        double S0 = 100.0, K = 102.0, T = 1.5, r = 0.04, sigma = 0.25;
        long N = 300;
        double c = binomial_option_pricing(OptionType::CALL, S0, K, T, r, sigma, N);
        double p = binomial_option_pricing(OptionType::PUT, S0, K, T, r, sigma, N);
        double parity = S0 - K * std::exp(-r * T);
        assert(almost_equal(c - p, parity, rel_tol));
    }

    // ---- Convergence to Black-Scholes ----
    // Test 7: Call, out-of-the-money
    {
        double S0 = 100.0, K = 105.0, T = 1.0, r = 0.05, sigma = 0.2;
        double bs = black_scholes(OptionType::CALL, S0, K, T, r, sigma);
        double bin = binomial_option_pricing(OptionType::CALL, S0, K, T, r, sigma, 500);
        assert(almost_equal(bs, bin, rel_tol));
    }

    // Test 8: Put, in-the-money
    {
        double S0 = 100.0, K = 95.0, T = 1.0, r = 0.05, sigma = 0.2;
        double bs = black_scholes(OptionType::PUT, S0, K, T, r, sigma);
        double bin = binomial_option_pricing(OptionType::PUT, S0, K, T, r, sigma, 500);
        assert(almost_equal(bs, bin, rel_tol));
    }

    // Test 9: At-the-money call
    {
        double S0 = 100.0, K = 100.0, T = 1.0, r = 0.05, sigma = 0.2;
        double bs = black_scholes(OptionType::CALL, S0, K, T, r, sigma);
        double bin = binomial_option_pricing(OptionType::CALL, S0, K, T, r, sigma, 400);
        assert(almost_equal(bs, bin, rel_tol));
    }

    // Test 10: Different maturity and vol
    {
        double S0 = 50.0, K = 52.0, T = 0.5, r = 0.03, sigma = 0.35;
        double bs = black_scholes(OptionType::CALL, S0, K, T, r, sigma);
        double bin = binomial_option_pricing(OptionType::CALL, S0, K, T, r, sigma, 400);
        assert(almost_equal(bs, bin, rel_tol));
    }

    // ---- Greeks (sign and approximate magnitude) ----
    const long N_greek = 200;
    double S0 = 100.0, K = 100.0, T = 1.0, r = 0.05, sigma = 0.2;

    // Test 11: Call delta in (0, 1)
    {
        double delta = Binom::compute_delta(OptionType::CALL, S0, K, T, r, sigma, N_greek);
        assert(delta > 0 && delta < 1.0);
    }

    // Test 12: Put delta in (-1, 0)
    {
        double delta = Binom::compute_delta(OptionType::PUT, S0, K, T, r, sigma, N_greek);
        assert(delta > -1.0 && delta < 0);
    }

    // Test 13: Gamma positive (for both call and put)
    {
        double gamma_c = Binom::compute_gamma(OptionType::CALL, S0, K, T, r, sigma, N_greek);
        double gamma_p = Binom::compute_gamma(OptionType::PUT, S0, K, T, r, sigma, N_greek);
        assert(gamma_c > 0 && gamma_p > 0);
        assert(almost_equal(gamma_c, gamma_p, 0.05));  // call and put gamma same
    }

    // Test 14: Vega positive (higher vol -> higher option value)
    {
        double vega = Binom::compute_vega(OptionType::CALL, S0, K, T, r, sigma, N_greek);
        assert(vega > 0);
    }

    // Test 15: Rho for call positive (higher r -> higher call value typically)
    {
        double rho = Binom::compute_rho(OptionType::CALL, S0, K, T, r, sigma, N_greek);
        assert(rho > 0);
    }

    // Test 16: Delta matches finite-difference of price (spot bump)
    {
        double dS = 0.01 * S0;
        double p0 = binomial_option_pricing(OptionType::CALL, S0, K, T, r, sigma, N_greek);
        double p_up = binomial_option_pricing(OptionType::CALL, S0 + dS, K, T, r, sigma, N_greek);
        double delta_fd = (p_up - p0) / dS;
        double delta = Binom::compute_delta(OptionType::CALL, S0, K, T, r, sigma, N_greek);
        assert(almost_equal(delta, delta_fd, 0.05));
    }

    std::cout << "Binomial-Tree Tests Passed!" << std::endl;
    return 0;
}

int main()
{
    return binomial_option_pricing_tests();
}
