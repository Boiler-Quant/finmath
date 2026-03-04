#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "finmath/FixedIncome/bond_pricing.h"

bool almost_equal(double a, double b, double tolerance = 1e-5) {
    return std::abs(a - b) < tolerance;
}

int main() {
    int tests_passed = 0;
    int tests_total  = 0;

    // -------------------------------------------------------------------------
    // bond_price
    // -------------------------------------------------------------------------

    // Test 1: Par bond — when coupon rate == YTM, price == face value
    {
        tests_total++;
        double price = bond_price(1000.0, 0.05, 0.05, 2, 10.0);
        if (almost_equal(price, 1000.0, 1e-4)) {
            std::cout << "✓ Test 1 passed: Par bond (coupon rate == YTM)" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 1 failed: Expected 1000.0, got " << price << std::endl;
        }
    }

    // Test 2: Discount bond — YTM > coupon rate, price < face value
    {
        tests_total++;
        double price = bond_price(1000.0, 0.05, 0.06, 2, 10.0);
        if (price < 1000.0) {
            std::cout << "✓ Test 2 passed: Discount bond (price < face)" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 2 failed: Expected price < 1000.0, got " << price << std::endl;
        }
    }

    // Test 3: Premium bond — YTM < coupon rate, price > face value
    {
        tests_total++;
        double price = bond_price(1000.0, 0.05, 0.04, 2, 10.0);
        if (price > 1000.0) {
            std::cout << "✓ Test 3 passed: Premium bond (price > face)" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 3 failed: Expected price > 1000.0, got " << price << std::endl;
        }
    }

    // Test 4: Known value — 10yr, $1000 face, 5% semi-annual coupon, 4% YTM
    // Expected ≈ $1081.76
    {
        tests_total++;
        double price    = bond_price(1000.0, 0.05, 0.04, 2, 10.0);
        double expected = 1081.76;
        if (almost_equal(price, expected, 0.01)) {
            std::cout << "✓ Test 4 passed: Known semi-annual bond price" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 4 failed: Expected ~" << expected << ", got " << price << std::endl;
        }
    }

    // Test 5: Annual coupon bond
    {
        tests_total++;
        // 5yr, $1000 face, 6% annual coupon, 6% YTM → par
        double price = bond_price(1000.0, 0.06, 0.06, 1, 5.0);
        if (almost_equal(price, 1000.0, 1e-4)) {
            std::cout << "✓ Test 5 passed: Annual coupon par bond" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 5 failed: Expected 1000.0, got " << price << std::endl;
        }
    }

    // Test 6: Zero coupon bond — price == face / (1+r)^n
    {
        tests_total++;
        double price    = bond_price(1000.0, 0.0, 0.05, 1, 5.0);
        double expected = 1000.0 / std::pow(1.05, 5);
        if (almost_equal(price, expected, 1e-4)) {
            std::cout << "✓ Test 6 passed: Zero coupon bond" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 6 failed: Expected " << expected << ", got " << price << std::endl;
        }
    }

    // -------------------------------------------------------------------------
    // bond_yield  (inverse of bond_price)
    // -------------------------------------------------------------------------

    // Test 7: Round-trip — yield(price(ytm)) == ytm
    {
        tests_total++;
        double ytm   = 0.04;
        double price = bond_price(1000.0, 0.05, ytm, 2, 10.0);
        double ytm2  = bond_yield(1000.0, 0.05, price, 2, 10.0);
        if (almost_equal(ytm2, ytm, 1e-5)) {
            std::cout << "✓ Test 7 passed: bond_yield round-trip" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 7 failed: Expected " << ytm << ", got " << ytm2 << std::endl;
        }
    }

    // Test 8: Par bond yield == coupon rate
    {
        tests_total++;
        double ytm = bond_yield(1000.0, 0.05, 1000.0, 2, 10.0);
        if (almost_equal(ytm, 0.05, 1e-5)) {
            std::cout << "✓ Test 8 passed: Par bond yield == coupon rate" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 8 failed: Expected 0.05, got " << ytm << std::endl;
        }
    }

    // Test 9: Discount bond — high price → lower yield than coupon
    {
        tests_total++;
        double price = bond_price(1000.0, 0.05, 0.06, 2, 10.0);
        double ytm   = bond_yield(1000.0, 0.05, price, 2, 10.0);
        if (almost_equal(ytm, 0.06, 1e-5)) {
            std::cout << "✓ Test 9 passed: Discount bond yield recovery" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 9 failed: Expected 0.06, got " << ytm << std::endl;
        }
    }

    // -------------------------------------------------------------------------
    // bond_duration
    // -------------------------------------------------------------------------

    // Test 10: Zero-coupon bond — duration == time to maturity
    {
        tests_total++;
        double dur = bond_duration(1000.0, 0.0, 0.05, 1, 5.0);
        if (almost_equal(dur, 5.0, 1e-4)) {
            std::cout << "✓ Test 10 passed: Zero-coupon duration == maturity" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 10 failed: Expected 5.0, got " << dur << std::endl;
        }
    }

    // Test 11: Coupon bond — duration < time to maturity
    {
        tests_total++;
        double dur = bond_duration(1000.0, 0.05, 0.05, 2, 10.0);
        if (dur < 10.0 && dur > 0.0) {
            std::cout << "✓ Test 11 passed: Coupon bond duration < maturity" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 11 failed: Expected 0 < dur < 10, got " << dur << std::endl;
        }
    }

    // Test 12: Higher coupon → shorter duration (more weight on early cash flows)
    {
        tests_total++;
        double dur_low  = bond_duration(1000.0, 0.02, 0.05, 2, 10.0);
        double dur_high = bond_duration(1000.0, 0.08, 0.05, 2, 10.0);
        if (dur_high < dur_low) {
            std::cout << "✓ Test 12 passed: Higher coupon → shorter duration" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 12 failed: Expected dur_high < dur_low" << std::endl;
        }
    }

    // Test 13: Known Macaulay duration — 10yr, 5% semi-annual, 5% YTM ≈ 7.99 years
    {
        tests_total++;
        double dur      = bond_duration(1000.0, 0.05, 0.05, 2, 10.0);
        double expected = 7.99;
        if (almost_equal(dur, expected, 0.02)) {
            std::cout << "✓ Test 13 passed: Known Macaulay duration value" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 13 failed: Expected ~" << expected << ", got " << dur << std::endl;
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
