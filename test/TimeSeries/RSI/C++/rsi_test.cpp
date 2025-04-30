#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include "finmath/TimeSeries/rsi.h"

// Helper Function
bool almost_equal(double a, double b, double tolerance)
{
    return std::abs(a - b) <= tolerance * std::max(std::abs(a), std::abs(b));
}

int rsi_tests()
{
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
        expected = 66.23;
        assert(almost_equal(rsi_values.back(), expected, tolerance));
    }

    // Test 6: Constant rise then drop (RSI should adjust accordingly)
    {
        std::vector<double> prices = {1, 2, 3, 4, 5, 6, 7, 8, 7, 6, 5, 4, 3, 2};
        std::vector<double> rsi_values = compute_smoothed_rsi(prices, 14);
        expected = 50.0;
        assert(almost_equal(rsi_values.back(), expected, tolerance));
    }

    // Test 7: Short price list (should handle or reject properly)
    {
        std::vector<double> prices = {100, 102};
        std::vector<double> rsi_values = compute_smoothed_rsi(prices, 14);
        assert(rsi_values.empty());
    }

    // Test 8: Large price movements (RSI should handle well)
    {
        std::vector<double> prices = {1, 1000, 1001, 500, 2000, 3000, 1500, 3500, 3000, 2500, 2000};
        std::vector<double> rsi_values = compute_smoothed_rsi(prices, 14);
        expected = 80.0;
        assert(almost_equal(rsi_values.back(), expected, tolerance));
    }

    std::cout << "RSI Tests Passed!" << std::endl;
    return 0;
}

int main()
{
    return rsi_tests();
}
