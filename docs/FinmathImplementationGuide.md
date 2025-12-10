# Implementation Guide: QuantLib-Style Simple Functions for Finmath

## Table of Contents
1. [Introduction & Motivation](#introduction--motivation)
2. [Project Context](#project-context)
3. [Function Specifications](#function-specifications)
4. [Step-by-Step Integration Guide](#step-by-step-integration-guide)
5. [Comprehensive Testing Guide](#comprehensive-testing-guide)
6. [Real-World Examples](#real-world-examples)
7. [Common Pitfalls & Best Practices](#common-pitfalls--best-practices)
8. [References & Resources](#references--resources)

---

## Introduction & Motivation

### Why These Functions Matter

The functions you'll be implementing are the **foundational building blocks** of quantitative finance. Every complex financial model, from bond pricing to portfolio optimization, relies on these core concepts:

1. **Discount Factors**: Convert future cash flows to present values (essential for all valuation)
2. **Present/Future Value**: The time value of money - a dollar today is worth more than a dollar tomorrow
3. **Annuities**: Regular payment streams (mortgages, bonds, leases all use annuities)
4. **Cash Flow Analysis**: Investment decision-making (NPV, IRR determine if projects are profitable)
5. **Bond Pricing**: Understanding fixed-income securities (largest asset class in finance)

### Real-World Applications

- **Portfolio Management**: Calculating present values of future returns
- **Corporate Finance**: Evaluating investment projects using NPV and IRR
- **Fixed Income Trading**: Pricing bonds and calculating yields
- **Risk Management**: Understanding time value of money in risk models
- **Derivatives Pricing**: Options, swaps, and other derivatives all use discount factors

### Learning Value

By implementing these functions, you'll:
- Understand core financial mathematics concepts
- Learn C++ best practices (error handling, testing, documentation)
- Practice software engineering (modular design, integration, testing)
- Contribute to a production-ready library used by quantitative finance professionals

---

## Project Context

### Finmath Library Structure

```
finmath/
├── include/finmath/          # Header files (.h)
│   ├── InterestAndAnnuities/ # Where your functions will go
│   ├── OptionPricing/
│   ├── TimeSeries/
│   └── Helper/
├── src/cpp/                  # Implementation files (.cpp)
│   ├── InterestAndAnnuities/ # Your .cpp files here
│   └── ...
├── test/                      # Test files
│   └── InterestAndAnnuities/ # Your test files here
└── src/python_bindings.cpp    # Python integration
```

### Existing Functions (Reference)

Before starting, examine these existing functions to understand the code style:
- `src/cpp/InterestAndAnnuities/compound_interest.cpp` - Similar structure to what you'll write
- `test/InterestAndAnnuities/compound_interest_test.cpp` - See how tests are structured
- `src/python_bindings.cpp` - See how C++ functions are exposed to Python

### Code Style Guidelines

1. **Use meaningful names**: `discount_factor` not `df`
2. **Validate inputs**: Check for negative rates, invalid ranges
3. **Handle edge cases**: Zero rates, zero time periods
4. **Document clearly**: Comment your mathematical formulas
5. **Follow existing patterns**: Match the style of `compound_interest.cpp`

---

## Function Specifications

### 1. Discount Factors

**Location:** `include/finmath/InterestAndAnnuities/discount_factor.h`

**Motivation:** A discount factor tells you how much a dollar received in the future is worth today. This is fundamental to all valuation - you can't compare cash flows at different times without discounting them.

**Mathematical Background:**
- **Discrete Compounding**: Interest is paid at regular intervals (annually, quarterly, etc.)
  - Formula: `DF = 1 / (1 + r)^t`
  - Example: At 5% annual rate, $1 in 1 year = $0.9524 today
  
- **Continuous Compounding**: Interest is compounded continuously (like a savings account that compounds every instant)
  - Formula: `DF = e^(-r*t)`
  - Example: At 5% continuous rate, $1 in 1 year = $0.9512 today

- **Future Value Factor**: The inverse of discount factor - how much $1 today will be worth in the future
  - Formula: `FVF = (1 + r)^t`

**Function Signatures:**
```cpp
/**
 * Discrete compounding discount factor
 * Converts a future value to present value using discrete compounding
 * 
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Discount factor (0 < DF <= 1)
 * 
 * Formula: DF = 1 / (1 + r)^t
 * 
 * @throws std::invalid_argument if rate < 0 or time < 0
 */
double discount_factor(double rate, double time);

/**
 * Continuous compounding discount factor
 * Uses exponential compounding for continuous interest
 * 
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Discount factor (0 < DF <= 1)
 * 
 * Formula: DF = e^(-r*t)
 * 
 * @throws std::invalid_argument if rate < 0 or time < 0
 */
double discount_factor_continuous(double rate, double time);

/**
 * Future value factor
 * Converts a present value to future value
 * 
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Future value factor (FVF >= 1)
 * 
 * Formula: FVF = (1 + r)^t
 * 
 * @throws std::invalid_argument if rate < 0 or time < 0
 */
double future_value_factor(double rate, double time);
```

**Implementation Notes:**
- Use `std::pow(1.0 + rate, time)` for discrete compounding
- Use `std::exp(-rate * time)` for continuous compounding
- Validate inputs: throw `std::invalid_argument` for negative values
- Handle edge case: `rate == 0` should return `1.0` for discount factor, `1.0` for future value factor

---

### 2. Present/Future Value

**Location:** `include/finmath/InterestAndAnnuities/present_future_value.h`

**Motivation:** Present and future value calculations are the most common operations in finance. Every investment decision involves comparing cash flows at different times.

**Mathematical Background:**
- **Present Value (PV)**: The current worth of a future sum of money
  - Discrete: `PV = FV / (1 + r)^t`
  - Continuous: `PV = FV * e^(-r*t)`
  
- **Future Value (FV)**: The value of a current sum of money at a future date
  - Discrete: `FV = PV * (1 + r)^t`
  - Continuous: `FV = PV * e^(r*t)`

**Function Signatures:**
```cpp
/**
 * Present value with discrete compounding
 * Calculates what a future amount is worth today
 * 
 * @param future_value Amount to be received in the future
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years until the future value is received
 * @return Present value of the future amount
 * 
 * Formula: PV = FV / (1 + r)^t
 */
double present_value(double future_value, double rate, double time);

/**
 * Future value with discrete compounding
 * Calculates what a current amount will be worth in the future
 * 
 * @param present_value Current amount
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Future value of the present amount
 * 
 * Formula: FV = PV * (1 + r)^t
 */
double future_value(double present_value, double rate, double time);

/**
 * Present value with continuous compounding
 * Uses exponential compounding
 * 
 * @param future_value Amount to be received in the future
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Present value with continuous compounding
 * 
 * Formula: PV = FV * e^(-r*t)
 */
double present_value_continuous(double future_value, double rate, double time);

/**
 * Future value with continuous compounding
 * 
 * @param present_value Current amount
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Future value with continuous compounding
 * 
 * Formula: FV = PV * e^(r*t)
 */
double future_value_continuous(double present_value, double rate, double time);
```

**Implementation Notes:**
- These can reuse `discount_factor` functions internally
- `present_value(fv, r, t) = fv * discount_factor(r, t)`
- `future_value(pv, r, t) = pv * future_value_factor(r, t)`

---

### 3. Annuity Functions

**Location:** `include/finmath/InterestAndAnnuities/annuity.h`

**Motivation:** Annuities are everywhere in finance: mortgage payments, bond coupon payments, lease payments, retirement savings. Understanding annuities is essential for fixed-income analysis and personal finance.

**Mathematical Background:**
- **Ordinary Annuity**: Payments made at the END of each period
  - PV Formula: `PV = P * [1 - (1+r)^(-n)] / r`
  - FV Formula: `FV = P * [(1+r)^n - 1] / r`
  
- **Annuity Due**: Payments made at the BEGINNING of each period
  - PV Formula: `PV = P * [1 - (1+r)^(-n)] / r * (1 + r)`
  - FV Formula: `FV = P * [(1+r)^n - 1] / r * (1 + r)`

**Real-World Example:**
- Mortgage: $200,000 loan at 4% for 30 years
  - Monthly payment = `annuity_present_value` solved for payment
  - Total payments = `annuity_future_value`

**Function Signatures:**
```cpp
/**
 * Present value of ordinary annuity
 * Payments are made at the END of each period
 * 
 * @param payment Payment amount per period
 * @param rate Interest rate per period (not annualized if periods are not annual)
 * @param periods Number of payment periods
 * @return Present value of the annuity
 * 
 * Formula: PV = P * [1 - (1+r)^(-n)] / r
 * 
 * Edge cases:
 * - If rate == 0: return payment * periods
 * - If periods == 0: return 0
 */
double annuity_present_value(double payment, double rate, int periods);

/**
 * Future value of ordinary annuity
 * 
 * @param payment Payment amount per period
 * @param rate Interest rate per period
 * @param periods Number of payment periods
 * @return Future value of the annuity
 * 
 * Formula: FV = P * [(1+r)^n - 1] / r
 */
double annuity_future_value(double payment, double rate, int periods);

/**
 * Present value of annuity due
 * Payments are made at the BEGINNING of each period
 * 
 * @param payment Payment amount per period
 * @param rate Interest rate per period
 * @param periods Number of payment periods
 * @return Present value of annuity due
 * 
 * Formula: PV = P * [1 - (1+r)^(-n)] / r * (1 + r)
 */
double annuity_due_present_value(double payment, double rate, int periods);

/**
 * Future value of annuity due
 * 
 * @param payment Payment amount per period
 * @param rate Interest rate per period
 * @param periods Number of payment periods
 * @return Future value of annuity due
 * 
 * Formula: FV = P * [(1+r)^n - 1] / r * (1 + r)
 */
double annuity_due_future_value(double payment, double rate, int periods);
```

**Implementation Notes:**
- **Critical edge case**: When `rate == 0`, the formula becomes `0/0`. Handle this by returning `payment * periods`
- **Zero periods**: Return `0` if `periods == 0`
- **Negative periods**: Consider throwing an exception or returning `0`
- Use `std::pow(1.0 + rate, periods)` for `(1+r)^n`

---

### 4. Cash Flow Operations

**Location:** `include/finmath/InterestAndAnnuities/cash_flow.h`

**Motivation:** These are the decision-making tools of corporate finance. Companies use NPV and IRR to decide which projects to invest in. Understanding these is essential for investment banking, corporate finance, and private equity.

**Mathematical Background:**
- **Net Present Value (NPV)**: Sum of all discounted cash flows
  - Formula: `NPV = sum(CF_i / (1+r)^i) - Initial Investment`
  - Decision rule: Invest if NPV > 0
  
- **Internal Rate of Return (IRR)**: The discount rate that makes NPV = 0
  - Found using iterative methods (Newton-Raphson)
  - Decision rule: Invest if IRR > required rate of return
  
- **Payback Period**: How long until cumulative cash flows recover initial investment
  - Simple metric: doesn't account for time value of money

**Function Signatures:**
```cpp
/**
 * Net Present Value
 * Calculates the present value of all cash flows
 * 
 * @param cash_flows Vector of cash flows (negative = outflow, positive = inflow)
 * @param rate Discount rate (e.g., 0.10 for 10%)
 * @param initial_investment Optional initial investment (default: 0.0)
 * @return Net present value
 * 
 * Formula: NPV = sum(CF_i / (1+r)^i) - Initial Investment
 * 
 * Example:
 *   cash_flows = [-1000, 100, 200, 300, 400]
 *   rate = 0.10
 *   NPV = -1000 + 100/(1.1) + 200/(1.1)^2 + 300/(1.1)^3 + 400/(1.1)^4
 */
double net_present_value(
    const std::vector<double>& cash_flows, 
    double rate,
    double initial_investment = 0.0
);

/**
 * Internal Rate of Return
 * Finds the discount rate that makes NPV = 0
 * Uses Newton-Raphson iterative method
 * 
 * @param cash_flows Vector of cash flows
 * @param initial_guess Starting guess for IRR (default: 0.1 = 10%)
 * @param max_iterations Maximum iterations for convergence (default: 100)
 * @param tolerance Convergence tolerance (default: 1e-6)
 * @return Internal rate of return
 * 
 * Algorithm:
 * 1. Start with initial guess
 * 2. Calculate NPV and dNPV/dr at current guess
 * 3. Update: r_new = r_old - NPV / dNPV/dr
 * 4. Repeat until |NPV| < tolerance
 * 
 * @throws std::runtime_error if convergence fails
 */
double internal_rate_of_return(
    const std::vector<double>& cash_flows,
    double initial_guess = 0.1,
    int max_iterations = 100,
    double tolerance = 1e-6
);

/**
 * Payback period
 * Returns the number of periods until cumulative cash flows exceed initial investment
 * 
 * @param cash_flows Vector of cash flows (first element is typically initial investment)
 * @param initial_investment Initial investment amount
 * @return Number of periods until payback (returns -1 if never pays back)
 * 
 * Example:
 *   cash_flows = [100, 200, 300, 400]
 *   initial_investment = 500
 *   Cumulative: 100, 300, 600 (payback at period 3)
 */
int payback_period(
    const std::vector<double>& cash_flows,
    double initial_investment
);
```

**Implementation Notes:**
- **NPV**: Use discount factors for each period, sum all discounted cash flows
- **IRR**: Implement Newton-Raphson method:
  ```cpp
  // Derivative of NPV with respect to rate
  double dnpv_dr = 0.0;
  for (size_t i = 0; i < cash_flows.size(); ++i) {
      double df = discount_factor(rate, static_cast<double>(i));
      dnpv_dr -= static_cast<double>(i) * cash_flows[i] * df / (1.0 + rate);
  }
  ```
- **Payback**: Simple cumulative sum, find first period where cumulative > initial investment

---

### 5. Bond Pricing Basics

**Location:** `include/finmath/FixedIncome/bond_pricing.h`

**Motivation:** Bonds are the largest asset class in global markets. Understanding bond pricing is essential for fixed-income trading, portfolio management, and risk analysis.

**Mathematical Background:**
- **Bond Price**: Sum of discounted coupon payments plus discounted face value
  - Formula: `Price = sum(Coupon / (1+r)^i) + Face / (1+r)^n`
  
- **Yield to Maturity (YTM)**: The discount rate that makes bond price = market price
  - Found iteratively (similar to IRR)
  
- **Duration (Macaulay)**: Weighted average time to receive cash flows
  - Formula: `Duration = sum(t * PV(CF_t)) / Price`
  - Measures interest rate sensitivity

**Function Signatures:**
```cpp
/**
 * Bond price (coupon bond)
 * Calculates the theoretical price of a bond
 * 
 * @param face_value Face value (par value) of the bond
 * @param coupon_rate Annual coupon rate (e.g., 0.05 for 5%)
 * @param yield_to_maturity Yield to maturity (discount rate)
 * @param periods Number of coupon payment periods per year
 * @param time_to_maturity Time to maturity in years
 * @return Bond price
 * 
 * Formula: Price = sum(Coupon / (1+r)^i) + Face / (1+r)^n
 * 
 * Example:
 *   10-year bond, $1000 face, 5% coupon, paid semi-annually, 4% YTM
 *   Coupon per period = $1000 * 0.05 / 2 = $25
 *   Number of periods = 10 * 2 = 20
 *   Rate per period = 0.04 / 2 = 0.02
 */
double bond_price(
    double face_value,
    double coupon_rate,
    double yield_to_maturity,
    int periods,
    double time_to_maturity
);

/**
 * Bond yield (simplified, iterative)
 * Finds YTM given bond price
 * Uses Newton-Raphson method
 * 
 * @param face_value Face value of the bond
 * @param coupon_rate Annual coupon rate
 * @param price Current market price of the bond
 * @param periods Number of coupon payment periods per year
 * @param time_to_maturity Time to maturity in years
 * @return Yield to maturity
 */
double bond_yield(
    double face_value,
    double coupon_rate,
    double price,
    int periods,
    double time_to_maturity
);

/**
 * Duration (Macaulay)
 * Measures interest rate sensitivity
 * 
 * @param face_value Face value of the bond
 * @param coupon_rate Annual coupon rate
 * @param yield_to_maturity Yield to maturity
 * @param periods Number of coupon payment periods per year
 * @param time_to_maturity Time to maturity in years
 * @return Macaulay duration in years
 * 
 * Formula: Duration = sum(t * PV(CF_t)) / Price
 */
double bond_duration(
    double face_value,
    double coupon_rate,
    double yield_to_maturity,
    int periods,
    double time_to_maturity
);
```

---

## Step-by-Step Integration Guide

### Step 1: Create Header File

**Example: `include/finmath/InterestAndAnnuities/discount_factor.h`**

```cpp
#ifndef DISCOUNT_FACTOR_H
#define DISCOUNT_FACTOR_H

#include <cmath>

/**
 * @file discount_factor.h
 * @brief Discount factor calculations for time value of money
 * 
 * Discount factors convert future cash flows to present values.
 * This is fundamental to all financial valuation.
 */

/**
 * Discrete compounding discount factor
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Discount factor (0 < DF <= 1)
 * @throws std::invalid_argument if rate < 0 or time < 0
 */
double discount_factor(double rate, double time);

/**
 * Continuous compounding discount factor
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Discount factor (0 < DF <= 1)
 * @throws std::invalid_argument if rate < 0 or time < 0
 */
double discount_factor_continuous(double rate, double time);

/**
 * Future value factor
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Future value factor (FVF >= 1)
 * @throws std::invalid_argument if rate < 0 or time < 0
 */
double future_value_factor(double rate, double time);

#endif // DISCOUNT_FACTOR_H
```

**Key Points:**
- Use header guards (`#ifndef`, `#define`, `#endif`)
- Include necessary headers (`<cmath>` for `std::pow`, `std::exp`)
- Add clear documentation comments
- Specify parameter meanings and return values

---

### Step 2: Implement Functions

**Example: `src/cpp/InterestAndAnnuities/discount_factor.cpp`**

```cpp
#include "finmath/InterestAndAnnuities/discount_factor.h"
#include <cmath>
#include <stdexcept>

double discount_factor(double rate, double time) {
    // Input validation
    if (rate < 0) {
        throw std::invalid_argument("Interest rate cannot be negative");
    }
    if (time < 0) {
        throw std::invalid_argument("Time cannot be negative");
    }
    
    // Edge case: zero rate
    if (rate == 0.0) {
        return 1.0;
    }
    
    // Discrete compounding: DF = 1 / (1 + r)^t
    return 1.0 / std::pow(1.0 + rate, time);
}

double discount_factor_continuous(double rate, double time) {
    // Input validation
    if (rate < 0) {
        throw std::invalid_argument("Interest rate cannot be negative");
    }
    if (time < 0) {
        throw std::invalid_argument("Time cannot be negative");
    }
    
    // Continuous compounding: DF = e^(-r*t)
    return std::exp(-rate * time);
}

double future_value_factor(double rate, double time) {
    // Input validation
    if (rate < 0) {
        throw std::invalid_argument("Interest rate cannot be negative");
    }
    if (time < 0) {
        throw std::invalid_argument("Time cannot be negative");
    }
    
    // Edge case: zero rate
    if (rate == 0.0) {
        return 1.0;
    }
    
    // Future value factor: FVF = (1 + r)^t
    return std::pow(1.0 + rate, time);
}
```

**Key Points:**
- Always validate inputs
- Handle edge cases (zero rate, zero time)
- Use appropriate mathematical functions (`std::pow`, `std::exp`)
- Throw meaningful exceptions for invalid inputs

---

### Step 3: Add Python Bindings

**In: `src/python_bindings.cpp`**

Add your includes at the top:
```cpp
#include "finmath/InterestAndAnnuities/discount_factor.h"
#include "finmath/InterestAndAnnuities/present_future_value.h"
#include "finmath/InterestAndAnnuities/annuity.h"
#include "finmath/InterestAndAnnuities/cash_flow.h"
```

Add bindings inside `PYBIND11_MODULE(finmath, m)`:
```cpp
PYBIND11_MODULE(finmath, m) {
    m.doc() = "Financial Math Library";
    
    // ... existing bindings ...
    
    // Discount factors
    m.def("discount_factor", &discount_factor, 
          "Discount factor (discrete compounding)",
          py::arg("rate"), py::arg("time"));
    
    m.def("discount_factor_continuous", &discount_factor_continuous,
          "Discount factor (continuous compounding)",
          py::arg("rate"), py::arg("time"));
    
    m.def("future_value_factor", &future_value_factor,
          "Future value factor",
          py::arg("rate"), py::arg("time"));
    
    // Present/Future value
    m.def("present_value", &present_value,
          "Present value (discrete compounding)",
          py::arg("future_value"), py::arg("rate"), py::arg("time"));
    
    m.def("future_value", &future_value,
          "Future value (discrete compounding)",
          py::arg("present_value"), py::arg("rate"), py::arg("time"));
    
    // ... continue for all functions ...
}
```

**Key Points:**
- Use `py::arg()` for named arguments (makes Python API clearer)
- Add descriptive docstrings
- Follow the existing pattern in `python_bindings.cpp`

---

### Step 4: Add to CMakeLists.txt (If Needed)

The build system should automatically pick up new `.cpp` files in `src/cpp/`, but verify:

1. Check that your `.cpp` file is in the right directory
2. Rebuild: `cd build && cmake .. && make`
3. If compilation fails, check that all includes are correct

---

### Step 5: Create Test File

**Example: `test/InterestAndAnnuities/discount_factor_test.cpp`**

```cpp
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "finmath/InterestAndAnnuities/discount_factor.h"

// Helper function for floating-point comparison
bool almost_equal(double a, double b, double tolerance = 1e-5) {
    return std::abs(a - b) < tolerance;
}

int main() {
    int tests_passed = 0;
    int tests_total = 0;
    
    // Test 1: Basic discount factor calculation
    {
        tests_total++;
        double df = discount_factor(0.05, 1.0);
        double expected = 1.0 / 1.05; // 0.952380952...
        if (almost_equal(df, expected, 1e-6)) {
            std::cout << "✓ Test 1 passed: Basic discount factor" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 1 failed: Expected " << expected 
                      << ", got " << df << std::endl;
        }
    }
    
    // Test 2: Discount factor for multiple years
    {
        tests_total++;
        double df = discount_factor(0.10, 2.0);
        double expected = 1.0 / (1.1 * 1.1); // 0.826446281
        if (almost_equal(df, expected, 1e-6)) {
            std::cout << "✓ Test 2 passed: Multi-year discount factor" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 2 failed" << std::endl;
        }
    }
    
    // Test 3: Zero rate
    {
        tests_total++;
        double df = discount_factor(0.0, 1.0);
        if (almost_equal(df, 1.0, 1e-6)) {
            std::cout << "✓ Test 3 passed: Zero rate" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 3 failed" << std::endl;
        }
    }
    
    // Test 4: Zero time
    {
        tests_total++;
        double df = discount_factor(0.05, 0.0);
        if (almost_equal(df, 1.0, 1e-6)) {
            std::cout << "✓ Test 4 passed: Zero time" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 4 failed" << std::endl;
        }
    }
    
    // Test 5: Negative rate (should throw exception)
    {
        tests_total++;
        try {
            discount_factor(-0.05, 1.0);
            std::cout << "✗ Test 5 failed: Should have thrown exception" << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << "✓ Test 5 passed: Negative rate exception" << std::endl;
            tests_passed++;
        }
    }
    
    // Test 6: Negative time (should throw exception)
    {
        tests_total++;
        try {
            discount_factor(0.05, -1.0);
            std::cout << "✗ Test 6 failed: Should have thrown exception" << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << "✓ Test 6 passed: Negative time exception" << std::endl;
            tests_passed++;
        }
    }
    
    // Test 7: Continuous discount factor
    {
        tests_total++;
        double df = discount_factor_continuous(0.05, 1.0);
        double expected = std::exp(-0.05); // 0.9512294245
        if (almost_equal(df, expected, 1e-6)) {
            std::cout << "✓ Test 7 passed: Continuous discount factor" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 7 failed: Expected " << expected 
                      << ", got " << df << std::endl;
        }
    }
    
    // Test 8: Future value factor
    {
        tests_total++;
        double fvf = future_value_factor(0.05, 1.0);
        double expected = 1.05;
        if (almost_equal(fvf, expected, 1e-6)) {
            std::cout << "✓ Test 8 passed: Future value factor" << std::endl;
            tests_passed++;
        } else {
            std::cout << "✗ Test 8 failed" << std::endl;
        }
    }
    
    // Summary
    std::cout << "\n" << tests_passed << "/" << tests_total 
              << " tests passed" << std::endl;
    
    if (tests_passed == tests_total) {
        std::cout << "All tests passed! ✓" << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed. Please review your implementation." << std::endl;
        return 1;
    }
}
```

**Add to CMakeLists.txt:**
```cmake
# In the test section, add:
add_cpp_test(DiscountFactorTest test/InterestAndAnnuities/discount_factor_test.cpp)
```

**Key Points:**
- Test normal cases (expected behavior)
- Test edge cases (zero rate, zero time)
- Test error cases (negative inputs should throw)
- Use floating-point tolerance for comparisons
- Provide clear test output

---

## Comprehensive Testing Guide

### Types of Tests to Write

#### 1. **Normal Case Tests**
Test the function with typical inputs and verify against known results.

**Example for discount_factor:**
```cpp
// Known result: 5% for 1 year = 0.952380952...
double df = discount_factor(0.05, 1.0);
assert(almost_equal(df, 0.952380952, 1e-6));
```

#### 2. **Edge Case Tests**
Test boundary conditions and special values.

**Edge Cases to Test:**
- Zero rate: `rate == 0`
- Zero time: `time == 0`
- Very small values: `rate = 1e-10`, `time = 1e-10`
- Very large values: `rate = 1.0` (100%), `time = 100` years

#### 3. **Error Case Tests**
Test that invalid inputs throw appropriate exceptions.

**Error Cases:**
- Negative rate: Should throw `std::invalid_argument`
- Negative time: Should throw `std::invalid_argument`
- Empty cash flow vector (for NPV/IRR): Should handle gracefully

#### 4. **Mathematical Relationship Tests**
Test that related functions are consistent.

**Example:**
```cpp
// PV * FVF should equal FV
double pv = 1000.0;
double rate = 0.05;
double time = 2.0;
double fv = future_value(pv, rate, time);
double fvf = future_value_factor(rate, time);
assert(almost_equal(fv, pv * fvf, 1e-6));

// PV = FV * DF
double df = discount_factor(rate, time);
assert(almost_equal(pv, fv * df, 1e-6));
```

#### 5. **Comparison with Known Values**
Use financial calculators or spreadsheets to verify results.

**Resources:**
- Excel: `=PV(rate, nper, pmt, fv)` for present value
- Online calculators: Investopedia, Calculator.net
- QuantLib: Compare with QuantLib's results if available

### Test Data for Common Functions

#### Discount Factors
```cpp
// Test cases with known results
// Rate: 5%, Time: 1 year → DF = 0.952380952
// Rate: 10%, Time: 2 years → DF = 0.826446281
// Rate: 0%, Time: 5 years → DF = 1.0
```

#### Annuities
```cpp
// Test case: $100 payment, 5% rate, 10 periods
// PV of ordinary annuity = $772.17 (approximately)
// FV of ordinary annuity = $1,257.79 (approximately)
```

#### NPV
```cpp
// Test case from Investopedia:
// Initial investment: -$1000
// Cash flows: [100, 200, 300, 400]
// Rate: 10%
// Expected NPV: -$1000 + 100/1.1 + 200/1.1^2 + 300/1.1^3 + 400/1.1^4
//               = -$1000 + 90.91 + 165.29 + 225.39 + 273.21
//               = -$245.20 (approximately)
```

### Running Tests

```bash
# Build the project
cd /Users/shashank/Desktop/finmath
mkdir -p build && cd build
cmake ..
make

# Run a specific test
./DiscountFactorTest_executable

# Run all tests
ctest

# Run with verbose output
ctest --verbose
```

---

## Real-World Examples

### Example 1: Mortgage Payment Calculation

```python
import finmath

# Mortgage: $300,000 loan at 4% annual rate for 30 years
# Monthly payments (annuity due)
principal = 300000
annual_rate = 0.04
monthly_rate = annual_rate / 12
months = 30 * 12

# Calculate monthly payment using annuity present value formula
# PV = Payment * [1 - (1+r)^(-n)] / r
# Payment = PV / ([1 - (1+r)^(-n)] / r)
pv_annuity_factor = finmath.annuity_present_value(1.0, monthly_rate, months)
monthly_payment = principal / pv_annuity_factor

print(f"Monthly mortgage payment: ${monthly_payment:.2f}")
# Expected: ~$1,432.25
```

### Example 2: Investment Decision Using NPV

```python
import finmath

# Project: Initial investment of $50,000
# Expected cash flows over 5 years: [10000, 15000, 20000, 25000, 30000]
# Required rate of return: 12%

cash_flows = [-50000, 10000, 15000, 20000, 25000, 30000]
required_rate = 0.12

npv = finmath.net_present_value(cash_flows, required_rate)
irr = finmath.internal_rate_of_return(cash_flows)

print(f"NPV: ${npv:,.2f}")
print(f"IRR: {irr*100:.2f}%")

if npv > 0:
    print("✓ Project is profitable - invest!")
else:
    print("✗ Project is not profitable - reject!")
```

### Example 3: Bond Pricing

```python
import finmath

# Bond: $1000 face value, 5% coupon, 10 years to maturity
# Semi-annual payments, 4% yield to maturity

face_value = 1000
coupon_rate = 0.05
ytm = 0.04
periods_per_year = 2
time_to_maturity = 10

price = finmath.bond_price(face_value, coupon_rate, ytm, 
                           periods_per_year, time_to_maturity)
duration = finmath.bond_duration(face_value, coupon_rate, ytm,
                                 periods_per_year, time_to_maturity)

print(f"Bond price: ${price:,.2f}")
print(f"Macaulay duration: {duration:.2f} years")
```

---

## Common Pitfalls & Best Practices

### Pitfall 1: Floating-Point Precision
**Problem:** Direct equality comparison of floating-point numbers
```cpp
// BAD
if (discount_factor(0.05, 1.0) == 0.952380952) { ... }

// GOOD
if (almost_equal(discount_factor(0.05, 1.0), 0.952380952, 1e-6)) { ... }
```

### Pitfall 2: Division by Zero
**Problem:** Not handling zero rate in annuity formulas
```cpp
// BAD
double pv = payment * (1 - std::pow(1 + rate, -periods)) / rate;
// Crashes when rate == 0

// GOOD
if (std::abs(rate) < 1e-10) {
    return payment * periods;  // Handle zero rate case
}
double pv = payment * (1 - std::pow(1 + rate, -periods)) / rate;
```

### Pitfall 3: Integer Division
**Problem:** Using integers where doubles are needed
```cpp
// BAD
double result = 1 / periods;  // Integer division!

// GOOD
double result = 1.0 / static_cast<double>(periods);
```

### Pitfall 4: Not Validating Inputs
**Problem:** Assuming inputs are always valid
```cpp
// BAD
double discount_factor(double rate, double time) {
    return 1.0 / std::pow(1.0 + rate, time);  // What if rate < 0?
}

// GOOD
double discount_factor(double rate, double time) {
    if (rate < 0 || time < 0) {
        throw std::invalid_argument("Rate and time must be non-negative");
    }
    return 1.0 / std::pow(1.0 + rate, time);
}
```

### Best Practices

1. **Always validate inputs** - Check for negative values, empty vectors, etc.
2. **Handle edge cases** - Zero rate, zero time, zero periods
3. **Use meaningful variable names** - `discount_factor` not `df`
4. **Document your formulas** - Add comments explaining the mathematics
5. **Test thoroughly** - Normal cases, edge cases, error cases
6. **Follow existing patterns** - Match the style of `compound_interest.cpp`
7. **Use appropriate data types** - `double` for rates, `int` for periods
8. **Throw meaningful exceptions** - Include context in error messages

---

## References & Resources

### Mathematical References

1. **Time Value of Money**
   - [Investopedia: Time Value of Money](https://www.investopedia.com/terms/t/timevalueofmoney.asp)
   - [Wikipedia: Present Value](https://en.wikipedia.org/wiki/Present_value)
   - [Khan Academy: Time Value of Money](https://www.khanacademy.org/economics-finance-domain/core-finance/interest-tutorial)

2. **Annuities**
   - [Investopedia: Annuity](https://www.investopedia.com/terms/a/annuity.asp)
   - [Investopedia: Present Value of Annuity](https://www.investopedia.com/retirement/calculating-present-and-future-value-of-annuities/)
   - [Wikipedia: Annuity](https://en.wikipedia.org/wiki/Annuity)

3. **Net Present Value & IRR**
   - [Investopedia: NPV](https://www.investopedia.com/terms/n/npv.asp)
   - [Investopedia: IRR](https://www.investopedia.com/terms/i/irr.asp)
   - [Corporate Finance Institute: NPV vs IRR](https://corporatefinanceinstitute.com/resources/valuation/npv-vs-irr/)

4. **Bond Pricing**
   - [Investopedia: Bond Pricing](https://www.investopedia.com/terms/b/bond-valuation.asp)
   - [Investopedia: Yield to Maturity](https://www.investopedia.com/terms/y/yieldtomaturity.asp)
   - [Investopedia: Duration](https://www.investopedia.com/terms/d/duration.asp)

### Implementation References

1. **QuantLib**
   - [QuantLib Documentation](https://www.quantlib.org/)
   - [QuantLib Reference Manual](https://www.quantlib.org/reference/)
   - [QuantLib Source Code](https://github.com/lballabio/QuantLib)

2. **C++ Best Practices**
   - [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
   - [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

3. **Financial Mathematics Textbooks**
   - "Mathematics of Finance" by Zima & Brown
   - "Options, Futures, and Other Derivatives" by Hull (Chapter 4: Interest Rates)
   - "Fixed Income Securities" by Tuckman & Serrat

### Online Calculators for Verification

1. **Financial Calculators**
   - [Calculator.net: Present Value](https://www.calculator.net/present-value-calculator.html)
   - [Calculator.net: Future Value](https://www.calculator.net/future-value-calculator.html)
   - [Calculator.net: NPV](https://www.calculator.net/npv-calculator.html)
   - [Calculator.net: IRR](https://www.calculator.net/irr-calculator.html)

2. **Excel Functions** (for verification)
   - `=PV(rate, nper, pmt, fv)` - Present Value
   - `=FV(rate, nper, pmt, pv)` - Future Value
   - `=NPV(rate, values)` - Net Present Value
   - `=IRR(values, guess)` - Internal Rate of Return

### Testing Resources

1. **Test Data Sources**
   - [Investopedia Examples](https://www.investopedia.com/) - Many articles include worked examples
   - [Corporate Finance Institute](https://corporatefinanceinstitute.com/) - Free courses with examples

2. **QuantLib Examples**
   - [QuantLib Examples](https://www.quantlib.org/example1.shtml) - Can be used to verify your implementations