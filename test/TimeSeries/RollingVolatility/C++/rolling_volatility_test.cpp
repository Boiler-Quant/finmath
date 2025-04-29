#include "finmath/TimeSeries/rolling_volatility.h"
#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>

// Helper to compare floating point numbers approximately
bool approx_equal(double a, double b, double epsilon = std::numeric_limits<double>::epsilon() * 100)
{
    return std::fabs(a - b) <= epsilon * std::max(1.0, std::max(std::fabs(a), std::fabs(b)));
}

int main()
{
    // Test Case 1: Basic calculation
    std::vector<double> prices1 = {100, 101, 102, 100, 99, 98, 100, 102, 103, 104, 105};
    size_t window1 = 5;
    // std::vector<double> expected1 = {0.189256337, 0.189256337, 0.189256337, 0.221880118, 0.221880118, 0.189256337}; // Old corrected values
    std::vector<double> expected1 = {0.189255946, 0.233483658, 0.265300727, 0.215894675, 0.174817515, 0.080444774}; // Recalculated values
    std::vector<double> result1 = rolling_volatility(prices1, window1);

    assert(result1.size() == expected1.size());
    for (size_t i = 0; i < result1.size(); ++i)
    {
        if (!approx_equal(result1[i], expected1[i], 1e-6)) // Use explicit tolerance
        {
            std::cerr << "Test Case 1 Failed: Index " << i << " Expected: " << expected1[i] << " Got: " << result1[i] << std::endl;
            return 1;
        }
    }
    std::cout << "Test Case 1 Passed." << std::endl;

    // Test Case 2: Edge case - window size equals log returns size
    std::vector<double> prices2 = {100, 101, 102, 103, 104, 105};
    size_t window2 = 5;
    std::vector<double> result2 = rolling_volatility(prices2, window2);
    assert(result2.size() == 1); // Should produce one volatility value
    std::cout << "Test Case 2 Passed." << std::endl;

    // Test Case 3: Exception - window size too large
    try
    {
        rolling_volatility(prices2, 6); // window > log_returns.size()
        std::cerr << "Test Case 3 Failed: Expected exception for window too large." << std::endl;
        return 1;
    }
    catch (const std::runtime_error &e)
    {
        std::cout << "Test Case 3 Passed (Caught expected exception)." << std::endl;
    }

    // Test Case 4: Exception - window size zero
    try
    {
        rolling_volatility(prices2, 0);
        std::cerr << "Test Case 4 Failed: Expected exception for window size zero." << std::endl;
        return 1;
    }
    catch (const std::runtime_error &e)
    {
        std::cout << "Test Case 4 Passed (Caught expected exception)." << std::endl;
    }

    // Test Case 5: Exception - insufficient data
    try
    {
        rolling_volatility({100.0}, 1);
        std::cerr << "Test Case 5 Failed: Expected exception for insufficient data." << std::endl;
        return 1;
    }
    catch (const std::runtime_error &e)
    {
        std::cout << "Test Case 5 Passed (Caught expected exception)." << std::endl;
    }

    std::cout << "All rolling_volatility C++ tests passed." << std::endl;
    return 0;
}