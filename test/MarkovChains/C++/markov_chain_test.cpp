#include "finmath/MarkovChains/markov_chain.h"
#include "test_common.h"

#include <cassert>
#include <cmath>
#include <iostream>

using namespace finmath_test;

int main()
{
    // Two-state absorbing-ish chain: [[0.9, 0.1], [0.5, 0.5]]
    std::vector<std::vector<double>> P = {{0.9, 0.1}, {0.5, 0.5}};
    assert(markov_is_row_stochastic(P, 1e-9));

    auto P2 = markov_matrix_power(P, 2);
    FINMATH_TEST_ASSERT_NEAR(P2[0][0] + P2[0][1], 1.0, 1e-9);
    FINMATH_TEST_ASSERT_NEAR(P2[1][0] + P2[1][1], 1.0, 1e-9);

    auto pi = markov_steady_state(P, 1e-10, 1000000);
    FINMATH_TEST_ASSERT_NEAR(pi[0] + pi[1], 1.0, 1e-9);
    // Stationary: pi0 = 0.9*pi0 + 0.5*pi1, pi1 = 0.1*pi0 + 0.5*pi1 -> pi0/pi1 = 5/1 -> pi = (5/6, 1/6)
    FINMATH_TEST_ASSERT_NEAR(pi[0], 5.0 / 6.0, 1e-5);
    FINMATH_TEST_ASSERT_NEAR(pi[1], 1.0 / 6.0, 1e-5);

    std::cout << "Markov Chain Tests Passed!" << std::endl;
    return 0;
}
