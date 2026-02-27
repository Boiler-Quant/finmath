#include "finmath/InterestAndAnnuities/compound_interest.h"
#include <cmath>

double compound_interest(double principal, double rate, int time, int frequency) {
    if (time < 0) {
        return 0.0;
    }
    if (frequency <= 0) {
        return principal;
    }
    return principal * std::pow(1.0 + rate / (100.0 * frequency), static_cast<double>(time * frequency));
}
