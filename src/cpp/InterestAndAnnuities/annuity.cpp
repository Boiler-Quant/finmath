#include "finmath/InterestAndAnnuities/annuity.h"


/**
 * Present value of ordinary annuity
 * Payments are made at the END of each period
 *
 * @param payment Payment amount per period
 * @param rate Interest rate per period (not annualized if periods are not annual)
 * @param periods Number of payment periods
 * @return Present value of the annuity
 *
 * Formula: PV = P * [1 - (1+r)^(-n)] / r
 *
 * Edge cases:
 * - If rate == 0: return payment * periods
 * - If periods == 0: return 0
 */

double annuity_present_value(double payment, double rate, int periods) {
    if (rate < 0.0) {
        throw std::invalid_argument("rate must be non-negative");
    }
    if (periods < 0.0) {
        throw std::invalid_argument("periods must be non-negative");
    }
    if (rate == 0.0) {
        return payment * periods;
    }
    if (periods == 0.0) {
        return 0.0;
    }
    return payment * (1 - std::pow(1.0 + rate, -periods)) / rate;
}

/**
 * Future value of ordinary annuity
 *
 * @param payment Payment amount per period
 * @param rate Interest rate per period
 * @param periods Number of payment periods
 * @return Future value of the annuity
 *
 * Formula: FV = P * [(1+r)^n - 1] / r
 */

double annuity_future_value(double payment, double rate, int periods) {
    if (rate < 0.0) {
        throw std::invalid_argument("rate must be non-negative");
    }
    if (periods < 0.0) {
        throw std::invalid_argument("periods must be non-negative");
    }
    if (rate == 0.0) {
        return payment * periods;
    }

    return payment * (std::pow(1.0 + rate, periods) - 1) / rate;
}

/**
 * Present value of annuity due
 * Payments are made at the BEGINNING of each period
 *
 * @param payment Payment amount per period
 * @param rate Interest rate per period
 * @param periods Number of payment periods
 * @return Present value of annuity due
 *
 * Formula: PV = P * [1 - (1+r)^(-n)] / r * (1 + r)
 */

double annuity_due_present_value(double payment, double rate, int periods) {
    if (rate < 0.0) {
        throw std::invalid_argument("rate must be non-negative");
    }
    if (periods < 0.0) {
        throw std::invalid_argument("periods must be non-negative");
    }

    return payment * (1 - std::pow(1.0 + rate, -periods)) / rate * (1.0 + rate);
}

/**
 * Future value of annuity due
 *
 * @param payment Payment amount per period
 * @param rate Interest rate per period
 * @param periods Number of payment periods
 * @return Future value of annuity due
 *
 * Formula: FV = P * [(1+r)^n - 1] / r * (1 + r)
 */

double annuity_due_future_value(double payment, double rate, int periods) {
    if (rate < 0.0) {
        throw std::invalid_argument("rate must be non-negative");
    }
    if (periods < 0.0) {
        throw std::invalid_argument("periods must be non-negative");
    }
    if (rate == 0.0) {
        return payment * periods;
    }

    return payment * (std::pow(1.0 + rate, periods) - 1) / rate * (1.0 + rate);
}

