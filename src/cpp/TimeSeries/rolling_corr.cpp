#include "finmath/TimeSeries/rolling_corr.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

static void validate(size_t window_size, const std::vector<double>& x, const std::vector<double>& y)
{
    if (window_size == 0) {
        throw std::invalid_argument("Window size cannot be zero.");
    }
    if (x.size() != y.size()) {
        throw std::invalid_argument("Inputs must have the same length.");
    }
}

std::vector<double> rolling_cov(size_t window_size, const std::vector<double>& x, const std::vector<double>& y)
{
    validate(window_size, x, y);
    std::vector<double> out(x.size(), 0.0);
    if (x.empty()) {
        return out;
    }
    if (window_size > x.size()) {
        window_size = x.size();
    }

    double sx = 0.0, sy = 0.0, sxy = 0.0;
    for (size_t i = 0; i < window_size; ++i) {
        sx += x[i];
        sy += y[i];
        sxy += x[i] * y[i];
    }

    auto emit = [&](size_t idx) {
        const double n = static_cast<double>(window_size);
        const double mx = sx / n;
        const double my = sy / n;
        out[idx] = (sxy / n) - (mx * my);
    };

    emit(window_size - 1);
    for (size_t i = window_size; i < x.size(); ++i) {
        const double xo = x[i - window_size];
        const double yo = y[i - window_size];
        sx += x[i] - xo;
        sy += y[i] - yo;
        sxy += x[i] * y[i] - xo * yo;
        emit(i);
    }

    return out;
}

std::vector<double> rolling_corr(size_t window_size, const std::vector<double>& x, const std::vector<double>& y)
{
    validate(window_size, x, y);
    std::vector<double> out(x.size(), 0.0);
    if (x.empty()) {
        return out;
    }
    if (window_size > x.size()) {
        window_size = x.size();
    }

    double sx = 0.0, sy = 0.0, sxx = 0.0, syy = 0.0, sxy = 0.0;
    for (size_t i = 0; i < window_size; ++i) {
        sx += x[i];
        sy += y[i];
        sxx += x[i] * x[i];
        syy += y[i] * y[i];
        sxy += x[i] * y[i];
    }

    auto emit = [&](size_t idx) {
        const double n = static_cast<double>(window_size);
        const double mx = sx / n;
        const double my = sy / n;
        double vx = (sxx / n) - (mx * mx);
        double vy = (syy / n) - (my * my);
        double cov = (sxy / n) - (mx * my);
        if (vx < 0.0) {
            vx = 0.0;
        }
        if (vy < 0.0) {
            vy = 0.0;
        }
        const double denom = std::sqrt(vx) * std::sqrt(vy);
        if (denom == 0.0) {
            out[idx] = 0.0;
        } else {
            out[idx] = cov / denom;
        }
    };

    emit(window_size - 1);
    for (size_t i = window_size; i < x.size(); ++i) {
        const double xo = x[i - window_size];
        const double yo = y[i - window_size];
        sx += x[i] - xo;
        sy += y[i] - yo;
        sxx += x[i] * x[i] - xo * xo;
        syy += y[i] * y[i] - yo * yo;
        sxy += x[i] * y[i] - xo * yo;
        emit(i);
    }

    return out;
}

std::vector<double> rolling_beta(size_t window_size, const std::vector<double>& y, const std::vector<double>& x)
{
    validate(window_size, x, y);
    std::vector<double> out(x.size(), 0.0);
    if (x.empty()) {
        return out;
    }
    if (window_size > x.size()) {
        window_size = x.size();
    }

    double sx = 0.0, sy = 0.0, sxx = 0.0, sxy = 0.0;
    for (size_t i = 0; i < window_size; ++i) {
        sx += x[i];
        sy += y[i];
        sxx += x[i] * x[i];
        sxy += x[i] * y[i];
    }

    auto emit = [&](size_t idx) {
        const double n = static_cast<double>(window_size);
        const double mx = sx / n;
        const double my = sy / n;
        double vx = (sxx / n) - (mx * mx);
        if (vx < 0.0) {
            vx = 0.0;
        }
        const double cov = (sxy / n) - (mx * my);
        if (vx == 0.0) {
            out[idx] = 0.0;
        } else {
            out[idx] = cov / vx;
        }
    };

    emit(window_size - 1);
    for (size_t i = window_size; i < x.size(); ++i) {
        const double xo = x[i - window_size];
        const double yo = y[i - window_size];
        sx += x[i] - xo;
        sy += y[i] - yo;
        sxx += x[i] * x[i] - xo * xo;
        sxy += x[i] * y[i] - xo * yo;
        emit(i);
    }

    return out;
}
