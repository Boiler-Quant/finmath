#include "finmath/TimeSeries/simple_moving_average.h"
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
    // Test Case 1: Basic SMA
    std::vector<double> data1 = {1, 2, 3, 4, 5, 6, 7};
    size_t window1 = 3;
    std::vector<double> expected1 = {2.0, 3.0, 4.0, 5.0, 6.0};
    std::vector<double> result1 = simple_moving_average(data1, window1);
    assert(result1.size() == expected1.size());
    for (size_t i = 0; i < result1.size(); ++i)
    {
        if (!approx_equal(result1[i], expected1[i]))
        {
            std::cerr << "SMA Test Case 1 Failed: Index " << i << " Expected: " << expected1[i] << " Got: " << result1[i] << std::endl;
            return 1;
        }
    }
    std::cout << "SMA Test Case 1 Passed." << std::endl;

    // Test Case 2: Window size equals data size
    std::vector<double> data2 = {10, 20, 30};
    size_t window2 = 3;
    std::vector<double> expected2 = {20.0};
    std::vector<double> result2 = simple_moving_average(data2, window2);
    assert(result2.size() == expected2.size());
    assert(approx_equal(result2[0], expected2[0]));
    std::cout << "SMA Test Case 2 Passed." << std::endl;

    // Test Case 3: Window size larger than data size (expects empty vector)
    std::vector<double> data3 = {1, 2};
    size_t window3 = 3;
    std::vector<double> result3 = simple_moving_average(data3, window3);
    assert(result3.empty());
    std::cout << "SMA Test Case 3 Passed." << std::endl;

    // Test Case 4: Empty data input (expects empty vector)
    std::vector<double> data4 = {};
    size_t window4 = 3;
    std::vector<double> result4 = simple_moving_average(data4, window4);
    assert(result4.empty());
    std::cout << "SMA Test Case 4 Passed." << std::endl;

    // Test Case 5: Exception - window size zero
    try
    {
        simple_moving_average(data1, 0);
        std::cerr << "SMA Test Case 5 Failed: Expected exception for window size zero." << std::endl;
        return 1;
    }
    catch (const std::runtime_error &e)
    {
        std::cout << "SMA Test Case 5 Passed (Caught expected exception)." << std::endl;
    }

    std::cout << "All simple_moving_average C++ tests passed." << std::endl;
    return 0;
}