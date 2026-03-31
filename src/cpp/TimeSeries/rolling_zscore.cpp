#include "finmath/TimeSeries/rolling_zscore.h"

#include <pybind11/numpy.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace py = pybind11;

std::vector<double> rolling_mean(size_t window_size, const std::vector<double>& data)
{
    if (window_size == 0) {
        throw std::invalid_argument("Window size cannot be zero.");
    }

    std::vector<double> result(data.size(), 0.0);
    if (data.empty()) {
        return result;
    }

    if (window_size > data.size()) {
        window_size = data.size();
    }

    double sum = 0.0;
    for (size_t i = 0; i < window_size; ++i) {
        sum += data[i];
    }
    result[window_size - 1] = sum / static_cast<double>(window_size);

    for (size_t i = window_size; i < data.size(); ++i) {
        sum += data[i] - data[i - window_size];
        result[i] = sum / static_cast<double>(window_size);
    }

    return result;
}

std::vector<double> rolling_zscore(size_t window_size, const std::vector<double>& data)
{
    if (window_size == 0) {
        throw std::invalid_argument("Window size cannot be zero.");
    }

    std::vector<double> result(data.size(), 0.0);
    if (data.empty()) {
        return result;
    }

    if (window_size > data.size()) {
        window_size = data.size();
    }

    double sum = 0.0;
    double sumsq = 0.0;
    for (size_t i = 0; i < window_size; ++i) {
        sum += data[i];
        sumsq += data[i] * data[i];
    }

    auto write = [&](size_t idx) {
        const double mean = sum / static_cast<double>(window_size);
        const double ex2 = sumsq / static_cast<double>(window_size);
        double var = ex2 - mean * mean;
        if (var < 0.0) {
            var = 0.0;
        }
        const double std = std::sqrt(var);
        if (std == 0.0) {
            result[idx] = 0.0;
            return;
        }
        result[idx] = (data[idx] - mean) / std;
    };

    write(window_size - 1);
    for (size_t i = window_size; i < data.size(); ++i) {
        const double out = data[i - window_size];
        const double in = data[i];
        sum += in - out;
        sumsq += in * in - out * out;
        write(i);
    }

    return result;
}

std::vector<double> rolling_mean_np(py::array_t<double> data_arr, size_t window_size)
{
    py::buffer_info buf_info = data_arr.request();
    if (buf_info.ndim != 1) {
        throw std::runtime_error("Input array must be 1-dimensional.");
    }

    const size_t n = buf_info.size;
    const double* ptr = static_cast<const double*>(buf_info.ptr);

    std::vector<double> data(ptr, ptr + n);
    return rolling_mean(window_size, data);
}

std::vector<double> rolling_zscore_np(py::array_t<double> data_arr, size_t window_size)
{
    py::buffer_info buf_info = data_arr.request();
    if (buf_info.ndim != 1) {
        throw std::runtime_error("Input array must be 1-dimensional.");
    }

    const size_t n = buf_info.size;
    const double* ptr = static_cast<const double*>(buf_info.ptr);

    std::vector<double> data(ptr, ptr + n);
    return rolling_zscore(window_size, data);
}
