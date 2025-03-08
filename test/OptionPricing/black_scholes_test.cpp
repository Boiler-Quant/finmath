#include <cassert>
#include <cmath>
#include <iostream>
#include "finmath/OptionPricing/black_scholes.h"

// Helper Function
bool almost_equal(double a, double b, double tolerance)
{
    return std::abs(a - b) <= tolerance * std::max(std::abs(a), std::abs(b));
}

int black_scholes_tests()
{
    double expected = 0.0;
    double tolerance = 0.001;

    // Test 1: Call option, basic parameters
    {
        double result = black_scholes(OptionType::CALL, 100, 105, 1, 0.05, 0.2);
        expected = 13.8579;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 2: Put option, basic parameters
    {
        double result = black_scholes(OptionType::PUT, 100, 95, 1, 0.05, 0.2);
        expected = 7.6338;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 3: At-the-money call option
    {
        double result = black_scholes(OptionType::CALL, 100, 100, 1, 0.05, 0.2);
        expected = 10.4506;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 4: At-the-money put option
    {
        double result = black_scholes(OptionType::PUT, 100, 100, 1, 0.05, 0.2);
        expected = 5.5735;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 5: Long time to maturity
    {
        double result = black_scholes(OptionType::CALL, 100, 100, 10, 0.05, 0.2);
        expected = 45.1930;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 6: High volatility
    {
        double result = black_scholes(OptionType::CALL, 100, 100, 1, 0.05, 1.0);
        expected = 39.8402;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 7: Near zero volatility
    {
        double result = black_scholes(OptionType::CALL, 100, 100, 1, 0.05, 0.01);
        expected = 4.8771;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 8: Near zero interest rate
    {
        double result = black_scholes(OptionType::CALL, 100, 100, 1, 0.01, 0.2);
        expected = 8.4333;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 9: Deep in-the-money call option
    {
        double result = black_scholes(OptionType::CALL, 50, 100, 1, 0.05, 0.2);
        expected = 52.4389;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 10: Deep out-of-the-money call option
    {
        double result = black_scholes(OptionType::CALL, 150, 100, 1, 0.05, 0.2);
        expected = 0.3596;
        assert(almost_equal(result, expected, tolerance));
    }

    std::cout << "Black-Scholes Tests Passed!" << std::endl;
    return 0;
}

int main()
{
    return black_scholes_tests();
}
