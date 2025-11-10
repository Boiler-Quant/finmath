#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "finmath/InterestAndAnnuities/discount_factor.h"

// Helper function for floating-point comparison
bool almost_equal(double a, double b, double tolerance = 1e-5) {
    return std::abs(a - b) < tolerance;
}

int main() {
    int tests_passed = 0;
    int tests_total = 0;

    // Test 1: Basic discount factor calculation
    {
        tests_total++;
        double df = discount_factor(0.05, 1.0);
        double expected = 1.0 / 1.05; // 0.952380952...
        if (almost_equal(df, expected, 1e-6)) {
            std::cout << "✓ Test 1 passed: Basic discount factor" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 1 failed: Expected " << expected
                      << ", got " << df << std::endl;
        }
    }

    // Test 2: Discount factor for multiple years
    {
        tests_total++;
        double df = discount_factor(0.10, 2.0);
        double expected = 1.0 / (1.1 * 1.1); // 0.826446281
        if (almost_equal(df, expected, 1e-6)) {
            std::cout << "✓ Test 2 passed: Multi-year discount factor" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 2 failed" << std::endl;
        }
    }

    // Test 3: Zero rate
    {
        tests_total++;
        double df = discount_factor(0.0, 1.0);
        if (almost_equal(df, 1.0, 1e-6)) {
            std::cout << "✓ Test 3 passed: Zero rate" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 3 failed" << std::endl;
        }
    }

    // Test 4: Zero time
    {
        tests_total++;
        double df = discount_factor(0.05, 0.0);
        if (almost_equal(df, 1.0, 1e-6)) {
            std::cout << "✓ Test 4 passed: Zero time" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 4 failed" << std::endl;
        }
    }

    // Test 5: Negative rate (should throw exception)
    {
        tests_total++;
        try {
            discount_factor(-0.05, 1.0);
            std::cout << "✗ Test 5 failed: Should have thrown exception" << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << "✓ Test 5 passed: Negative rate exception" << std::endl;
            tests_passed++;
        }
    }

    // Test 6: Negative time (should throw exception)
    {
        tests_total++;
        try {
            discount_factor(0.05, -1.0);
            std::cout << "✗ Test 6 failed: Should have thrown exception" << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << "✓ Test 6 passed: Negative time exception" << std::endl;
            tests_passed++;
        }
    }

    // Test 7: Continuous discount factor
    {
        tests_total++;
        double df = discount_factor_continuous(0.05, 1.0);
        double expected = std::exp(-0.05); // 0.9512294245
        if (almost_equal(df, expected, 1e-6)) {
            std::cout << "✓ Test 7 passed: Continuous discount factor" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 7 failed: Expected " << expected
                      << ", got " << df << std::endl;
        }
    }

    // Test 8: Future value factor
    {
        tests_total++;
        double fvf = future_value_factor(0.05, 1.0);
        double expected = 1.05;
        if (almost_equal(fvf, expected, 1e-6)) {
            std::cout << "✓ Test 8 passed: Future value factor" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 8 failed" << std::endl;
        }
    }

    // Summary
    std::cout << "\\n" << tests_passed << "/" << tests_total
              << " tests passed" << std::endl;

    if (tests_passed == tests_total) {
        std::cout << "All tests passed! ✓" << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed. Please review your implementation." << std::endl;
        return 1;
    }
}
