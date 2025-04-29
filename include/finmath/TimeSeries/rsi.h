#ifndef RSI_H
#define RSI_H

#include <vector>
#include <pybind11/numpy.h>

namespace py = pybind11;

// function to compute the average gain over a window
double compute_avg_gain(const std::vector<double> &price_changes, size_t window_size);

// Function to compute the average loss over a window
double compute_avg_loss(const std::vector<double> &price_changes, size_t window_size);

// Function to compute the RSI from a time series of prices
std::vector<double> compute_smoothed_rsi(const std::vector<double> &prices, size_t window_size);

// NumPy overload
std::vector<double> compute_smoothed_rsi_np(py::array_t<double> prices_arr, size_t window_size);

#endif // RSI_H
