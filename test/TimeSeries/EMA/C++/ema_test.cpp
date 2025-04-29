#include "finmath/TimeSeries/ema.h"
#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept> // Required for std::runtime_error

// Helper from rolling_volatility_test.cpp
bool approx_equal(double a, double b, double epsilon = std::numeric_limits<double>::epsilon() * 100)
{
    return std::fabs(a - b) <= epsilon * std::max(1.0, std::max(std::fabs(a), std::fabs(b)));
}

int main()
{
    std::vector<double> prices = {10, 11, 12, 11, 10, 11, 12, 13};

    // --- Test compute_ema_with_smoothing ---
    double smoothing1 = 0.5;
    std::vector<double> expected_s1 = {10.0, 10.5, 11.25, 11.125, 10.5625, 10.78125, 11.390625, 12.1953125};
    std::vector<double> result_s1 = compute_ema_with_smoothing(prices, smoothing1);
    assert(result_s1.size() == expected_s1.size());
    for (size_t i = 0; i < result_s1.size(); ++i)
    {
        if (!approx_equal(result_s1[i], expected_s1[i], 1e-7))
        { // Tighter tolerance for EMA
            std::cerr << "EMA Smooth Test 1 Failed: Index " << i << " Expected: " << expected_s1[i] << " Got: " << result_s1[i] << std::endl;
            return 1;
        }
    }
    std::cout << "EMA Smooth Test 1 Passed." << std::endl;

    // --- Test compute_ema (window) ---
    size_t window2 = 3;                            // Corresponds to smoothing = 2 / (3 + 1) = 0.5
    std::vector<double> expected_w2 = expected_s1; // Should be same as above
    std::vector<double> result_w2 = compute_ema(prices, window2);
    assert(result_w2.size() == expected_w2.size());
    for (size_t i = 0; i < result_w2.size(); ++i)
    {
        if (!approx_equal(result_w2[i], expected_w2[i], 1e-7))
        {
            std::cerr << "EMA Window Test 2 Failed: Index " << i << " Expected: " << expected_w2[i] << " Got: " << result_w2[i] << std::endl;
            return 1;
        }
    }
    std::cout << "EMA Window Test 2 Passed." << std::endl;

    // --- Test Edge Cases ---
    std::vector<double> empty_prices = {};
    assert(compute_ema(empty_prices, 3).empty());
    assert(compute_ema_with_smoothing(empty_prices, 0.5).empty());
    std::cout << "EMA Edge Case (Empty Input) Passed." << std::endl;

    try
    {
        compute_ema(prices, 0); // Window 0
        std::cerr << "EMA Edge Case (Window 0) Failed: Expected exception." << std::endl;
        return 1;
    }
    catch (const std::runtime_error &)
    {
        std::cout << "EMA Edge Case (Window 0) Passed." << std::endl;
    }

    try
    {
        compute_ema_with_smoothing(prices, 0); // Smoothing 0
        std::cerr << "EMA Edge Case (Smoothing 0) Failed: Expected exception." << std::endl;
        return 1;
    }
    catch (const std::runtime_error &)
    {
        std::cout << "EMA Edge Case (Smoothing 0) Passed." << std::endl;
    }

    try
    {
        compute_ema_with_smoothing(prices, 1.0); // Smoothing 1
        std::cerr << "EMA Edge Case (Smoothing 1) Failed: Expected exception." << std::endl;
        return 1;
    }
    catch (const std::runtime_error &)
    {
        std::cout << "EMA Edge Case (Smoothing 1) Passed." << std::endl;
    }

    std::cout << "All ema C++ tests passed." << std::endl;
    return 0;
}