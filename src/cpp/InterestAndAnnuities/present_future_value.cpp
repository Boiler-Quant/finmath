#include "finmath/InterestAndAnnuities/present_future_value.h"
#include <cmath>
#include <stdexcept>

double present_value(double future_value, double rate, double time) {
    return future_value / std::pow(1.0 + rate, time);
}

double future_value(double present_value, double rate, double time) {
    return present_value * std::pow(1.0 + rate, time);
}

double present_value_continuous(double future_value, double rate, double time) {
    return future_value * std::exp(-rate * time);
}

double future_value_continuous(double future_value, double rate, double time) {
    return future_value * std::exp(rate * time);
}