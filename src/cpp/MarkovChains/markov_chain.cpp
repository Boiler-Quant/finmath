#include "finmath/MarkovChains/markov_chain.h"

#include <cmath>
#include <stdexcept>

namespace {

double row_sum(const std::vector<std::vector<double>>& P, size_t i)
{
    double s = 0.0;
    for (double v : P[i]) {
        s += v;
    }
    return s;
}

}  // namespace

bool markov_is_row_stochastic(const std::vector<std::vector<double>>& P, double tol)
{
    if (P.empty()) {
        return false;
    }
    const size_t n = P.size();
    for (const auto& row : P) {
        if (row.size() != n) {
            return false;
        }
    }
    for (size_t i = 0; i < n; ++i) {
        if (std::abs(row_sum(P, i) - 1.0) > tol) {
            return false;
        }
    }
    return true;
}

std::vector<std::vector<double>> markov_matrix_multiply(const std::vector<std::vector<double>>& A,
                                                        const std::vector<std::vector<double>>& B)
{
    if (A.empty() || A.size() != A[0].size() || B.size() != B[0].size() || A.size() != B.size()) {
        throw std::invalid_argument("markov_matrix_multiply: expected square matrices of equal size");
    }
    const size_t n = A.size();
    std::vector<std::vector<double>> C(n, std::vector<double>(n, 0.0));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            double s = 0.0;
            for (size_t k = 0; k < n; ++k) {
                s += A[i][k] * B[k][j];
            }
            C[i][j] = s;
        }
    }
    return C;
}

std::vector<std::vector<double>> markov_matrix_power(const std::vector<std::vector<double>>& P, int n)
{
    if (n < 0) {
        throw std::invalid_argument("markov_matrix_power: n must be >= 0");
    }
    if (P.empty()) {
        return {};
    }
    const size_t dim = P.size();
    std::vector<std::vector<double>> R(dim, std::vector<double>(dim, 0.0));
    for (size_t i = 0; i < dim; ++i) {
        R[i][i] = 1.0;
    }
    std::vector<std::vector<double>> base = P;
    int e = n;
    while (e > 0) {
        if (e & 1) {
            R = markov_matrix_multiply(R, base);
        }
        e >>= 1;
        if (e > 0) {
            base = markov_matrix_multiply(base, base);
        }
    }
    return R;
}


std::vector<double> markov_steady_state(const std::vector<std::vector<double>>& P, double tol, int max_iter)
{
    if (!markov_is_row_stochastic(P, 1e-6)) {
        throw std::invalid_argument("markov_steady_state: P must be a square row-stochastic matrix");
    }
    const size_t n = P.size();
    std::vector<double> pi(n, 1.0 / static_cast<double>(n));
    for (int iter = 0; iter < max_iter; ++iter) {
        std::vector<double> next(n, 0.0);
        for (size_t j = 0; j < n; ++j) {
            for (size_t i = 0; i < n; ++i) {
                next[j] += pi[i] * P[i][j];
            }
        }
        double diff = 0.0;
        for (size_t i = 0; i < n; ++i) {
            diff = std::max(diff, std::abs(next[i] - pi[i]));
        }
        pi.swap(next);
        if (diff < tol) {
            return pi;
        }
    }
    throw std::runtime_error("markov_steady_state: did not converge");
}
