# Option Pricing Documentation

This document provides documentation for functions available in the
**OptionPricing** module of the `FinMath` library.\
It includes Black--Scholes pricing and the associated option Greeks.

------------------------------------------------------------------------

## Table of Contents

-   Black--Scholes Pricing
-   Option Greeks
    -   Delta
    -   Gamma
    -   Vega
    -   Theta
    -   Rho

------------------------------------------------------------------------

# Black--Scholes Pricing

## black_scholes

Computes the theoretical price of a European call or put option using
the Black--Scholes model.

### Syntax

``` cpp
double black_scholes(
    OptionType type,
    double strike,
    double price,
    double time,
    double rate,
    double volatility
);
```

### Parameters

-   **type**: OptionType --- CALL or PUT option
-   **strike**: K, strike price
-   **price**: S, current underlying price
-   **time**: t, time to maturity in years
-   **rate**: r, risk-free interest rate
-   **volatility**: sigma, asset volatility

### Returns

-   Option price as a double
-   Returns NaN if inputs are invalid:
    -   strike ≤ 0
    -   price ≤ 0
    -   time ≤ 0
    -   volatility \< 0

### Formula

Call: C = S N(d1) - K e\^{-rt} N(d2)

Put: P = K e\^{-rt} N(-d2) - S N(-d1)

------------------------------------------------------------------------

## Example

``` cpp
double price = black_scholes(
    OptionType::CALL,
    100.0,
    105.0,
    1.0,
    0.05,
    0.2
);
```

------------------------------------------------------------------------

# Option Greeks

All Greeks are implemented in the BlackScholes namespace and assume
continuous dividend yield q.

------------------------------------------------------------------------

## Delta

### compute_delta

Measures sensitivity of option price to underlying price changes.

``` cpp
double compute_delta(
    OptionType type,
    double S0,
    double K,
    double t,
    double r,
    double q,
    double sigma
);
```

------------------------------------------------------------------------

## Gamma

### compute_gamma

Measures rate of change of delta with respect to underlying price.

``` cpp
double compute_gamma(
    double S0,
    double K,
    double t,
    double r,
    double q,
    double sigma
);
```

------------------------------------------------------------------------

## Vega

### compute_vega

Measures sensitivity to volatility.

``` cpp
double compute_vega(
    double S0,
    double K,
    double t,
    double r,
    double q,
    double sigma
);
```

------------------------------------------------------------------------

## Theta

### compute_theta

Measures time decay.

``` cpp
double compute_theta(
    OptionType type,
    double S0,
    double K,
    double t,
    double T,
    double r,
    double q,
    double sigma
);
```

------------------------------------------------------------------------

## Rho

### compute_rho

Measures sensitivity to interest rate changes.

``` cpp
double compute_rho(
    OptionType type,
    double S0,
    double K,
    double t,
    double r,
    double q,
    double sigma
);
```

------------------------------------------------------------------------

# Notes

-   European options only
-   Black--Scholes assumptions
-   Invalid inputs return NaN

