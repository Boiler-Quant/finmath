#ifndef FINMATH_TEST_COMMON_H
#define FINMATH_TEST_COMMON_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

namespace finmath_test {

/// Relative tolerance comparison: |a - b| <= rel_tol * max(|a|, |b|).
/// Use for values that are not near zero.
[[nodiscard]] inline bool almost_equal(double a, double b, double rel_tol = 1e-9) noexcept {
    const double scale = std::max(std::abs(a), std::abs(b));
    if (scale < 1e-15) return std::abs(a - b) <= rel_tol;
    return std::abs(a - b) <= rel_tol * scale;
}

/// Absolute tolerance: |a - b| <= abs_tol. Use for values near zero.
[[nodiscard]] inline bool almost_equal_abs(double a, double b, double abs_tol = 1e-9) noexcept {
    return std::abs(a - b) <= abs_tol;
}

/// Fail the test with a message and exit(1). Use when assert isn't enough.
inline void fail(const char* file, int line, const std::string& msg) {
    std::cerr << "FAIL " << file << ":" << line << " " << msg << std::endl;
    std::exit(1);
}

#define FINMATH_TEST_FAIL(msg) \
    do { std::ostringstream _s; _s << msg; finmath_test::fail(__FILE__, __LINE__, _s.str()); } while (0)

#define FINMATH_TEST_ASSERT_NEAR(a, b, rel_tol) \
    do { \
        if (!finmath_test::almost_equal((a), (b), (rel_tol))) { \
            std::ostringstream _s; _s << "expected " << (a) << " ~= " << (b) << " (rel_tol=" << (rel_tol) << ")"; \
            finmath_test::fail(__FILE__, __LINE__, _s.str()); \
        } \
    } while (0)

}  // namespace finmath_test

#endif  // FINMATH_TEST_COMMON_H
