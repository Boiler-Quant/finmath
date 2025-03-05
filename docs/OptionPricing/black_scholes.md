# Black-Scholes Option Pricing Model

## Overview

The Black-Scholes model is a mathematical model for pricing an options contract. The model assumes the market is efficient and the price of the underlying asset follows a geometric Brownian motion with constant drift and volatility.

## Function Signature

```cpp
double black_scholes(OptionType type, double strike, double price, double time, double rate, double volatility);
```

## Parameters

- `type` (OptionType): The type of the option (CALL or PUT).
- `strike` (double): The strike price of the option.
- `price` (double): The current price of the underlying asset.
- `time` (double): The time to maturity (in years).
- `rate` (double): The risk-free interest rate (annualized).
- `volatility` (double): The volatility of the underlying asset (annualized).

## Returns

- (double): The price of the option.

## Example Usage

```cpp
#include "finmath/OptionPricing/black_scholes.h"

int main() {
    double call_price = black_scholes(OptionType::CALL, 100, 105, 1, 0.05, 0.2);
    double put_price = black_scholes(OptionType::PUT, 100, 95, 1, 0.05, 0.2);

    std::cout << "Call Option Price: " << call_price << std::endl;
    std::cout << "Put Option Price: " << put_price << std::endl;

    return 0;
}
```

## Python Usage

```python
import finmath

# Example: pricing a call option using Black-Scholes
option_price_call = finmath.black_scholes(
    finmath.OptionType.CALL,
    100.0,  # strike
    105.0,  # current price
    1.0,    # time (in years)
    0.05,   # risk-free rate
    0.2     # volatility
)
print("Call Option Price:", option_price_call)

# Example: pricing a put option
option_price_put = finmath.black_scholes(
    finmath.OptionType.PUT,
    100.0,
    95.0,
    1.0,
    0.05,
    0.2
)
print("Put Option Price:", option_price_put)
```

## Mathematical Formula

The Black-Scholes formula for a call option is:

\[ C = S_0 \Phi(d_1) - K e^{-rT} \Phi(d_2) \]

For a put option, the formula is:

\[ P = K e^{-rT} \Phi(-d_2) - S_0 \Phi(-d_1) \]

Where:

\[ d_1 = \frac{\ln(S_0 / K) + (r + \sigma^2 / 2) T}{\sigma \sqrt{T}} \]

\[ d_2 = d_1 - \sigma \sqrt{T} \]

- \( \Phi \) is the cumulative distribution function of the standard normal distribution.
- \( S_0 \) is the current price of the underlying asset.
- \( K \) is the strike price.
- \( r \) is the risk-free interest rate.
- \( \sigma \) is the volatility.
- \( T \) is the time to maturity.
