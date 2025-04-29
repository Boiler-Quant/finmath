#include <cassert>
#include <cmath>
#include <iostream>
#include "finmath/OptionPricing/options_pricing.h"

// Helper Function
bool almost_equal(double a, double b, double tolerance)
{
    return std::abs(a - b) <= tolerance * std::max(std::abs(a), std::abs(b));
}

int binomial_option_pricing_tests()
{
    double tolerance = 0.001;

    // Test Case 1: Basic European Call Option
    {
        double S0 = 100.0;  // Initial stock price
        double K = 100.0;   // Strike price
        double T = 1.0;     // Time to maturity (1 year)
        double r = 0.05;    // Risk-free rate
        double sigma = 0.2; // Volatility
        long N = 100;       // Number of time steps

        double call_price = binomial_option_pricing(OptionType::CALL, S0, K, T, r, sigma, N);
        std::cout << "Test Case 1 - Basic Call Option Price: " << call_price << std::endl;
        assert(call_price > 0.0 && call_price < S0);
    }

    // Test Case 2: Basic European Put Option
    {
        double S0 = 100.0;
        double K = 100.0;
        double T = 1.0;
        double r = 0.05;
        double sigma = 0.2;
        long N = 100;

        double put_price = binomial_option_pricing(OptionType::PUT, S0, K, T, r, sigma, N);
        std::cout << "Test Case 2 - Basic Put Option Price: " << put_price << std::endl;
        assert(put_price > 0.0 && put_price < K * std::exp(-r * T));
    }

    // Test Case 3: Deep In-The-Money Call
    {
        double S0 = 100.0;
        double K = 80.0;    // Deep ITM
        double T = 1.0;
        double r = 0.05;
        double sigma = 0.2;
        long N = 100;

        double call_price = binomial_option_pricing(OptionType::CALL, S0, K, T, r, sigma, N);
        std::cout << "Test Case 3 - Deep ITM Call Price: " << call_price << std::endl;
        assert(call_price > (S0 - K * std::exp(-r * T)));
    }

    // Test Case 4: Deep Out-of-The-Money Put
    {
        double S0 = 100.0;
        double K = 120.0;   // Deep OTM
        double T = 1.0;
        double r = 0.05;
        double sigma = 0.2;
        long N = 100;

        double put_price = binomial_option_pricing(OptionType::PUT, S0, K, T, r, sigma, N);
        std::cout << "Test Case 4 - Deep OTM Put Price: " << put_price << std::endl;
        assert(put_price > 0.0 && put_price < K * std::exp(-r * T));
    }

    // Test Case 5: Convergence with Different Time Steps
    {
        double S0 = 100.0;
        double K = 100.0;
        double T = 1.0;
        double r = 0.05;
        double sigma = 0.2;

        // Test with different number of time steps
        long N1 = 50;
        long N2 = 100;
        long N3 = 200;

        double price1 = binomial_option_pricing(OptionType::CALL, S0, K, T, r, sigma, N1);
        double price2 = binomial_option_pricing(OptionType::CALL, S0, K, T, r, sigma, N2);
        double price3 = binomial_option_pricing(OptionType::CALL, S0, K, T, r, sigma, N3);

        std::cout << "Test Case 5 - Convergence Test:" << std::endl;
        std::cout << "N=50: " << price1 << std::endl;
        std::cout << "N=100: " << price2 << std::endl;
        std::cout << "N=200: " << price3 << std::endl;

        // Prices should converge as N increases
        assert(std::abs(price2 - price1) > std::abs(price3 - price2));
    }

    // Test Case 6: Different Volatility Levels
    {
        double S0 = 100.0;
        double K = 100.0;
        double T = 1.0;
        double r = 0.05;
        long N = 100;

        // Test with different volatility levels
        double sigma1 = 0.1;  // Low volatility
        double sigma2 = 0.2;  // Medium volatility
        double sigma3 = 0.3;  // High volatility

        double price1 = binomial_option_pricing(OptionType::CALL, S0, K, T, r, sigma1, N);
        double price2 = binomial_option_pricing(OptionType::CALL, S0, K, T, r, sigma2, N);
        double price3 = binomial_option_pricing(OptionType::CALL, S0, K, T, r, sigma3, N);

        std::cout << "Test Case 6 - Volatility Test:" << std::endl;
        std::cout << "sigma=0.1: " << price1 << std::endl;
        std::cout << "sigma=0.2: " << price2 << std::endl;
        std::cout << "sigma=0.3: " << price3 << std::endl;

        // Higher volatility should lead to higher option prices
        assert(price1 < price2 && price2 < price3);
    }

    std::cout << "All Binomial-Tree Tests Passed!" << std::endl;
    return 0;
}

int main()
{
    return binomial_option_pricing_tests();
}
