#include "finmath/InterestAndAnnuities/cash_flow.h"
#include <vector>
#include <cmath>
#include <stdexcept>

double net_present_value(
    const std::vector<double>& cash_flows,
    double rate,
    double initial_investment
) {
    double npv = -initial_investment;
    double r = 1.0;
    for (int i = 0; i < cash_flows.size(); ++i) {
        npv += cash_flows[i] / r;
        r *= 1 + rate;
    }
    return npv;
}

double internal_rate_of_return(
    const std::vector<double>& cash_flows,
    double initial_guess,
    int max_iterations,
    double tolerance
) {
    double rate = initial_guess;
    double npv = net_present_value(cash_flows, rate);

    while (abs(npv) > tolerance) {
        double dnpv = 0.0;
        double r = 1 + rate;
        for (int i = 1; i < cash_flows.size(); ++i) {
            r *= 1 + rate;
            dnpv -= cash_flows[i] / r * i;
        }
        rate -= npv / dnpv;
        npv = net_present_value(cash_flows, rate);
        --max_iterations;
        if (max_iterations < 0)
            throw std::runtime_error("convergence failed");
    }

    return rate;
}

int payback_period(
    const std::vector<double>& cash_flows,
    double initial_investment
) {
    double sum = cash_flows[0];
    int i = 0;
    while (sum < initial_investment) {
        ++i;
        if (i >= cash_flows.size())
            return -1;
        sum += cash_flows[i];
    }
    return i + 1;
}
