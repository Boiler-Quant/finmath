# Relative Strength Index (RSI)

## Overview

The Relative Strength Index (RSI) is a momentum oscillator that measures the speed and magnitude of price movements to identify overbought or oversold conditions in a market.

## Function Signature

```cpp
std::vector<double> compute_smoothed_rsi(const std::vector<double>& prices, size_t window_size);
```

## Parameters

- `prices` (std::vector<double>): Historical price data.
- `window_size` (size_t): The period length used for the RSI calculation.

## Returns

- (std::vector<double>): A list of RSI values for each eligible index in the original price array.

## Example Usage

```cpp
#include "finmath/TimeSeries/rsi.h"
// ...existing code...
std::vector<double> prices = {44.34, 44.09, 44.15, 43.61, 44.33};
std::vector<double> rsi_vals = compute_smoothed_rsi(prices, 14);
// ...existing code...
```

## Python Usage

```python
import finmath
# ...existing code...
prices = [44.34, 44.09, 44.15, 43.61, 44.33]
rsi_vals = finmath.smoothed_rsi(prices, 14)
# ...existing code...
```

## Mathematical Formula

RSI is commonly calculated as follows:
RS = (Avg. of gains over N periods) / (Avg. of losses over N periods)

RSI = 100 - (100 / (1 + RS))
