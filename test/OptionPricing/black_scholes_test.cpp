#include "test_common.h"
#include "finmath/OptionPricing/black_scholes.h"
#include <array>
#include <iostream>
#include <tuple>

using namespace finmath_test;

namespace {
constexpr double kTolerance = 0.001;

// (option_type, strike, price/spot, time, rate, vol, expected_price) — API: black_scholes(type, strike, price, time, rate, vol)
using Case = std::tuple<OptionType, double, double, double, double, double, double>;
constexpr std::array kCases = {
    Case{OptionType::CALL, 100, 105, 1, 0.05, 0.2, 13.8579},  // strike 100, spot 105
    Case{OptionType::PUT,  100, 95, 1, 0.05, 0.2, 7.6338},    // strike 100, spot 95
    Case{OptionType::CALL, 100, 100, 1, 0.05, 0.2, 10.4506},
    Case{OptionType::PUT,  100, 100, 1, 0.05, 0.2, 5.5735},
    Case{OptionType::CALL, 100, 100, 10, 0.05, 0.2, 45.1930},
    Case{OptionType::CALL, 100, 100, 1, 0.05, 1.0, 39.8402},
    Case{OptionType::CALL, 100, 100, 1, 0.05, 0.01, 4.8771},
    Case{OptionType::CALL, 100, 100, 1, 0.01, 0.2, 8.4333},
    Case{OptionType::CALL, 50, 100, 1, 0.05, 0.2, 52.4389},   // deep ITM: strike 50, spot 100
    Case{OptionType::CALL, 150, 100, 1, 0.05, 0.2, 0.3596},   // deep OTM: strike 150, spot 100
};
}  // namespace

int black_scholes_tests() {
    for (const auto& [type, strike, spot, T, r, sigma, expected] : kCases) {
        const auto result = black_scholes(type, strike, spot, T, r, sigma);
        FINMATH_TEST_ASSERT_NEAR(result, expected, kTolerance);
    }
    std::cout << "Black-Scholes Tests Passed!" << std::endl;
    return 0;
}

int main() {
    return black_scholes_tests();
}
