#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "finmath/InterestAndAnnuities/present_future_value.h"

bool almost_equal(double a, double b, double tolerance = 1e-5) {
    return std::abs(a - b) < tolerance;
}

int main() {
    int tests_passed = 0;
    int tests_total  = 0;

    // -------------------------------------------------------------------------
    // present_value   PV = FV / (1+r)^t
    // -------------------------------------------------------------------------

    // Test 1: Basic PV
    {
        tests_total++;
        double pv       = present_value(1000.0, 0.10, 1.0);
        double expected = 1000.0 / 1.10;
        if (almost_equal(pv, expected, 1e-6)) {
            std::cout << "✓ Test 1 passed: Basic present value" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 1 failed: Expected " << expected << ", got " << pv << std::endl;
        }
    }

    // Test 2: Multi-year PV
    {
        tests_total++;
        double pv       = present_value(1000.0, 0.10, 2.0);
        double expected = 1000.0 / (1.1 * 1.1);
        if (almost_equal(pv, expected, 1e-6)) {
            std::cout << "✓ Test 2 passed: Multi-year PV" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 2 failed: Expected " << expected << ", got " << pv << std::endl;
        }
    }

    // Test 3: Zero rate — PV == FV
    {
        tests_total++;
        double pv = present_value(500.0, 0.0, 5.0);
        if (almost_equal(pv, 500.0, 1e-6)) {
            std::cout << "✓ Test 3 passed: Zero rate PV == FV" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 3 failed: Expected 500.0, got " << pv << std::endl;
        }
    }

    // Test 4: Zero time — PV == FV
    {
        tests_total++;
        double pv = present_value(500.0, 0.10, 0.0);
        if (almost_equal(pv, 500.0, 1e-6)) {
            std::cout << "✓ Test 4 passed: Zero time PV == FV" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 4 failed: Expected 500.0, got " << pv << std::endl;
        }
    }

    // Test 5: Negative rate — should throw
    {
        tests_total++;
        try {
            present_value(1000.0, -0.05, 1.0);
            std::cout << "✗ Test 5 failed: Should have thrown on negative rate" << std::endl;
        } catch (const std::invalid_argument&) {
            std::cout << "✓ Test 5 passed: Negative rate exception" << std::endl;
            tests_passed++;
        }
    }

    // Test 6: Negative time — should throw
    {
        tests_total++;
        try {
            present_value(1000.0, 0.05, -1.0);
            std::cout << "✗ Test 6 failed: Should have thrown on negative time" << std::endl;
        } catch (const std::invalid_argument&) {
            std::cout << "✓ Test 6 passed: Negative time exception" << std::endl;
            tests_passed++;
        }
    }

    // -------------------------------------------------------------------------
    // future_value   FV = PV * (1+r)^t
    // -------------------------------------------------------------------------

    // Test 7: Basic FV
    {
        tests_total++;
        double fv       = future_value(1000.0, 0.10, 1.0);
        double expected = 1100.0;
        if (almost_equal(fv, expected, 1e-6)) {
            std::cout << "✓ Test 7 passed: Basic future value" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 7 failed: Expected " << expected << ", got " << fv << std::endl;
        }
    }

    // Test 8: FV round-trip with PV
    {
        tests_total++;
        double original = 750.0;
        double fv       = future_value(original, 0.08, 3.0);
        double pv_back  = present_value(fv, 0.08, 3.0);
        if (almost_equal(pv_back, original, 1e-6)) {
            std::cout << "✓ Test 8 passed: FV/PV round-trip" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 8 failed: Round-trip got " << pv_back << std::endl;
        }
    }

    // Test 9: Zero rate FV == PV
    {
        tests_total++;
        double fv = future_value(300.0, 0.0, 10.0);
        if (almost_equal(fv, 300.0, 1e-6)) {
            std::cout << "✓ Test 9 passed: Zero rate FV == PV" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 9 failed: Expected 300.0, got " << fv << std::endl;
        }
    }

    // Test 10: Known compound growth — $1000 at 5% for 10 years ≈ $1628.89
    {
        tests_total++;
        double fv       = future_value(1000.0, 0.05, 10.0);
        double expected = 1628.894627;
        if (almost_equal(fv, expected, 0.001)) {
            std::cout << "✓ Test 10 passed: Known 10-year compound growth" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 10 failed: Expected ~" << expected << ", got " << fv << std::endl;
        }
    }

    // -------------------------------------------------------------------------
    // present_value_continuous  PV = FV * e^(-r*t)
    // future_value_continuous   FV = PV * e^(r*t)
    // -------------------------------------------------------------------------

    // Test 11: Continuous PV
    {
        tests_total++;
        double pv       = present_value_continuous(1000.0, 0.05, 2.0);
        double expected = 1000.0 * std::exp(-0.05 * 2.0);
        if (almost_equal(pv, expected, 1e-6)) {
            std::cout << "✓ Test 11 passed: Continuous PV" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 11 failed: Expected " << expected << ", got " << pv << std::endl;
        }
    }

    // Test 12: Continuous FV round-trip
    {
        tests_total++;
        double original = 500.0;
        double fv       = future_value_continuous(original, 0.07, 4.0);
        double pv_back  = present_value_continuous(fv, 0.07, 4.0);
        if (almost_equal(pv_back, original, 1e-6)) {
            std::cout << "✓ Test 12 passed: Continuous FV/PV round-trip" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 12 failed: Round-trip got " << pv_back << std::endl;
        }
    }

    // Test 13: Continuous PV < discrete PV for same inputs (continuous discounts more)
    {
        tests_total++;
        double pv_disc = present_value(1000.0, 0.05, 1.0);
        double pv_cont = present_value_continuous(1000.0, 0.05, 1.0);
        if (pv_cont < pv_disc) {
            std::cout << "✓ Test 13 passed: Continuous PV < discrete PV" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 13 failed: Expected pv_cont < pv_disc" << std::endl;
        }
    }

    // Summary
    std::cout << "\n" << tests_passed << "/" << tests_total << " tests passed" << std::endl;
    if (tests_passed == tests_total) {
        std::cout << "All tests passed! ✓" << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed. Please review your implementation." << std::endl;
        return 1;
    }
}
