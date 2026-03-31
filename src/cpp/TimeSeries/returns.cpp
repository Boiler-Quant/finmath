#include "finmath/TimeSeries/returns.h"

#include <pybind11/numpy.h>

#include <cmath>
#include <stdexcept>

namespace py = pybind11;

std::vector<double> log_returns(const std::vector<double>& prices)
{
    if (prices.empty()) {
        return {};
    }

    std::vector<double> out(prices.size(), 0.0);
    for (size_t i = 1; i < prices.size(); ++i) {
        const double prev = prices[i - 1];
        const double curr = prices[i];
        if (prev <= 0.0 || curr <= 0.0) {
            throw std::invalid_argument("log_returns requires all prices > 0");
        }
        out[i] = std::log(curr / prev);
    }
    return out;
}

std::vector<double> pct_returns(const std::vector<double>& prices)
{
    if (prices.empty()) {
        return {};
    }

    std::vector<double> out(prices.size(), 0.0);
    for (size_t i = 1; i < prices.size(); ++i) {
        const double prev = prices[i - 1];
        const double curr = prices[i];
        if (prev == 0.0) {
            throw std::invalid_argument("pct_returns requires previous price != 0");
        }
        out[i] = (curr / prev) - 1.0;
    }
    return out;
}

std::vector<double> log_returns_np(py::array_t<double> prices_arr)
{
    py::buffer_info buf_info = prices_arr.request();
    if (buf_info.ndim != 1) {
        throw std::runtime_error("Input array must be 1-dimensional.");
    }

    const size_t n = buf_info.size;
    const double* prices_ptr = static_cast<const double*>(buf_info.ptr);

    if (n == 0) {
        return {};
    }

    std::vector<double> out(n, 0.0);
    for (size_t i = 1; i < n; ++i) {
        const double prev = prices_ptr[i - 1];
        const double curr = prices_ptr[i];
        if (prev <= 0.0 || curr <= 0.0) {
            throw std::runtime_error("log_returns requires all prices > 0");
        }
        out[i] = std::log(curr / prev);
    }
    return out;
}

std::vector<double> pct_returns_np(py::array_t<double> prices_arr)
{
    py::buffer_info buf_info = prices_arr.request();
    if (buf_info.ndim != 1) {
        throw std::runtime_error("Input array must be 1-dimensional.");
    }

    const size_t n = buf_info.size;
    const double* prices_ptr = static_cast<const double*>(buf_info.ptr);

    if (n == 0) {
        return {};
    }

    std::vector<double> out(n, 0.0);
    for (size_t i = 1; i < n; ++i) {
        const double prev = prices_ptr[i - 1];
        const double curr = prices_ptr[i];
        if (prev == 0.0) {
            throw std::runtime_error("pct_returns requires previous price != 0");
        }
        out[i] = (curr / prev) - 1.0;
    }
    return out;
}
