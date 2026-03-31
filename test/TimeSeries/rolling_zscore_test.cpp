#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#include "finmath/TimeSeries/rolling_zscore.h"

static bool almost_equal(double a, double b, double tol)
{
    return std::abs(a - b) <= tol * std::max(std::abs(a), std::abs(b));
}

int main()
{
    const double tol = 1e-6;

    // Constant series => std=0 => zscore 0
    {
        std::vector<double> x{5.0, 5.0, 5.0, 5.0};
        auto z = rolling_zscore(3, x);
        assert(z.size() == x.size());
        assert(z[0] == 0.0);
        assert(z[1] == 0.0);
        assert(z[2] == 0.0);
        assert(z[3] == 0.0);
    }

    // Increasing series: validate last point against computed mean/std of last window
    {
        std::vector<double> x{1.0, 2.0, 3.0, 4.0, 5.0};
        const size_t w = 3;
        auto z = rolling_zscore(w, x);
        assert(z.size() == x.size());

        const double mean = (3.0 + 4.0 + 5.0) / 3.0;
        const double ex2 = (3.0 * 3.0 + 4.0 * 4.0 + 5.0 * 5.0) / 3.0;
        const double var = ex2 - mean * mean;
        const double std = std::sqrt(var);
        const double expected = (5.0 - mean) / std;
        assert(almost_equal(z.back(), expected, tol));
    }

    // rolling_mean: last value check
    {
        std::vector<double> x{1.0, 2.0, 3.0, 4.0, 5.0};
        auto m = rolling_mean(2, x);
        assert(m.size() == x.size());
        assert(almost_equal(m.back(), 4.5, tol));
    }

    std::cout << "Rolling Z-Score Tests Pass!" << std::endl;
    return 0;
}
