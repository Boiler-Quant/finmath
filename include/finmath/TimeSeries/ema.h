#ifndef EMA_H
#define EMA_H

#include <vector>
#include <pybind11/numpy.h>

namespace py = pybind11;

// Function to compute the Exponential Moving Average (EMA) using window size
std::vector<double> compute_ema(const std::vector<double> &prices, size_t window);

// Function to compute the Exponential Moving Average (EMA) using a smoothing factor
std::vector<double> compute_ema_with_smoothing(const std::vector<double> &prices, double smoothing_factor);

// NumPy overloads
std::vector<double> compute_ema_np(py::array_t<double> prices_arr, size_t window);
std::vector<double> compute_ema_with_smoothing_np(py::array_t<double> prices_arr, double smoothing_factor);

#endif // EMA_H
