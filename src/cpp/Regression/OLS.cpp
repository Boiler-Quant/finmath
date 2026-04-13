#include "finmath/Regression/OLS.h"

#include <cmath>
#include <stdexcept>

namespace {

// Solve SPD system A x = b with Cholesky (A is m x m).
bool cholesky_solve(std::vector<std::vector<double>> A, std::vector<double> b, std::vector<double>& x)
{
    const size_t n = A.size();
    if (n == 0 || b.size() != n) {
        return false;
    }
    for (const auto& row : A) {
        if (row.size() != n) {
            return false;
        }
    }

    // L L^T = A
    std::vector<std::vector<double>> L(n, std::vector<double>(n, 0.0));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            double sum = A[i][j];
            for (size_t k = 0; k < j; ++k) {
                sum -= L[i][k] * L[j][k];
            }
            if (i == j) {
                if (sum <= 0.0) {
                    return false;
                }
                L[i][j] = std::sqrt(sum);
            } else {
                L[i][j] = sum / L[j][j];
            }
        }
    }

    // Forward y = L^{-1} b
    std::vector<double> y(n, 0.0);
    for (size_t i = 0; i < n; ++i) {
        double sum = b[i];
        for (size_t k = 0; k < i; ++k) {
            sum -= L[i][k] * y[k];
        }
        y[i] = sum / L[i][i];
    }

    // Back L^T x = y
    x.assign(n, 0.0);
    for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
        double sum = y[static_cast<size_t>(i)];
        for (size_t k = static_cast<size_t>(i) + 1; k < n; ++k) {
            sum -= L[k][i] * x[k];
        }
        x[static_cast<size_t>(i)] = sum / L[i][i];
    }
    return true;
}

}  // namespace

OLS::OLS(const std::vector<std::vector<double>>& X, const std::vector<double>& y)
{
    if (X.empty() || y.size() != X.size()) {
        throw std::invalid_argument("OLS: X and y must have the same number of rows");
    }
    const size_t n = X.size();
    const size_t p = X[0].size();
    for (const auto& row : X) {
        if (row.size() != p) {
            throw std::invalid_argument("OLS: all rows of X must have the same length");
        }
    }

    const size_t m = p + 1;  // intercept + features
    std::vector<std::vector<double>> XtX(m, std::vector<double>(m, 0.0));
    std::vector<double> Xty(m, 0.0);

    for (size_t i = 0; i < n; ++i) {
        std::vector<double> row(m, 1.0);
        for (size_t j = 0; j < p; ++j) {
            row[j + 1] = X[i][j];
        }
        for (size_t a = 0; a < m; ++a) {
            Xty[a] += row[a] * y[i];
            for (size_t b = 0; b < m; ++b) {
                XtX[a][b] += row[a] * row[b];
            }
        }
    }

    std::vector<double> beta;
    if (!cholesky_solve(std::move(XtX), Xty, beta)) {
        throw std::runtime_error("OLS: (X'X) is not positive definite; check for collinearity");
    }

    intercept_ = beta[0];
    coefficients_.assign(beta.begin() + 1, beta.end());
}

std::vector<double> OLS::predict(const std::vector<std::vector<double>>& X) const
{
    std::vector<double> out;
    out.reserve(X.size());
    for (const auto& row : X) {
        if (row.size() != coefficients_.size()) {
            throw std::invalid_argument("OLS::predict: feature dimension mismatch");
        }
        double pred = intercept_;
        for (size_t i = 0; i < row.size(); ++i) {
            pred += row[i] * coefficients_[i];
        }
        out.push_back(pred);
    }
    return out;
}
