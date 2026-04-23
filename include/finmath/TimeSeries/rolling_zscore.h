#ifndef FINMATH_TIME_SERIES_ROLLING_ZSCORE_H
#define FINMATH_TIME_SERIES_ROLLING_ZSCORE_H

#include <vector>

std::vector<double> rolling_mean(size_t window_size, const std::vector<double>& data);
std::vector<double> rolling_zscore(size_t window_size, const std::vector<double>& data);

#endif // FINMATH_TIME_SERIES_ROLLING_ZSCORE_H
