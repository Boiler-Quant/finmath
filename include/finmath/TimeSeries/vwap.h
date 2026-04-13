#ifndef FINMATH_TIME_SERIES_VWAP_H
#define FINMATH_TIME_SERIES_VWAP_H

#include <vector>

// Sliding-window VWAP; length is n - window_size + 1 (first full window at index 0 of output).
std::vector<double> vwap(const std::vector<double>& prices, const std::vector<double>& volumes, size_t window_size);

std::vector<double> rolling_dollar_volume(const std::vector<double>& prices, const std::vector<double>& volumes,
                                          size_t window_size);

#endif  // FINMATH_TIME_SERIES_VWAP_H
