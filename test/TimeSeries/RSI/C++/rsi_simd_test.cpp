#include "finmath/TimeSeries/rsi_simd.h"
#include "finmath/TimeSeries/rsi.h"
#include <pybind11/numpy.h>
#include <pybind11/embed.h>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <limits>

namespace py = pybind11;

// Helper to compare floating point numbers
bool approx_equal(double a, double b, double epsilon = 1e-6) {
    return std::fabs(a - b) <= epsilon * std::max(1.0, std::max(std::fabs(a), std::fabs(b)));
}

// Helper to compare vectors
bool vectors_approx_equal(const std::vector<double>& a, const std::vector<double>& b, double epsilon = 1e-6) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (!approx_equal(a[i], b[i], epsilon)) return false;
    }
    return true;
}

int main() {
    // Initialize Python interpreter for NumPy arrays
    py::scoped_interpreter guard{};
    
    // Check if NumPy is available
    try {
        py::module_ np = py::module_::import("numpy");
    } catch (const py::error_already_set& e) {
        std::cout << "⚠ NumPy not available - skipping SIMD tests that require NumPy arrays." << std::endl;
        std::cout << "  These functions are designed to be tested from Python." << std::endl;
        std::cout << "  To run full tests, install NumPy: pip install numpy" << std::endl;
        std::cout << "  Or run Python tests: python test/TimeSeries/RSI/Python/test_rsi.py" << std::endl;
        return 0; // Skip tests gracefully
    }
    
    std::cout << "Starting RSI SIMD Tests..." << std::endl;
    std::cout << std::endl;

    int tests_passed = 0;
    int tests_total = 0;

    // Test 1: Basic functionality
    {
        tests_total++;
        // Price series with known RSI behavior
        std::vector<double> prices = {100.0, 102.0, 101.0, 103.0, 105.0, 104.0, 106.0, 108.0, 107.0, 109.0, 110.0};
        size_t window_size = 5;
        
        py::array_t<double> prices_arr(prices.size());
        auto buf = prices_arr.mutable_unchecked<1>();
        for (size_t i = 0; i < prices.size(); ++i) {
            buf(i) = prices[i];
        }
        
        std::vector<double> result = compute_smoothed_rsi_simd(prices_arr, window_size);
        
        // RSI should be calculated and have reasonable values (0-100)
        bool valid = true;
        for (double rsi : result) {
            if (rsi < 0 || rsi > 100) {
                valid = false;
                break;
            }
        }
        
        if (valid && !result.empty()) {
            std::cout << "✓ Test 1 (Basic Functionality) Passed" << std::endl;
            tests_passed++;
        } else {
            std::cerr << "✗ Test 1 Failed: Invalid RSI values" << std::endl;
        }
    }

    // Test 2: Compare with non-SIMD version
    {
        tests_total++;
        std::vector<double> prices = {100.0, 101.0, 102.0, 101.0, 100.0, 99.0, 98.0, 99.0, 100.0, 101.0, 102.0};
        size_t window_size = 5;
        
        // Non-SIMD version
        std::vector<double> baseline = compute_smoothed_rsi(prices, window_size);
        
        // SIMD version
        py::array_t<double> prices_arr(prices.size());
        auto buf = prices_arr.mutable_unchecked<1>();
        for (size_t i = 0; i < prices.size(); ++i) {
            buf(i) = prices[i];
        }
        std::vector<double> simd_result = compute_smoothed_rsi_simd(prices_arr, window_size);
        
        if (vectors_approx_equal(baseline, simd_result, 1e-4)) {
            std::cout << "✓ Test 2 (Comparison with Baseline) Passed" << std::endl;
            tests_passed++;
        } else {
            std::cerr << "✗ Test 2 Failed: Results don't match baseline" << std::endl;
            std::cerr << "  Baseline size: " << baseline.size() << ", SIMD size: " << simd_result.size() << std::endl;
        }
    }

    // Test 3: All gains (should give high RSI)
    {
        tests_total++;
        std::vector<double> prices = {100.0, 101.0, 102.0, 103.0, 104.0, 105.0, 106.0, 107.0, 108.0, 109.0, 110.0};
        size_t window_size = 5;
        
        py::array_t<double> prices_arr(prices.size());
        auto buf = prices_arr.mutable_unchecked<1>();
        for (size_t i = 0; i < prices.size(); ++i) {
            buf(i) = prices[i];
        }
        
        std::vector<double> result = compute_smoothed_rsi_simd(prices_arr, window_size);
        
        // With all gains, RSI should be high (close to 100)
        if (!result.empty() && result[0] > 50.0) {
            std::cout << "✓ Test 3 (All Gains) Passed" << std::endl;
            tests_passed++;
        } else {
            std::cerr << "✗ Test 3 Failed: RSI should be high for all gains" << std::endl;
        }
    }

    // Test 4: All losses (should give low RSI)
    {
        tests_total++;
        std::vector<double> prices = {110.0, 109.0, 108.0, 107.0, 106.0, 105.0, 104.0, 103.0, 102.0, 101.0, 100.0};
        size_t window_size = 5;
        
        py::array_t<double> prices_arr(prices.size());
        auto buf = prices_arr.mutable_unchecked<1>();
        for (size_t i = 0; i < prices.size(); ++i) {
            buf(i) = prices[i];
        }
        
        std::vector<double> result = compute_smoothed_rsi_simd(prices_arr, window_size);
        
        // With all losses, RSI should be low (close to 0)
        if (!result.empty() && result[0] < 50.0) {
            std::cout << "✓ Test 4 (All Losses) Passed" << std::endl;
            tests_passed++;
        } else {
            std::cerr << "✗ Test 4 Failed: RSI should be low for all losses" << std::endl;
        }
    }

    // Test 5: Edge case - data smaller than window
    {
        tests_total++;
        std::vector<double> prices = {100.0, 101.0, 102.0};
        size_t window_size = 5;
        
        py::array_t<double> prices_arr(prices.size());
        auto buf = prices_arr.mutable_unchecked<1>();
        for (size_t i = 0; i < prices.size(); ++i) {
            buf(i) = prices[i];
        }
        
        std::vector<double> result = compute_smoothed_rsi_simd(prices_arr, window_size);
        
        if (result.empty()) {
            std::cout << "✓ Test 5 (Data Smaller Than Window) Passed" << std::endl;
            tests_passed++;
        } else {
            std::cerr << "✗ Test 5 Failed: Should return empty vector" << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Tests Passed: " << tests_passed << "/" << tests_total << std::endl;
    
    if (tests_passed == tests_total) {
        std::cout << "All RSI SIMD Tests Passed! ✅" << std::endl;
        std::cout << "========================================" << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed. ❌" << std::endl;
        std::cout << "========================================" << std::endl;
        return 1;
    }
}

