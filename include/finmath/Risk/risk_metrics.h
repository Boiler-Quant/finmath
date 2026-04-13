#ifndef FINMATH_RISK_RISK_METRICS_H
#define FINMATH_RISK_RISK_METRICS_H

#include <vector>

// Historical VaR: left-tail alpha (e.g. 0.05); returns positive number = loss magnitude at that quantile
// when losses are -returns (typical sign convention for long P&L returns).
double historical_var(const std::vector<double>& returns, double alpha);

// Expected shortfall / CVaR: mean of returns in the worst alpha fraction (sorted ascending).
double historical_cvar(const std::vector<double>& returns, double alpha);

// Gaussian VaR at alpha (left tail), given mean and std dev of returns.
double parametric_var(double mean, double std_dev, double alpha);

#endif  // FINMATH_RISK_RISK_METRICS_H
