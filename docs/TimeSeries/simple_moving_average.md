# Simple Moving Average (SMA)

## Overview

A simple moving average (SMA) is calculated by summing a series of data points and dividing by the number of points in the series. This technique smooths out short-term fluctuations and highlights longer-term trends.

## Function Signature

```cpp
std::vector<double> simple_moving_average(const std::vector<double>& data, size_t window_size);
```

## Parameters

- `data` (std::vector<double>): Historical price or data series.
- `window_size` (size_t): Number of data points used to calculate each average.

## Returns

- (std::vector<double>): A list of moving averages for each position where a full window was available.

## Example Usage

### C++ Example

```cpp
#include "finmath/TimeSeries/simple_moving_average.h"
// ...existing code...
std::vector<double> data = {10.0, 11.0, 12.5, 13.0, 12.8};
// ...existing code...
std::vector<double> sma_vals = simple_moving_average(data, 3);
// ...existing code...
```

### Python Example

```python
import finmath
# ...existing code...
data = [10.0, 11.0, 12.5, 13.0, 12.8]
# ...existing code...
sma_vals = finmath.simple_moving_average(data, 3)
# ...existing code...
```

## Mathematical Formula

SMA = (Sum of N data points) / N
