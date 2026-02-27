// Profiling harness: runs rolling_volatility (scalar, vector API) in a tight loop
// so perf/gprof can attribute time to the implementation.
// Build with the main library; run with: ./RollingVolatilityProfile_harness
// For SIMD path use: python profiling/run_rolling_volatility_bench.py --simd

#include "finmath/TimeSeries/rolling_volatility.h"
#include <vector>
#include <cstdlib>
#include <iostream>

int main() {
    const size_t num_prices = 100000;
    const size_t window = 252;
    const int iterations = 50;

    std::vector<double> prices(num_prices);
    for (size_t i = 0; i < num_prices; ++i) {
        prices[i] = 100.0 + 0.01 * (i % 1000);  // positive, non-constant
    }

    volatile double sink = 0;
    for (int i = 0; i < iterations; ++i) {
        auto result = rolling_volatility(prices, window);
        sink += result.empty() ? 0 : result.back();
    }
    std::cout << "rolling_volatility (scalar): " << iterations << " iterations, "
              << num_prices << " prices, window " << window << " (sink=" << sink << ")\n";
    return 0;
}
