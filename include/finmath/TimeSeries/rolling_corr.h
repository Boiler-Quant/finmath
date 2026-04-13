#ifndef FINMATH_TIME_SERIES_ROLLING_CORR_H
#define FINMATH_TIME_SERIES_ROLLING_CORR_H

#include <vector>

std::vector<double> rolling_cov(size_t window_size, const std::vector<double>& x, const std::vector<double>& y);

std::vector<double> rolling_corr(size_t window_size, const std::vector<double>& x, const std::vector<double>& y);

// OLS beta of y on x within each window: cov(x,y)/var(x).
std::vector<double> rolling_beta(size_t window_size, const std::vector<double>& y, const std::vector<double>& x);

#endif  // FINMATH_TIME_SERIES_ROLLING_CORR_H
