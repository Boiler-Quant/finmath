#include "finmath/InterestAndAnnuities/discount_factor.h"
#include <cmath>
#include <stdexcept>

double discount_factor(double rate, double time) {
    // Input validation
    if (rate < 0) {
        throw std::invalid_argument("Interest rate cannot be negative");
    }
    if (time < 0) {
        throw std::invalid_argument("Time cannot be negative");
    }

    // Edge case: zero rate
    if (rate == 0.0) {
        return 1.0;
    }

    // Discrete compounding: DF = 1 / (1 + r)^t
    return 1.0 / std::pow(1.0 + rate, time);
}

double discount_factor_continuous(double rate, double time) {
    // Input validation
    if (rate < 0) {
        throw std::invalid_argument("Interest rate cannot be negative");
    }
    if (time < 0) {
        throw std::invalid_argument("Time cannot be negative");
    }

    // Continuous compounding: DF = e^(-r*t)
    return std::exp(-rate * time);
}

double future_value_factor(double rate, double time) {
    // Input validation
    if (rate < 0) {
        throw std::invalid_argument("Interest rate cannot be negative");
    }
    if (time < 0) {
        throw std::invalid_argument("Time cannot be negative");
    }

    // Edge case: zero rate
    if (rate == 0.0) {
        return 1.0;
    }

    // Future value factor: FVF = (1 + r)^t
    return std::pow(1.0 + rate, time);
}
