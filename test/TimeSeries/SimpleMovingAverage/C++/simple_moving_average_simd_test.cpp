#include "finmath/TimeSeries/simple_moving_average_simd.h"
#include "finmath/TimeSeries/simple_moving_average.h"
#include <pybind11/numpy.h>
#include <pybind11/embed.h>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

namespace py = pybind11;

// Helper to compare floating point numbers
bool approx_equal(double a, double b, double epsilon = 1e-9) {
    return std::fabs(a - b) <= epsilon * std::max(1.0, std::max(std::fabs(a), std::fabs(b)));
}

// Helper to compare vectors
bool vectors_approx_equal(const std::vector<double>& a, const std::vector<double>& b, double epsilon = 1e-9) {
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
        std::cout << "  Or run Python tests: python test/TimeSeries/SimpleMovingAverage/Python/test_simple_moving_average.py" << std::endl;
        return 0; // Skip tests gracefully
    }
    
    std::cout << "Starting Simple Moving Average SIMD Tests..." << std::endl;
    std::cout << std::endl;

    int tests_passed = 0;
    int tests_total = 0;

    // Test 1: Basic functionality
    {
        tests_total++;
        std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
        size_t window_size = 3;
        
        // Convert to NumPy array
        py::array_t<double> data_arr(data.size());
        auto buf = data_arr.mutable_unchecked<1>();
        for (size_t i = 0; i < data.size(); ++i) {
            buf(i) = data[i];
        }
        
        std::vector<double> result = simple_moving_average_simd(data_arr, window_size);
        std::vector<double> expected = {2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0}; // (1+2+3)/3, (2+3+4)/3, ...
        
        if (vectors_approx_equal(result, expected)) {
            std::cout << "✓ Test 1 (Basic Functionality) Passed" << std::endl;
            tests_passed++;
        } else {
            std::cerr << "✗ Test 1 Failed" << std::endl;
        }
    }

    // Test 2: Compare with non-SIMD version
    {
        tests_total++;
        std::vector<double> data = {10.0, 20.0, 30.0, 40.0, 50.0, 60.0};
        size_t window_size = 2;
        
        // Non-SIMD version
        std::vector<double> baseline = simple_moving_average(data, window_size);
        
        // SIMD version
        py::array_t<double> data_arr(data.size());
        auto buf = data_arr.mutable_unchecked<1>();
        for (size_t i = 0; i < data.size(); ++i) {
            buf(i) = data[i];
        }
        std::vector<double> simd_result = simple_moving_average_simd(data_arr, window_size);
        
        if (vectors_approx_equal(baseline, simd_result, 1e-6)) {
            std::cout << "✓ Test 2 (Comparison with Baseline) Passed" << std::endl;
            tests_passed++;
        } else {
            std::cerr << "✗ Test 2 Failed: Results don't match baseline" << std::endl;
        }
    }

    // Test 3: Window size equals data size
    {
        tests_total++;
        std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0};
        size_t window_size = 5;
        
        py::array_t<double> data_arr(data.size());
        auto buf = data_arr.mutable_unchecked<1>();
        for (size_t i = 0; i < data.size(); ++i) {
            buf(i) = data[i];
        }
        
        std::vector<double> result = simple_moving_average_simd(data_arr, window_size);
        
        if (result.size() == 1 && approx_equal(result[0], 3.0)) { // (1+2+3+4+5)/5
            std::cout << "✓ Test 3 (Window Size = Data Size) Passed" << std::endl;
            tests_passed++;
        } else {
            std::cerr << "✗ Test 3 Failed" << std::endl;
        }
    }

    // Test 4: Large dataset
    {
        tests_total++;
        const size_t data_size = 1000;
        std::vector<double> data(data_size);
        for (size_t i = 0; i < data_size; ++i) {
            data[i] = static_cast<double>(i + 1);
        }
        
        size_t window_size = 50;
        
        py::array_t<double> data_arr(data.size());
        auto buf = data_arr.mutable_unchecked<1>();
        for (size_t i = 0; i < data.size(); ++i) {
            buf(i) = data[i];
        }
        
        std::vector<double> result = simple_moving_average_simd(data_arr, window_size);
        
        // Verify first and last values
        double expected_first = (1.0 + 50.0) / 2.0; // Average of first window
        double expected_last = (951.0 + 1000.0) / 2.0; // Average of last window
        
        if (result.size() == data_size - window_size + 1 &&
            approx_equal(result[0], expected_first, 1e-6) &&
            approx_equal(result.back(), expected_last, 1e-6)) {
            std::cout << "✓ Test 4 (Large Dataset) Passed" << std::endl;
            tests_passed++;
        } else {
            std::cerr << "✗ Test 4 Failed" << std::endl;
        }
    }

    // Test 5: Edge case - data smaller than window
    {
        tests_total++;
        std::vector<double> data = {1.0, 2.0, 3.0};
        size_t window_size = 5;
        
        py::array_t<double> data_arr(data.size());
        auto buf = data_arr.mutable_unchecked<1>();
        for (size_t i = 0; i < data.size(); ++i) {
            buf(i) = data[i];
        }
        
        std::vector<double> result = simple_moving_average_simd(data_arr, window_size);
        
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
        std::cout << "All Simple Moving Average SIMD Tests Passed! ✅" << std::endl;
        std::cout << "========================================" << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed. ❌" << std::endl;
        std::cout << "========================================" << std::endl;
        return 1;
    }
}

