#ifndef FINMATH_REGRESSION_OLS_H
#define FINMATH_REGRESSION_OLS_H

#include <vector>

// Ordinary least squares: y ~ intercept + X1 + ... + Xp (each row of X is one observation).
class OLS {
public:
    OLS(const std::vector<std::vector<double>>& X, const std::vector<double>& y);

    const std::vector<double>& coefficients() const { return coefficients_; }
    double intercept() const { return intercept_; }

    std::vector<double> predict(const std::vector<std::vector<double>>& X) const;

private:
    std::vector<double> coefficients_;
    double intercept_{0.0};
};

#endif  // FINMATH_REGRESSION_OLS_H
