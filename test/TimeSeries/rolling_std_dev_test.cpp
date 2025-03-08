#include <cassert>
#include <cmath>
#include <iostream>
#include "finmath/TimeSeries/rolling_std_dev.h"

// Helper Function
bool almost_equal(double a, double b, double tolerance)
{
    return std::abs(a - b) <= tolerance * std::max(std::abs(a), std::abs(b));
}

int rolling_std_dev_tests()
{
     double expected = 0.0;
    double tolerance = 0.001;

    //Test 1: Test Basic Standard Deviation Calculation(no rolling calculation)
    {
        std::vector<double> prices = {12.3,15.4,12.7,17.8,12.8};
        std::vector<double> std_val_values = rolling_std_dev(prices.size(),prices);
        expected = 2.108;
        assert(almost_equal(std_val_values.back(), expected, tolerance));

    }

    //Test 2: Test Basic Window Size to minizize computation
    {
        std::vector<double> prices = {1,2,3,4,5,6};
        std::vector<double> std_val_values = rolling_std_dev(2,prices);
        expected = 0.5000;
        assert(almost_equal(std_val_values.back(), expected, tolerance));
    }

    //Test 3: Test Singular Window Size(should be all zeros)
    {
        std::vector<double> prices = {3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.10,2.2};
        std::vector<double> std_val_values = rolling_std_dev(1,prices);
        expected = 0.000;
        assert(almost_equal(std_val_values.back(), expected, tolerance));
    }

    // Test 4: Test Window Size Larger than Array (Should return single value std dev)
    {
        std::vector<double> prices = {5, 10, 15};
        std::vector<double> std_val_values = rolling_std_dev(5, prices);
        expected = 4.082; // Since entire array is used
        assert(almost_equal(std_val_values.back(), expected, tolerance));
    }

    // Test 6: Test Window Size of 3 (Sliding effect)
    {
        std::vector<double> prices = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::vector<double> std_val_values = rolling_std_dev(3, prices);
        expected = 0.816; // For last three numbers (8,9,10)
        assert(almost_equal(std_val_values.back(), expected, tolerance));
    }

    // Test 7: Test All Elements Same (Should be zero)
    {
        std::vector<double> prices = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
        std::vector<double> std_val_values = rolling_std_dev(3, prices);
        expected = 0.000;
        assert(almost_equal(std_val_values.back(), expected, tolerance));
    }

    // Test 8: Test Decreasing Series
    {
        std::vector<double> prices = {100, 90, 80, 70, 60, 50, 40, 30, 20, 10};
        std::vector<double> std_val_values = rolling_std_dev(4, prices);
        expected = 11.180; // Precomputed expected value
        assert(almost_equal(std_val_values.back(), expected, tolerance));
    }

    // Test 9: Test Small Values (Close to Zero)
   {
        std::vector<double> prices = {0.001, 0.002, 0.003, 0.004, 0.005};
        std::vector<double> std_val_values = rolling_std_dev(3, prices);
        double mean = (0.003 + 0.004 + 0.005) / 3.0;
        double variance = ((0.003 - mean) * (0.003 - mean) +
                       (0.004 - mean) * (0.004 - mean) +
                       (0.005 - mean) * (0.005 - mean)) / 3.0;
        expected = std::sqrt(variance);
        assert(almost_equal(std_val_values.back(), expected, tolerance));
    }

    // Test 10: Test Large Numbers
    {
        std::vector<double> prices = {1000000, 1000001, 1000002, 1000003, 1000004};
        std::vector<double> std_val_values = rolling_std_dev(3, prices);
        expected = 0.816;
        assert(almost_equal(std_val_values.back(), expected, tolerance));
    }
    std::cout << "Rolling Std. Deviation Tests Pass!" << std::endl;
    return 0;
}

int main()
{
    return rolling_std_dev_tests();
}
