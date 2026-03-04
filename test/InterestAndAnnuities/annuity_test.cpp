#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "finmath/InterestAndAnnuities/annuity.h"

bool almost_equal(double a, double b, double tolerance = 1e-5) {
    return std::abs(a - b) < tolerance;
}

int main() {
    int tests_passed = 0;
    int tests_total  = 0;

    // -------------------------------------------------------------------------
    // present_value_annuity
    // PVA = PMT * [1 - (1+r)^-n] / r
    // -------------------------------------------------------------------------

    // Test 1: Basic PVA
    {
        tests_total++;
        // PMT=100, r=0.05, n=5 → PVA = 100 * [1 - 1.05^-5] / 0.05 ≈ 432.948
        double pva      = present_value_annuity(100.0, 0.05, 5);
        double expected = 432.948;
        if (almost_equal(pva, expected, 0.001)) {
            std::cout << "✓ Test 1 passed: Basic PVA" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 1 failed: Expected ~" << expected << ", got " << pva << std::endl;
        }
    }

    // Test 2: Zero interest rate — PVA == PMT * n
    {
        tests_total++;
        double pva = present_value_annuity(100.0, 0.0, 10);
        if (almost_equal(pva, 1000.0, 1e-4)) {
            std::cout << "✓ Test 2 passed: Zero rate PVA == PMT * n" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 2 failed: Expected 1000.0, got " << pva << std::endl;
        }
    }

    // Test 3: Single period — PVA == PMT / (1+r)
    {
        tests_total++;
        double pva      = present_value_annuity(100.0, 0.05, 1);
        double expected = 100.0 / 1.05;
        if (almost_equal(pva, expected, 1e-6)) {
            std::cout << "✓ Test 3 passed: Single period PVA" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 3 failed: Expected " << expected << ", got " << pva << std::endl;
        }
    }

    // Test 4: Negative payment amount (outflow annuity)
    {
        tests_total++;
        double pva_pos = present_value_annuity( 100.0, 0.05, 5);
        double pva_neg = present_value_annuity(-100.0, 0.05, 5);
        if (almost_equal(pva_pos, -pva_neg, 1e-6)) {
            std::cout << "✓ Test 4 passed: Negative PMT symmetry" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 4 failed: Symmetry broken" << std::endl;
        }
    }

    // Test 5: Negative rate — should throw
    {
        tests_total++;
        try {
            present_value_annuity(100.0, -0.05, 5);
            std::cout << "✗ Test 5 failed: Should have thrown on negative rate" << std::endl;
        } catch (const std::invalid_argument&) {
            std::cout << "✓ Test 5 passed: Negative rate exception" << std::endl;
            tests_passed++;
        }
    }

    // Test 6: Zero periods — should throw or return 0
    {
        tests_total++;
        try {
            double pva = present_value_annuity(100.0, 0.05, 0);
            if (almost_equal(pva, 0.0, 1e-9)) {
                std::cout << "✓ Test 6 passed: Zero periods returns 0" << std::endl;
                tests_passed++;
            } else {
                std::cout << "✗ Test 6 failed: Expected 0 for zero periods, got " << pva << std::endl;
            }
        } catch (const std::invalid_argument&) {
            std::cout << "✓ Test 6 passed: Zero periods exception" << std::endl;
            tests_passed++;
        }
    }

    // -------------------------------------------------------------------------
    // future_value_annuity
    // FVA = PMT * [(1+r)^n - 1] / r
    // -------------------------------------------------------------------------

    // Test 7: Basic FVA
    {
        tests_total++;
        // PMT=100, r=0.05, n=5 → FVA ≈ 552.563
        double fva      = future_value_annuity(100.0, 0.05, 5);
        double expected = 552.563;
        if (almost_equal(fva, expected, 0.001)) {
            std::cout << "✓ Test 7 passed: Basic FVA" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 7 failed: Expected ~" << expected << ", got " << fva << std::endl;
        }
    }

    // Test 8: FVA == PVA * (1+r)^n  (consistency check)
    {
        tests_total++;
        double pva = present_value_annuity(100.0, 0.05, 5);
        double fva = future_value_annuity(100.0, 0.05, 5);
        double fva_from_pva = pva * std::pow(1.05, 5);
        if (almost_equal(fva, fva_from_pva, 1e-4)) {
            std::cout << "✓ Test 8 passed: FVA == PVA * (1+r)^n" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 8 failed: FVA=" << fva << " vs PVA*(1+r)^n=" << fva_from_pva << std::endl;
        }
    }

    // Test 9: Zero rate FVA == PMT * n
    {
        tests_total++;
        double fva = future_value_annuity(100.0, 0.0, 10);
        if (almost_equal(fva, 1000.0, 1e-4)) {
            std::cout << "✓ Test 9 passed: Zero rate FVA == PMT * n" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 9 failed: Expected 1000.0, got " << fva << std::endl;
        }
    }

    // -------------------------------------------------------------------------
    // annuity_due  (payments at start of period — multiply ordinary annuity by (1+r))
    // -------------------------------------------------------------------------

    // Test 10: PVA due == ordinary PVA * (1+r)
    {
        tests_total++;
        double pva_ordinary = present_value_annuity(100.0, 0.05, 5);
        double pva_due      = present_value_annuity_due(100.0, 0.05, 5);
        if (almost_equal(pva_due, pva_ordinary * 1.05, 1e-4)) {
            std::cout << "✓ Test 10 passed: PVA due == PVA ordinary * (1+r)" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 10 failed: Expected " << pva_ordinary * 1.05
                      << ", got " << pva_due << std::endl;
        }
    }

    // Test 11: Perpetuity PV == PMT / r
    {
        tests_total++;
        double pv_perp  = perpetuity_present_value(100.0, 0.05);
        double expected = 100.0 / 0.05; // 2000
        if (almost_equal(pv_perp, expected, 1e-4)) {
            std::cout << "✓ Test 11 passed: Perpetuity PV == PMT / r" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 11 failed: Expected " << expected << ", got " << pv_perp << std::endl;
        }
    }

    // Test 12: Perpetuity with zero rate — should throw
    {
        tests_total++;
        try {
            perpetuity_present_value(100.0, 0.0);
            std::cout << "✗ Test 12 failed: Should have thrown on zero rate" << std::endl;
        } catch (const std::invalid_argument&) {
            std::cout << "✓ Test 12 passed: Perpetuity zero rate exception" << std::endl;
            tests_passed++;
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
