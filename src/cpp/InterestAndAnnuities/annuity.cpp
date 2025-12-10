#include "finmath/InterestAndAnnuities/annuity.h"
#include <cmath>

double annuity_present_value(double payment, double rate, int periods) {
    if (rate == 0)
        return payment * periods;
    if (periods == 0)
        return 0;
    return payment * (1 - pow(1 + rate, -periods)) / rate;
}

double annuity_future_value(double payment, double rate, int periods) {
    if (rate == 0)
        return payment * periods;
    if (periods == 0)
        return 0;
    return payment * (pow(1 + rate, periods) - 1) / rate;
}

double annuity_due_present_value(double payment, double rate, int periods) {
    return annuity_present_value(payment, rate, periods) * (1 + rate);
}

double annuity_due_future_value(double payment, double rate, int periods) {
    return annuity_future_value(payment, rate, periods) * (1 + rate);
}