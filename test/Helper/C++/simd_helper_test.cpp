#include "finmath/Helper/simd_helper.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <iomanip>

// Helper to compare floating point numbers
bool approx_equal(double a, double b, double epsilon = 1e-9) {
    return std::fabs(a - b) <= epsilon * std::max(1.0, std::max(std::fabs(a), std::fabs(b)));
}

int main() {
    std::cout << "Starting SIMD Helper Tests..." << std::endl;
    std::cout << "SIMD Backend: " << finmath::simd::get_simd_backend() << std::endl;
    std::cout << std::endl;

    // Test data
    std::vector<double> a = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    std::vector<double> b = {10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0};
    std::vector<double> result(a.size());

    // Test 1: Vector Addition
    {
        finmath::simd::vector_add(a.data(), b.data(), result.data(), a.size());
        std::vector<double> expected = {11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0};
        
        for (size_t i = 0; i < a.size(); ++i) {
            if (!approx_equal(result[i], expected[i])) {
                std::cerr << "Test 1 (Vector Add) Failed at index " << i 
                          << ": expected " << expected[i] 
                          << ", got " << result[i] << std::endl;
                return 1;
            }
        }
        std::cout << "✓ Test 1 (Vector Addition) Passed" << std::endl;
    }

    // Test 2: Vector Subtraction
    {
        finmath::simd::vector_sub(a.data(), b.data(), result.data(), a.size());
        std::vector<double> expected = {-9.0, -7.0, -5.0, -3.0, -1.0, 1.0, 3.0, 5.0, 7.0, 9.0};
        
        for (size_t i = 0; i < a.size(); ++i) {
            if (!approx_equal(result[i], expected[i])) {
                std::cerr << "Test 2 (Vector Sub) Failed at index " << i 
                          << ": expected " << expected[i] 
                          << ", got " << result[i] << std::endl;
                return 1;
            }
        }
        std::cout << "✓ Test 2 (Vector Subtraction) Passed" << std::endl;
    }

    // Test 3: Vector Multiplication
    {
        finmath::simd::vector_mul(a.data(), b.data(), result.data(), a.size());
        std::vector<double> expected = {10.0, 18.0, 24.0, 28.0, 30.0, 30.0, 28.0, 24.0, 18.0, 10.0};
        
        for (size_t i = 0; i < a.size(); ++i) {
            if (!approx_equal(result[i], expected[i])) {
                std::cerr << "Test 3 (Vector Mul) Failed at index " << i 
                          << ": expected " << expected[i] 
                          << ", got " << result[i] << std::endl;
                return 1;
            }
        }
        std::cout << "✓ Test 3 (Vector Multiplication) Passed" << std::endl;
    }

    // Test 4: Dot Product
    {
        double result_dot = finmath::simd::dot_product(a.data(), b.data(), a.size());
        double expected = 220.0; // 1*10 + 2*9 + 3*8 + ... + 10*1
        
        if (!approx_equal(result_dot, expected)) {
            std::cerr << "Test 4 (Dot Product) Failed: expected " << expected 
                      << ", got " << result_dot << std::endl;
            return 1;
        }
        std::cout << "✓ Test 4 (Dot Product) Passed" << std::endl;
    }

    // Test 5: Vector Sum
    {
        double result_sum = finmath::simd::vector_sum(a.data(), a.size());
        double expected = 55.0; // 1 + 2 + 3 + ... + 10
        
        if (!approx_equal(result_sum, expected)) {
            std::cerr << "Test 5 (Vector Sum) Failed: expected " << expected 
                      << ", got " << result_sum << std::endl;
            return 1;
        }
        std::cout << "✓ Test 5 (Vector Sum) Passed" << std::endl;
    }

    // Test 6: Vector Mean
    {
        double result_mean = finmath::simd::vector_mean(a.data(), a.size());
        double expected = 5.5; // (1 + 2 + ... + 10) / 10
        
        if (!approx_equal(result_mean, expected)) {
            std::cerr << "Test 6 (Vector Mean) Failed: expected " << expected 
                      << ", got " << result_mean << std::endl;
            return 1;
        }
        std::cout << "✓ Test 6 (Vector Mean) Passed" << std::endl;
    }

    // Test 7: Vector Variance
    {
        double result_var = finmath::simd::vector_variance(a.data(), a.size());
        double expected = 8.25; // Variance of 1..10
        
        if (!approx_equal(result_var, expected)) {
            std::cerr << "Test 7 (Vector Variance) Failed: expected " << expected 
                      << ", got " << result_var << std::endl;
            return 1;
        }
        std::cout << "✓ Test 7 (Vector Variance) Passed" << std::endl;
    }

    // Test 8: Vector Standard Deviation
    {
        double result_std = finmath::simd::vector_stddev(a.data(), a.size());
        double expected = std::sqrt(8.25); // Std dev of 1..10
        
        if (!approx_equal(result_std, expected)) {
            std::cerr << "Test 8 (Vector Std Dev) Failed: expected " << expected 
                      << ", got " << result_std << std::endl;
            return 1;
        }
        std::cout << "✓ Test 8 (Vector Standard Deviation) Passed" << std::endl;
    }

    // Test 9: Edge case - Empty/Small vectors
    {
        std::vector<double> small = {5.0};
        double mean = finmath::simd::vector_mean(small.data(), small.size());
        
        if (!approx_equal(mean, 5.0)) {
            std::cerr << "Test 9 (Single Element) Failed" << std::endl;
            return 1;
        }
        std::cout << "✓ Test 9 (Edge Cases) Passed" << std::endl;
    }

    // Test 10: Vector Scalar Multiplication
    {
        double scalar = 2.5;
        finmath::simd::vector_mul_scalar(a.data(), scalar, result.data(), a.size());
        std::vector<double> expected = {2.5, 5.0, 7.5, 10.0, 12.5, 15.0, 17.5, 20.0, 22.5, 25.0};
        
        for (size_t i = 0; i < a.size(); ++i) {
            if (!approx_equal(result[i], expected[i])) {
                std::cerr << "Test 10 (Vector Mul Scalar) Failed at index " << i << std::endl;
                return 1;
            }
        }
        std::cout << "✓ Test 10 (Vector Scalar Multiplication) Passed" << std::endl;
    }

    // Test 11: Vector Scalar Addition
    {
        double scalar = 10.0;
        finmath::simd::vector_add_scalar(a.data(), scalar, result.data(), a.size());
        std::vector<double> expected = {11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0};
        
        for (size_t i = 0; i < a.size(); ++i) {
            if (!approx_equal(result[i], expected[i])) {
                std::cerr << "Test 11 (Vector Add Scalar) Failed at index " << i << std::endl;
                return 1;
            }
        }
        std::cout << "✓ Test 11 (Vector Scalar Addition) Passed" << std::endl;
    }

    // Test 12: Vector Division
    {
        std::vector<double> numerator = {10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0};
        std::vector<double> denominator = {2.0, 4.0, 5.0, 8.0, 10.0, 12.0, 14.0, 16.0, 18.0, 20.0};
        finmath::simd::vector_div(numerator.data(), denominator.data(), result.data(), numerator.size());
        std::vector<double> expected = {5.0, 5.0, 6.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0};
        
        for (size_t i = 0; i < numerator.size(); ++i) {
            if (!approx_equal(result[i], expected[i])) {
                std::cerr << "Test 12 (Vector Division) Failed at index " << i << std::endl;
                return 1;
            }
        }
        std::cout << "✓ Test 12 (Vector Division) Passed" << std::endl;
    }

    // Test 13: Vector Maximum
    {
        std::vector<double> test_data = {1.0, 5.0, 3.0, 9.0, 2.0, 8.0, 4.0, 7.0, 6.0, 10.0};
        double max_val = finmath::simd::vector_max(test_data.data(), test_data.size());
        double expected = 10.0;
        
        if (!approx_equal(max_val, expected)) {
            std::cerr << "Test 13 (Vector Max) Failed: expected " << expected 
                      << ", got " << max_val << std::endl;
            return 1;
        }
        std::cout << "✓ Test 13 (Vector Maximum) Passed" << std::endl;
    }

    // Test 14: Vector Minimum
    {
        std::vector<double> test_data = {10.0, 5.0, 3.0, 9.0, 2.0, 8.0, 4.0, 7.0, 6.0, 1.0};
        double min_val = finmath::simd::vector_min(test_data.data(), test_data.size());
        double expected = 1.0;
        
        if (!approx_equal(min_val, expected)) {
            std::cerr << "Test 14 (Vector Min) Failed: expected " << expected 
                      << ", got " << min_val << std::endl;
            return 1;
        }
        std::cout << "✓ Test 14 (Vector Minimum) Passed" << std::endl;
    }

    // Test 15: Vector Conditional Sum (Positive)
    {
        std::vector<double> test_data = {-5.0, 10.0, -3.0, 7.0, -2.0, 8.0, -1.0, 5.0, -4.0, 6.0};
        double sum_positive = finmath::simd::vector_conditional_sum(test_data.data(), test_data.size(), true);
        double expected = 36.0; // 10 + 7 + 8 + 5 + 6
        
        if (!approx_equal(sum_positive, expected)) {
            std::cerr << "Test 15 (Conditional Sum Positive) Failed: expected " << expected 
                      << ", got " << sum_positive << std::endl;
            return 1;
        }
        std::cout << "✓ Test 15 (Vector Conditional Sum - Positive) Passed" << std::endl;
    }

    // Test 16: Vector Conditional Sum (Negative)
    {
        std::vector<double> test_data = {-5.0, 10.0, -3.0, 7.0, -2.0, 8.0, -1.0, 5.0, -4.0, 6.0};
        double sum_negative = finmath::simd::vector_conditional_sum(test_data.data(), test_data.size(), false);
        double expected = 15.0; // abs(-5) + abs(-3) + abs(-2) + abs(-1) + abs(-4) = 5 + 3 + 2 + 1 + 4
        
        if (!approx_equal(sum_negative, expected)) {
            std::cerr << "Test 16 (Conditional Sum Negative) Failed: expected " << expected 
                      << ", got " << sum_negative << std::endl;
            return 1;
        }
        std::cout << "✓ Test 16 (Vector Conditional Sum - Negative) Passed" << std::endl;
    }

    // Test 17: Large vector to test SIMD performance
    {
        const size_t large_size = 10000;
        std::vector<double> large_a(large_size);
        std::vector<double> large_b(large_size);
        std::vector<double> large_result(large_size);
        
        // Initialize with some pattern
        for (size_t i = 0; i < large_size; ++i) {
            large_a[i] = static_cast<double>(i);
            large_b[i] = static_cast<double>(large_size - i);
        }
        
        // Test addition on large vectors
        finmath::simd::vector_add(large_a.data(), large_b.data(), large_result.data(), large_size);
        
        // Verify a few samples
        bool large_test_passed = true;
        for (size_t i = 0; i < large_size; i += 1000) {
            double expected = static_cast<double>(large_size);
            if (!approx_equal(large_result[i], expected)) {
                std::cerr << "Test 17 (Large Vector) Failed at index " << i << std::endl;
                large_test_passed = false;
                break;
            }
        }
        
        if (large_test_passed) {
            std::cout << "✓ Test 17 (Large Vector Operations) Passed" << std::endl;
        } else {
            return 1;
        }
    }

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "All SIMD Helper Tests Passed! ✅" << std::endl;
    std::cout << "Backend: " << finmath::simd::get_simd_backend() << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}

