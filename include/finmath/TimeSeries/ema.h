#ifndef EMA_H
#define EMA_H

#include <vector>

// Function to compute the Exponential Moving Average (EMA) using window size
std::vector<double> compute_ema(const std::vector<double>& prices, size_t window);

// Function to compute the Exponential Moving Average (EMA) using a smoothing factor
std::vector<double> compute_ema_with_smoothing(const std::vector<double>& prices, double smoothing_factor);

#endif // EMA_H
