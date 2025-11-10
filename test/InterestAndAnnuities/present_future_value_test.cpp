#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "finmath/InterestAndAnnuities/present_future_value.h"

// Helper function for floating-point comparison
bool almost_equal(double a, double b, double tolerance = 1e-5) {
    return std::abs(a - b) < tolerance;
}

int main() {
    int tests_passed = 0;
    int tests_total = 0;

    // Test 1: Basic present value calculation
    {
        tests_total++;
        double df = present_value(1.0, 0.05, 5);
        double expected = 0.78352616646; // 1.0 / (1 + 0.05)^5
        if (almost_equal(df, expected, 1e-6)) {
            std::cout << "✓ Test 1 passed: Basic present value" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 1 failed: Expected " << expected
                      << ", got " << df << std::endl;
        }
    }

    // Test 2: Basic present value (negative interest rate)
    {
        tests_total++;
        double df = present_value(1.0, -0.05, 5);
        double expected = 1.2923554349; // 1.0 / (1 - 0.05)^5
        if (almost_equal(df, expected, 1e-6)) {
            std::cout << "✓ Test 2 passed: Basic present value" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 2 failed: Expected " << expected
                      << ", got " << df << std::endl;
        }
    }

    // Test 3: Basic future value calculation
    {
        tests_total++;
        double df = future_value(1.0, 0.05, 5);
        double expected = 1.2762815625; // 1.0 * (1 + 0.05)^5
        if (almost_equal(df, expected, 1e-6)) {
            std::cout << "✓ Test 3 passed: Basic future value" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 3 failed: Expected " << expected
                      << ", got " << df << std::endl;
        }
    }

    // Test 4: Basic future value (negative interest rate)
    {
        tests_total++;
        double df = future_value(1.0, -0.05, 5);
        double expected = 0.7737809375; // 1.0 * (1 - 0.05)^5
        if (almost_equal(df, expected, 1e-6)) {
            std::cout << "✓ Test 4 passed: Basic future value" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 4 failed: Expected " << expected
                      << ", got " << df << std::endl;
        }
    }

    // Test 5: Continuous present value calculation
    {
        tests_total++;
        double df = present_value_continuous(1.0, 0.05, 5);
        double expected = 0.77880078307; // 1.0 * e ^ (-0.05 * 5)
        if (almost_equal(df, expected, 1e-6)) {
            std::cout << "✓ Test 5 passed: Continuous present value" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 5 failed: Expected " << expected
                      << ", got " << df << std::endl;
        }
    }

    // Test 6: Continuous present value (negative interest rate)
    {
        tests_total++;
        double df = present_value_continuous(1.0, -0.05, 5);
        double expected = 1.28402541669; // 1.0 * e ^ (0.05 * 5)
        if (almost_equal(df, expected, 1e-6)) {
            std::cout << "✓ Test 6 passed: Continuous present value" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 6 failed: Expected " << expected
                      << ", got " << df << std::endl;
        }
    }

    // Test 7: Continuous future value calculation
    {
        tests_total++;
        double df = future_value_continuous(1.0, 0.05, 5);
        double expected = 1.28402541669; // 1.0 * e ^ (0.05 * 5)
        if (almost_equal(df, expected, 1e-6)) {
            std::cout << "✓ Test 7 passed: Continuous future value" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 7 failed: Expected " << expected
                      << ", got " << df << std::endl;
        }
    }

    // Test 8: Continuous future value (negative interest rate)
    {
        tests_total++;
        double df = future_value_continuous(1.0, -0.05, 5);
        double expected = 0.77880078307; // 1.0 * e ^ (-0.05 * 5)
        if (almost_equal(df, expected, 1e-6)) {
            std::cout << "✓ Test 8 passed: Continuous future value" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 8 failed: Expected " << expected
                      << ", got " << df << std::endl;
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
