#include "finmath/FixedIncome/bond_pricing.h"
#include <cmath>
#include <stdexcept>

#include <iostream>

double bond_price(
    double face_value,
    double coupon_rate,
    double yield_to_maturity,
    int periods,
    double time_to_maturity
) {
    double c = coupon_rate * face_value / periods;
    double d = periods / (periods + yield_to_maturity);
    double n = time_to_maturity * periods;
    return (1 - pow(d, n)) * c * periods / yield_to_maturity + face_value * pow(d, n);
}



double bond_yield(
    double face_value,
    double coupon_rate,
    double price,
    int periods,
    double time_to_maturity
) {
    //TODO: directly doing newton's method is very ill conditioned
    //impl below theoretically correct but doesnt work
    double n = time_to_maturity * periods;
    double c = coupon_rate * face_value / periods;
    double yield = 0.1;
    double p = bond_price(face_value, coupon_rate, yield, periods, time_to_maturity);
    int max_iterations = 100;
    while (abs(p - price) > 1e-6) {
        double d = periods / (periods + yield);
        double dp = -pow(d, n) * face_value * n / (periods + yield);
        dp += (pow(d, n) * n * yield / (periods + yield) + pow(d, n) - 1) * c * periods / (yield * yield);
        yield -= p / dp;
        
        p = bond_price(face_value, coupon_rate, yield, periods, time_to_maturity);
        --max_iterations;
        if (max_iterations < 0)
            throw std::runtime_error("convergence failed");
    }

    return yield;
}

double bond_duration(
    double face_value,
    double coupon_rate,
    double yield_to_maturity,
    int periods,
    double time_to_maturity
) {
    double d = 1.0;
    double r = 1.0 + yield_to_maturity / periods;
    double price = 0;

    for (int i = 0; i < periods * time_to_maturity; ++i) {
        d *= r;
        price += (i + 1) / d;
    }
    price *= coupon_rate * face_value / periods;
    price += periods * time_to_maturity * face_value / d;

    return price / bond_price(face_value, coupon_rate, yield_to_maturity, periods, time_to_maturity);
}
