#include "finmath/TimeSeries/ema_simd.h"
#include "finmath/TimeSeries/ema.h"
#include <pybind11/numpy.h>
#include <pybind11/embed.h>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

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
        std::cout << "  Or run Python tests: python test/TimeSeries/EMA/Python/test_ema.py" << std::endl;
        return 0; // Skip tests gracefully
    }
    
    std::cout << "Starting EMA SIMD Tests..." << std::endl;
    std::cout << std::endl;

    int tests_passed = 0;
    int tests_total = 0;

    // Test 1: Basic functionality with window
    {
        tests_total++;
        std::vector<double> prices = {100.0, 101.0, 102.0, 103.0, 104.0, 105.0};
        size_t window = 3;
        
        py::array_t<double> prices_arr(prices.size());
        auto buf = prices_arr.mutable_unchecked<1>();
        for (size_t i = 0; i < prices.size(); ++i) {
            buf(i) = prices[i];
        }
        
        std::vector<double> result = compute_ema_simd(prices_arr, window);
        
        // EMA should start with first price
        if (!result.empty() && approx_equal(result[0], prices[0])) {
            std::cout << "✓ Test 1 (Basic Functionality - Window) Passed" << std::endl;
            tests_passed++;
        } else {
            std::cerr << "✗ Test 1 Failed" << std::endl;
        }
    }

    // Test 2: Compare with non-SIMD version (window)
    {
        tests_total++;
        std::vector<double> prices = {100.0, 101.0, 102.0, 101.0, 100.0, 99.0, 98.0};
        size_t window = 5;
        
        // Non-SIMD version
        std::vector<double> baseline = compute_ema(prices, window);
        
        // SIMD version
        py::array_t<double> prices_arr(prices.size());
        auto buf = prices_arr.mutable_unchecked<1>();
        for (size_t i = 0; i < prices.size(); ++i) {
            buf(i) = prices[i];
        }
        std::vector<double> simd_result = compute_ema_simd(prices_arr, window);
        
        if (vectors_approx_equal(baseline, simd_result, 1e-5)) {
            std::cout << "✓ Test 2 (Comparison with Baseline - Window) Passed" << std::endl;
            tests_passed++;
        } else {
            std::cerr << "✗ Test 2 Failed: Results don't match baseline" << std::endl;
        }
    }

    // Test 3: Basic functionality with smoothing factor
    {
        tests_total++;
        std::vector<double> prices = {100.0, 101.0, 102.0, 103.0, 104.0};
        double smoothing_factor = 0.2;
        
        py::array_t<double> prices_arr(prices.size());
        auto buf = prices_arr.mutable_unchecked<1>();
        for (size_t i = 0; i < prices.size(); ++i) {
            buf(i) = prices[i];
        }
        
        std::vector<double> result = compute_ema_with_smoothing_simd(prices_arr, smoothing_factor);
        
        // EMA should start with first price
        if (!result.empty() && approx_equal(result[0], prices[0])) {
            std::cout << "✓ Test 3 (Basic Functionality - Smoothing) Passed" << std::endl;
            tests_passed++;
        } else {
            std::cerr << "✗ Test 3 Failed" << std::endl;
        }
    }

    // Test 4: Compare with non-SIMD version (smoothing)
    {
        tests_total++;
        std::vector<double> prices = {100.0, 101.0, 102.0, 101.0, 100.0, 99.0};
        double smoothing_factor = 0.3;
        
        // Non-SIMD version
        std::vector<double> baseline = compute_ema_with_smoothing(prices, smoothing_factor);
        
        // SIMD version
        py::array_t<double> prices_arr(prices.size());
        auto buf = prices_arr.mutable_unchecked<1>();
        for (size_t i = 0; i < prices.size(); ++i) {
            buf(i) = prices[i];
        }
        std::vector<double> simd_result = compute_ema_with_smoothing_simd(prices_arr, smoothing_factor);
        
        if (vectors_approx_equal(baseline, simd_result, 1e-5)) {
            std::cout << "✓ Test 4 (Comparison with Baseline - Smoothing) Passed" << std::endl;
            tests_passed++;
        } else {
            std::cerr << "✗ Test 4 Failed: Results don't match baseline" << std::endl;
        }
    }

    // Test 5: Edge case - single element
    {
        tests_total++;
        std::vector<double> prices = {100.0};
        size_t window = 5;
        
        py::array_t<double> prices_arr(prices.size());
        auto buf = prices_arr.mutable_unchecked<1>();
        for (size_t i = 0; i < prices.size(); ++i) {
            buf(i) = prices[i];
        }
        
        std::vector<double> result = compute_ema_simd(prices_arr, window);
        
        if (result.size() == 1 && approx_equal(result[0], prices[0])) {
            std::cout << "✓ Test 5 (Single Element) Passed" << std::endl;
            tests_passed++;
        } else {
            std::cerr << "✗ Test 5 Failed" << std::endl;
        }
    }

    // Test 6: Large dataset
    {
        tests_total++;
        const size_t data_size = 1000;
        std::vector<double> prices(data_size);
        for (size_t i = 0; i < data_size; ++i) {
            prices[i] = 100.0 + static_cast<double>(i) * 0.1;
        }
        
        size_t window = 20;
        
        py::array_t<double> prices_arr(prices.size());
        auto buf = prices_arr.mutable_unchecked<1>();
        for (size_t i = 0; i < prices.size(); ++i) {
            buf(i) = prices[i];
        }
        
        std::vector<double> result = compute_ema_simd(prices_arr, window);
        
        // Verify size and that EMA tracks prices
        if (result.size() == prices.size() && 
            approx_equal(result[0], prices[0]) &&
            result.back() > result[0]) { // EMA should increase with increasing prices
            std::cout << "✓ Test 6 (Large Dataset) Passed" << std::endl;
            tests_passed++;
        } else {
            std::cerr << "✗ Test 6 Failed" << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Tests Passed: " << tests_passed << "/" << tests_total << std::endl;
    
    if (tests_passed == tests_total) {
        std::cout << "All EMA SIMD Tests Passed! ✅" << std::endl;
        std::cout << "========================================" << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed. ❌" << std::endl;
        std::cout << "========================================" << std::endl;
        return 1;
    }
}

