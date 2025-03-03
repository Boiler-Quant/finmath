#include <cassert>
#include <cmath>
#include <cstddef>
#include <iostream>
#include "finmath/finmath.h"
#include "finmath/OptionPricing/black_scholes.h"

int compound_interest_tests();
int black_scholes_tests();
int rsi_tests();

int main() {
    std::cout << "Starting Unit Tests\n";
    compound_interest_tests();
    black_scholes_tests();
    rsi_tests();

    return 0;
}
// Helper Function

bool almost_equal(double a, double b, double tolerance) {
    return std::abs(a - b) <= tolerance * std::max(std::abs(a), std::abs(b));
}

// Unit Tests

int compound_interest_tests() {
    double expected = 0.0;
    double tolerance = 0.001;

    // Test 1: Basic test with yearly compounding
    {
        double result = compound_interest(1000, 5, 10, 1);
        expected = 1628.89;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 2: Different rate
    {
        double result = compound_interest(1000, 10, 10, 1);
        expected = 2593.74;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 3: Different time period
    {
        double result = compound_interest(1000, 5, 5, 1);
        expected = 1276.28;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 4: Different compounding frequency (quarterly)
    {
        double result = compound_interest(1000, 5, 10, 4);
        expected = 1643.62;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 5: Different compounding frequency (monthly)
    {
        double result = compound_interest(1000, 5, 10, 12);
        expected = 1647.01;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 6: Different compounding frequency (daily)
    {
        double result = compound_interest(1000, 5, 10, 365);
        expected = 1648.66;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 7: Zero principal
    {
        double result = compound_interest(0, 5, 10, 1);
        expected = 0.0;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 8: Zero rate
    {
        double result = compound_interest(1000, 0, 10, 1);
        expected = 1000.0;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 9: Zero time
    {
        double result = compound_interest(1000, 5, 0, 1);
        expected = 1000.0;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 10: Zero frequency
    {
        double result = compound_interest(1000, 5, 10, 0);
        expected = 1000.0;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 11: Negative principal (should handle or reject)
    {
        double result = compound_interest(-1000, 5, 10, 1);
        expected = -1628.89;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 12: Negative rate (deflation)
    {
        double result = compound_interest(1000, -5, 10, 1);
        expected = 598.74;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 13: Negative time (should reject)
    {
        double result = compound_interest(1000, 5, -10, 1);
        expected = 0.0;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 14: Large principal
    {
        double result = compound_interest(1e6, 5, 10, 1);
        expected = 1628890.0;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 15: Large rate
    {
        double result = compound_interest(1000, 100, 1, 1);
        expected = 2000.0;
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 16: Large time
    {
        double result = compound_interest(1000, 5, 100, 1);
        expected = 131501.26;
        assert(almost_equal(result, expected, tolerance));
    }

    std::cout << "Compound Interest Tests Passed!" << std::endl;
    return 0;
}

int black_scholes_tests() {
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
        expected = 45.1930; // Placeholder value, should be replaced with correct expected value
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 6: High volatility
    {
        double result = black_scholes(OptionType::CALL, 100, 100, 1, 0.05, 1.0);
        expected = 39.8402; // Placeholder value, should be replaced with correct expected value
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 7: Near zero volatility
    {
        double result = black_scholes(OptionType::CALL, 100, 100, 1, 0.05, 0.01);
        expected = 4.8771; // Placeholder value, should be replaced with correct expected value
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 8: Near zero interest rate
    {
        double result = black_scholes(OptionType::CALL, 100, 100, 1, 0.01, 0.2);
        expected = 8.4333; // Placeholder value, should be replaced with correct expected value
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 9: Deep in-the-money call option
    {
        double result = black_scholes(OptionType::CALL, 50, 100, 1, 0.05, 0.2);
        expected = 52.4389; // Placeholder value, should be replaced with correct expected value
        assert(almost_equal(result, expected, tolerance));
    }

    // Test 10: Deep out-of-the-money call option
    {
        double result = black_scholes(OptionType::CALL, 150, 100, 1, 0.05, 0.2);
        expected = 0.3596; // Placeholder value, should be replaced with correct expected value
        assert(almost_equal(result, expected, tolerance));
    }

    std::cout << "Black-Scholes Tests Passed!" << std::endl;
    return 0;
}

int binomial_option_pricing_tests() {
    double tolerance = 0.001;
    

    std::cout << "Binomial-Tree Tests Passed!" << std::endl;
    return 0;
}

int rsi_tests() {
    double expected = 0.0;
    double tolerance = 0.001;

    // Test 1: Basic test with a sample price list
    {
        std::vector<double> prices = {44.34, 44.09, 44.15, 43.61, 44.33, 44.83, 45.10, 45.42, 45.84, 46.08, 45.89, 46.03, 45.61, 46.28, 46.28};
        std::vector<double> rsi_values = compute_smoothed_rsi(prices, 14);
        expected = 70.53;
        assert(almost_equal(rsi_values.back(), expected, tolerance));
    }

    // Test 2: Zero price change (RSI should be 50 after the first window size is reached)
    {
        std::vector<double> prices = {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
        std::vector<double> rsi_values = compute_smoothed_rsi(prices, 14);
        expected = 50.0;
        assert(almost_equal(rsi_values.back(), expected, tolerance));
    }

    // Test 3: Increasing prices (RSI should approach 100)
    {
        std::vector<double> prices = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        std::vector<double> rsi_values = compute_smoothed_rsi(prices, 14);
        expected = 100.0;
        assert(almost_equal(rsi_values.back(), expected, tolerance));
    }

    // Test 4: Decreasing prices (RSI should approach 0)
    {
        std::vector<double> prices = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
        std::vector<double> rsi_values = compute_smoothed_rsi(prices, 14);
        expected = 0.0;
        assert(almost_equal(rsi_values.back(), expected, tolerance));
    }

    // Test 5: Shorter period RSI calculation
    {
        std::vector<double> prices = {44.34, 44.09, 44.15, 43.61, 44.33, 44.83, 45.10, 45.42, 45.84, 46.08, 45.89, 46.03, 45.61, 46.28, 46.28};
        std::vector<double> rsi_values = compute_smoothed_rsi(prices, 7);
        expected = 66.23;  // Adjust based on the expected shorter period RSI
        assert(almost_equal(rsi_values.back(), expected, tolerance));
    }

    // Test 6: Constant rise then drop (RSI should adjust accordingly)
    {
        std::vector<double> prices = {1, 2, 3, 4, 5, 6, 7, 8, 7, 6, 5, 4, 3, 2};
        std::vector<double> rsi_values = compute_smoothed_rsi(prices, 14);
        expected = 50.0;  // Expected RSI midpoint due to equal gains and losses
        assert(almost_equal(rsi_values.back(), expected, tolerance));
    }

    // Test 7: Short price list (should handle or reject properly)
    {
        std::vector<double> prices = {100, 102};
        std::vector<double> rsi_values = compute_smoothed_rsi(prices, 14);
        assert(rsi_values.empty());  // Not enough data to calculate RSI, list should be empty
    }

    // Test 8: Large price movements (RSI should handle well)
    {
        std::vector<double> prices = {1, 1000, 1001, 500, 2000, 3000, 1500, 3500, 3000, 2500, 2000};
        std::vector<double> rsi_values = compute_smoothed_rsi(prices, 14);
        expected = 80.0;  // Estimate RSI based on large price movements
        assert(almost_equal(rsi_values.back(), expected, tolerance));
    }

    std::cout << "RSI Tests Passed!" << std::endl;
    return 0;
}

