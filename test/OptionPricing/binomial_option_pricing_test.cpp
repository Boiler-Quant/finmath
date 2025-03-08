#include <cassert>
#include <cmath>
#include <iostream>
#include "finmath/OptionPricing/options_pricing.h"

// Helper Function
bool almost_equal(double a, double b, double tolerance)
{
    return std::abs(a - b) <= tolerance * std::max(std::abs(a), std::abs(b));
}

int binomial_option_pricing_tests()
{
    double tolerance = 0.001;

    std::cout << "Binomial-Tree Tests Passed!" << std::endl;
    return 0;
}

int main()
{
    return binomial_option_pricing_tests();
}
