#include "test_common.h"
#include "finmath/Helper/simd_helper.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <iterator>
#include <iostream>
#include <vector>

using namespace finmath_test;

namespace {
constexpr auto kEpsilon = 1e-9;

template <typename GotIt, typename ExpectedIt>
bool vectors_near(GotIt got_start, GotIt got_end, ExpectedIt expected_start, ExpectedIt expected_end) {
    return std::distance(got_start, got_end) == std::distance(expected_start, expected_end) &&
           std::equal(got_start, got_end, expected_start,
                     [](double x, double y) { return almost_equal(x, y, kEpsilon); });
}

template <typename ContainerGot, typename ContainerExpected>
bool vectors_near(const ContainerGot& got, const ContainerExpected& expected) {
    return vectors_near(got.begin(), got.end(), expected.begin(), expected.end());
}
}  // namespace

int main() {
    std::cout << "Starting SIMD Helper Tests..." << std::endl;
    std::cout << "SIMD Backend: " << finmath::simd::get_simd_backend() << std::endl;
    std::cout << std::endl;

    const std::vector<double> a = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    const std::vector<double> b = {10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0};
    std::vector<double> result(a.size());

    // Test 1: Vector Addition
    {
        finmath::simd::vector_add(a.data(), b.data(), result.data(), a.size());
        constexpr std::array expected = {11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0};
        if (!vectors_near(result, expected)) FINMATH_TEST_FAIL("Test 1 (Vector Add)");
        std::cout << "✓ Test 1 (Vector Addition) Passed" << std::endl;
    }

    // Test 2: Vector Subtraction
    {
        finmath::simd::vector_sub(a.data(), b.data(), result.data(), a.size());
        const std::array expected = {-9.0, -7.0, -5.0, -3.0, -1.0, 1.0, 3.0, 5.0, 7.0, 9.0};
        if (!vectors_near(result, expected)) FINMATH_TEST_FAIL("Test 2 (Vector Sub)");
        std::cout << "✓ Test 2 (Vector Subtraction) Passed" << std::endl;
    }

    // Test 3: Vector Multiplication
    {
        finmath::simd::vector_mul(a.data(), b.data(), result.data(), a.size());
        const std::array expected = {10.0, 18.0, 24.0, 28.0, 30.0, 30.0, 28.0, 24.0, 18.0, 10.0};
        if (!vectors_near(result, expected)) FINMATH_TEST_FAIL("Test 3 (Vector Mul)");
        std::cout << "✓ Test 3 (Vector Multiplication) Passed" << std::endl;
    }

    // Test 4: Dot Product
    {
        const auto result_dot = finmath::simd::dot_product(a.data(), b.data(), a.size());
        FINMATH_TEST_ASSERT_NEAR(result_dot, 220.0, kEpsilon);
        std::cout << "✓ Test 4 (Dot Product) Passed" << std::endl;
    }

    // Test 5: Vector Sum
    {
        const auto result_sum = finmath::simd::vector_sum(a.data(), a.size());
        FINMATH_TEST_ASSERT_NEAR(result_sum, 55.0, kEpsilon);
        std::cout << "✓ Test 5 (Vector Sum) Passed" << std::endl;
    }

    // Test 6: Vector Mean
    {
        const auto result_mean = finmath::simd::vector_mean(a.data(), a.size());
        FINMATH_TEST_ASSERT_NEAR(result_mean, 5.5, kEpsilon);
        std::cout << "✓ Test 6 (Vector Mean) Passed" << std::endl;
    }

    // Test 7: Vector Variance
    {
        const auto result_var = finmath::simd::vector_variance(a.data(), a.size());
        FINMATH_TEST_ASSERT_NEAR(result_var, 8.25, kEpsilon);
        std::cout << "✓ Test 7 (Vector Variance) Passed" << std::endl;
    }

    // Test 8: Vector Standard Deviation
    {
        const auto result_std = finmath::simd::vector_stddev(a.data(), a.size());
        FINMATH_TEST_ASSERT_NEAR(result_std, std::sqrt(8.25), kEpsilon);
        std::cout << "✓ Test 8 (Vector Standard Deviation) Passed" << std::endl;
    }

    // Test 9: Edge case - Single element
    {
        const std::array small = {5.0};
        const auto mean = finmath::simd::vector_mean(small.data(), small.size());
        FINMATH_TEST_ASSERT_NEAR(mean, 5.0, kEpsilon);
        std::cout << "✓ Test 9 (Edge Cases) Passed" << std::endl;
    }

    // Test 10: Vector Scalar Multiplication
    {
        finmath::simd::vector_mul_scalar(a.data(), 2.5, result.data(), a.size());
        const std::array expected = {2.5, 5.0, 7.5, 10.0, 12.5, 15.0, 17.5, 20.0, 22.5, 25.0};
        if (!vectors_near(result, expected)) FINMATH_TEST_FAIL("Test 10 (Vector Mul Scalar)");
        std::cout << "✓ Test 10 (Vector Scalar Multiplication) Passed" << std::endl;
    }

    // Test 11: Vector Scalar Addition
    {
        finmath::simd::vector_add_scalar(a.data(), 10.0, result.data(), a.size());
        const std::array expected = {11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0};
        if (!vectors_near(result, expected)) FINMATH_TEST_FAIL("Test 11 (Vector Add Scalar)");
        std::cout << "✓ Test 11 (Vector Scalar Addition) Passed" << std::endl;
    }

    // Test 12: Vector Division
    {
        const std::vector<double> numerator = {10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0};
        const std::vector<double> denominator = {2.0, 4.0, 5.0, 8.0, 10.0, 12.0, 14.0, 16.0, 18.0, 20.0};
        finmath::simd::vector_div(numerator.data(), denominator.data(), result.data(), numerator.size());
        const std::array expected = {5.0, 5.0, 6.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0};
        if (!vectors_near(result, expected)) FINMATH_TEST_FAIL("Test 12 (Vector Division)");
        std::cout << "✓ Test 12 (Vector Division) Passed" << std::endl;
    }

    // Test 13: Vector Maximum
    {
        const std::array test_data = {1.0, 5.0, 3.0, 9.0, 2.0, 8.0, 4.0, 7.0, 6.0, 10.0};
        const auto max_val = finmath::simd::vector_max(test_data.data(), test_data.size());
        FINMATH_TEST_ASSERT_NEAR(max_val, 10.0, kEpsilon);
        std::cout << "✓ Test 13 (Vector Maximum) Passed" << std::endl;
    }

    // Test 14: Vector Minimum
    {
        const std::array test_data = {10.0, 5.0, 3.0, 9.0, 2.0, 8.0, 4.0, 7.0, 6.0, 1.0};
        const auto min_val = finmath::simd::vector_min(test_data.data(), test_data.size());
        FINMATH_TEST_ASSERT_NEAR(min_val, 1.0, kEpsilon);
        std::cout << "✓ Test 14 (Vector Minimum) Passed" << std::endl;
    }

    // Test 15: Vector Conditional Sum (Positive)
    {
        const std::array test_data = {-5.0, 10.0, -3.0, 7.0, -2.0, 8.0, -1.0, 5.0, -4.0, 6.0};
        const auto sum_positive = finmath::simd::vector_conditional_sum(test_data.data(), test_data.size(), true);
        FINMATH_TEST_ASSERT_NEAR(sum_positive, 36.0, kEpsilon);
        std::cout << "✓ Test 15 (Vector Conditional Sum - Positive) Passed" << std::endl;
    }

    // Test 16: Vector Conditional Sum (Negative)
    {
        const std::array test_data = {-5.0, 10.0, -3.0, 7.0, -2.0, 8.0, -1.0, 5.0, -4.0, 6.0};
        const auto sum_negative = finmath::simd::vector_conditional_sum(test_data.data(), test_data.size(), false);
        FINMATH_TEST_ASSERT_NEAR(sum_negative, 15.0, kEpsilon);
        std::cout << "✓ Test 16 (Vector Conditional Sum - Negative) Passed" << std::endl;
    }

    // Test 17: Large vector
    {
        constexpr size_t large_size = 10000;
        std::vector<double> large_a(large_size);
        std::vector<double> large_b(large_size);
        std::vector<double> large_result(large_size);
        for (size_t i = 0; i < large_size; ++i) {
            large_a[i] = static_cast<double>(i);
            large_b[i] = static_cast<double>(large_size - i);
        }
        finmath::simd::vector_add(large_a.data(), large_b.data(), large_result.data(), large_size);
        const auto expected_val = static_cast<double>(large_size);
        const bool ok = std::all_of(large_result.begin(), large_result.end(),
            [expected_val](double x) { return almost_equal(x, expected_val, kEpsilon); });
        if (!ok) FINMATH_TEST_FAIL("Test 17 (Large Vector)");
        std::cout << "✓ Test 17 (Large Vector Operations) Passed" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "All SIMD Helper Tests Passed! ✅" << std::endl;
    std::cout << "Backend: " << finmath::simd::get_simd_backend() << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}

