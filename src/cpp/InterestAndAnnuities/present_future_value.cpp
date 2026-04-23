#include "finmath/InterestAndAnnuities/present_future_value.h"

/**
 * Present value with discrete compounding
 * Calculates what a future amount is worth today
 *
 * @param future_value Amount to be received in the future
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years until the future value is received
 * @return Present value of the future amount
 *
 * Formula: PV = FV / (1 + r)^t
 */

double present_value(double future_value, double rate, double time) {
    if (rate < 0.0) {
        throw std::invalid_argument("Interest rate cannot be negative");
    }
    if (time < 0.0) {
        throw std::invalid_argument("Time cannot be negative");
    }
    return future_value / std::pow(1.0 + rate, time);
}

/**
 * Future value with discrete compounding
 * Calculates what a current amount will be worth in the future
 *
 * @param present_value Current amount
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Future value of the present amount
 *
 * Formula: FV = PV * (1 + r)^t
 */

double future_value(double present_value, double rate, double time) {
    if (rate < 0.0) {
        throw std::invalid_argument("Interest rate cannot be negative");
    }
    if (time < 0.0) {
        throw std::invalid_argument("Time cannot be negative");
    }
    return present_value * std::pow(1.0 + rate, time);
}

/**
 * Present value with continuous compounding
 * Uses exponential compounding
 *
 * @param future_value Amount to be received in the future
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Present value with continuous compounding
 *
 * Formula: PV = FV * e^(-r*t)
 */

double present_value_continuous(double future_value, double rate, double time) {
    if (rate < 0.0) {
        throw std::invalid_argument("Interest rate cannot be negative");
    }
    if (time < 0.0) {
        throw std::invalid_argument("Time cannot be negative");
    }
    return future_value * std::exp(-rate * time);
}

/**
 * Future value with continuous compounding
 *
 * @param present_value Current amount
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Future value with continuous compounding
 *
 * Formula: FV = PV * e^(r*t)
 */

double future_value_continuous(double present_value, double rate, double time) {
    if (rate < 0.0) {
        throw std::invalid_argument("Interest rate cannot be negative");
    }
    if (time < 0.0) {
        throw std::invalid_argument("Time cannot be negative");
    }
    return present_value * std::exp(rate * time);
}

