#ifndef FINMATH_TIME_SERIES_RETURNS_H
#define FINMATH_TIME_SERIES_RETURNS_H

#include <vector>

// Index 0 is 0; index i is return from prices[i-1] to prices[i].
std::vector<double> log_returns(const std::vector<double>& prices);
std::vector<double> pct_returns(const std::vector<double>& prices);

#endif  // FINMATH_TIME_SERIES_RETURNS_H
