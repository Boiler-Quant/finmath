#include <cassert>
#include <cmath>
#include <iostream>
#include "finmath/finmath.h"

// Helper Function
bool almost_equal(double a, double b, double tolerance)
{
    return std::abs(a - b) <= tolerance * std::max(std::abs(a), std::abs(b));
}

int bond_pricing_tests()
{
    double expected = 0.0;
    double tolerance = 0.001;

    //test different periods (annually, semiannually, monthly)
    {
        double result = bond_price(10000, 0.05, 0.06, 1, 30);
        expected = 8623.5169;
        assert(almost_equal(result, expected, tolerance));
    }

    {
        double result = bond_price(10000, 0.05, 0.06, 2, 30);
        expected = 8616.2218;
        assert(almost_equal(result, expected, tolerance));
    }

    {
        double result = bond_price(10000, 0.05, 0.06, 12, 30);
        expected = 8610.0699;
        assert(almost_equal(result, expected, tolerance));
    }

    std::cout << "Bond Pricing Tests Passed!" << std::endl;
    return 0;
}

int main()
{
    return bond_pricing_tests();
}
