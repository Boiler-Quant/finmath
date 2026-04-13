#ifndef MARKOV_CHAIN_H
#define MARKOV_CHAIN_H

#include <vector>

// Row-stochastic transition matrix P: P[i][j] = P(X_{t+1}=j | X_t=i); rows sum to 1.

bool markov_is_row_stochastic(const std::vector<std::vector<double>>& P, double tol = 1e-9);

std::vector<std::vector<double>> markov_matrix_multiply(const std::vector<std::vector<double>>& A,
                                                        const std::vector<std::vector<double>>& B);

std::vector<std::vector<double>> markov_matrix_power(const std::vector<std::vector<double>>& P, int n);

// Stationary distribution (row vector pi with pi * P = pi, sum pi = 1) via power iteration.
std::vector<double> markov_steady_state(const std::vector<std::vector<double>>& P, double tol = 1e-12,
                                        int max_iter = 1000000);

#endif  // MARKOV_CHAIN_H
