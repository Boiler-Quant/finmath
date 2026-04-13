#ifndef FINMATH_TIME_SERIES_AUTOCORRELATION_H
#define FINMATH_TIME_SERIES_AUTOCORRELATION_H

#include <vector>

// Sample autocorrelation at lags 0, 1, ..., max_lag (inclusive). Denominator uses sum of squared
// deviations (standard ACF definition).
std::vector<double> autocorrelation(const std::vector<double>& values, int max_lag);

#endif  // FINMATH_TIME_SERIES_AUTOCORRELATION_H
