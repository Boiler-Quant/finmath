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
    // annuity_present_value
    // PV = P * [1 - (1+r)^(-n)] / r
    // -------------------------------------------------------------------------

    // Test 1: Basic PVA — PMT=100, r=0.05, n=5 → ≈ 432.948
    {
        tests_total++;
        double pva      = annuity_present_value(100.0, 0.05, 5);
        double expected = 432.948;
        if (almost_equal(pva, expected, 0.001)) {
            std::cout << "✓ Test 1 passed: Basic annuity_present_value" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 1 failed: Expected ~" << expected << ", got " << pva << std::endl;
        }
    }

    // Test 2: Zero rate — PV == PMT * n
    {
        tests_total++;
        double pva = annuity_present_value(100.0, 0.0, 10);
        if (almost_equal(pva, 1000.0, 1e-4)) {
            std::cout << "✓ Test 2 passed: Zero rate PV == PMT * n" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 2 failed: Expected 1000.0, got " << pva << std::endl;
        }
    }

    // Test 3: Single period — PV == PMT / (1+r)
    {
        tests_total++;
        double pva      = annuity_present_value(100.0, 0.05, 1);
        double expected = 100.0 / 1.05;
        if (almost_equal(pva, expected, 1e-6)) {
            std::cout << "✓ Test 3 passed: Single period PV" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 3 failed: Expected " << expected << ", got " << pva << std::endl;
        }
    }

    // Test 4: Zero periods — returns 0
    {
        tests_total++;
        double pva = annuity_present_value(100.0, 0.05, 0);
        if (almost_equal(pva, 0.0, 1e-9)) {
            std::cout << "✓ Test 4 passed: Zero periods returns 0" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 4 failed: Expected 0, got " << pva << std::endl;
        }
    }

    // Test 5: Negative PMT symmetry
    {
        tests_total++;
        double pva_pos = annuity_present_value( 100.0, 0.05, 5);
        double pva_neg = annuity_present_value(-100.0, 0.05, 5);
        if (almost_equal(pva_pos, -pva_neg, 1e-6)) {
            std::cout << "✓ Test 5 passed: Negative PMT symmetry" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 5 failed: Symmetry broken" << std::endl;
        }
    }

    // -------------------------------------------------------------------------
    // annuity_future_value
    // FV = P * [(1+r)^n - 1] / r
    // -------------------------------------------------------------------------

    // Test 6: Basic FVA — PMT=100, r=0.05, n=5 → ≈ 552.563
    {
        tests_total++;
        double fva      = annuity_future_value(100.0, 0.05, 5);
        double expected = 552.563;
        if (almost_equal(fva, expected, 0.001)) {
            std::cout << "✓ Test 6 passed: Basic annuity_future_value" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 6 failed: Expected ~" << expected << ", got " << fva << std::endl;
        }
    }

    // Test 7: FVA == PVA * (1+r)^n  (consistency)
    {
        tests_total++;
        double pva          = annuity_present_value(100.0, 0.05, 5);
        double fva          = annuity_future_value(100.0, 0.05, 5);
        double fva_from_pva = pva * std::pow(1.05, 5);
        if (almost_equal(fva, fva_from_pva, 1e-4)) {
            std::cout << "✓ Test 7 passed: FVA == PVA * (1+r)^n" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 7 failed: FVA=" << fva
                      << " vs PVA*(1+r)^n=" << fva_from_pva << std::endl;
        }
    }

    
    // Test 8: Zero rate FVA == PMT * n
    {
        tests_total++;
        double fva = annuity_future_value(100.0, 0.0, 10);
        if (almost_equal(fva, 1000.0, 1e-4)) {
            std::cout << "✓ Test 8 passed: Zero rate FVA == PMT * n" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 8 failed: Expected 1000.0, got " << fva << std::endl;
        }
    }

    // -------------------------------------------------------------------------
    // annuity_due_present_value
    // PV_due = PV_ordinary * (1 + r)
    // -------------------------------------------------------------------------

    // Test 9: PV due == ordinary PV * (1+r)
    {
        tests_total++;
        double pva_ordinary = annuity_present_value(100.0, 0.05, 5);
        double pva_due      = annuity_due_present_value(100.0, 0.05, 5);
        if (almost_equal(pva_due, pva_ordinary * 1.05, 1e-4)) {
            std::cout << "✓ Test 9 passed: PV due == PV ordinary * (1+r)" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 9 failed: Expected " << pva_ordinary * 1.05
                      << ", got " << pva_due << std::endl;
        }
    }

    // Test 10: PV due > PV ordinary (earlier payments are worth more)
    {
        tests_total++;
        double pva_ord = annuity_present_value(100.0, 0.05, 5);
        double pva_due = annuity_due_present_value(100.0, 0.05, 5);
        if (pva_due > pva_ord) {
            std::cout << "✓ Test 10 passed: PV due > PV ordinary" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 10 failed: Expected pva_due > pva_ord" << std::endl;
        }
    }

    // -------------------------------------------------------------------------
    // annuity_due_future_value
    // FV_due = FV_ordinary * (1 + r)
    // -------------------------------------------------------------------------

    // Test 11: FV due == ordinary FV * (1+r)
    {
        tests_total++;
        double fva_ordinary = annuity_future_value(100.0, 0.05, 5);
        double fva_due      = annuity_due_future_value(100.0, 0.05, 5);
        if (almost_equal(fva_due, fva_ordinary * 1.05, 1e-4)) {
            std::cout << "✓ Test 11 passed: FV due == FV ordinary * (1+r)" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 11 failed: Expected " << fva_ordinary * 1.05
                      << ", got " << fva_due << std::endl;
        }
    }

    // Test 12: Zero rate FV due == PMT * n  (same edge case as ordinary)
    {
        tests_total++;
        double fva_due = annuity_due_future_value(100.0, 0.0, 10);
        if (almost_equal(fva_due, 1000.0, 1e-4)) {
            std::cout << "✓ Test 12 passed: Zero rate FV due == PMT * n" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 12 failed: Expected 1000.0, got " << fva_due << std::endl;
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
