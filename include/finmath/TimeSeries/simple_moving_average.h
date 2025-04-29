#ifndef SIMPLE_MOVING_AVERAGE_H
#define SIMPLE_MOVING_AVERAGE_H

#include <vector>
#include <pybind11/numpy.h>

namespace py = pybind11;

// Function to compute the moving average from a time series
std::vector<double> simple_moving_average(const std::vector<double> &data, size_t window_size);

// NumPy overload
std::vector<double> simple_moving_average_np(py::array_t<double> data_arr, size_t window_size);

#endif // MOVING_AVERAGE_H
