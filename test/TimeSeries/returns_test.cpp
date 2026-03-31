#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#include "finmath/TimeSeries/returns.h"

static bool almost_equal(double a, double b, double tol)
{
    return std::abs(a - b) <= tol * std::max(std::abs(a), std::abs(b));
}

int main()
{
    const double tol = 1e-9;

    // pct_returns: simple known ratios
    {
        std::vector<double> prices{100.0, 110.0, 99.0};
        auto r = pct_returns(prices);
        assert(r.size() == prices.size());
        assert(r[0] == 0.0);
        assert(almost_equal(r[1], 0.10, tol));
        assert(almost_equal(r[2], -0.10, tol));
    }

    // log_returns: exp/log identity checks
    {
        std::vector<double> prices{100.0, 110.0, 99.0};
        auto r = log_returns(prices);
        assert(r.size() == prices.size());
        assert(r[0] == 0.0);
        assert(almost_equal(std::exp(r[1]), prices[1] / prices[0], tol));
        assert(almost_equal(std::exp(r[2]), prices[2] / prices[1], tol));
    }

    std::cout << "Returns Tests Pass!" << std::endl;
    return 0;
}
