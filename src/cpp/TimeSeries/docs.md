# TimeSeries Functions Documentation

This document provides documentation for functions available in the `TimeSeries` module of the `FinMath` library, organized by category. Each function includes its description, syntax, parameters, return values, and usage examples.

---

## Table of Contents

- [Exponential Moving Average (EMA) Functions](#exponential-moving-average-ema-functions)
  - [ema_window](#ema_window)
  - [ema_smoothing](#ema_smoothing)
- [Relative Strength Index (RSI) Functions](#relative-strength-index-rsi-functions)
  - [rsi](#rsi)
- [Simple Moving Average (SMA) Functions](#simple-moving-average-sma-functions)
  - [simple_moving_average](#simple_moving_average)

---

## Exponential Moving Average (EMA) Functions

### `ema_window`

#### Description

Calculates the Exponential Moving Average (EMA) of a given price series over a specified window size. EMA gives more weight to recent prices, making it more responsive to new information.

#### Syntax

```python
def ema_window(prices: List[float], window_size: int) -> List[float]:
```

#### Parameters

- **prices** (`List[float]`): A list containing the price series data.
- **window_size** (`int`): The number of periods over which to calculate the EMA.

#### Returns

- **List[float]**: A list containing the EMA values corresponding to the input price series.

#### Usage Example

```python
from finmath.TimeSeries import ema_window

prices = [22.27, 22.19, 22.08, 22.17, 22.18, 22.13, 22.23, 22.43, 22.24, 22.29]
window_size = 10
ema = ema_window(prices, window_size)

print(ema)
# Output: [22.27, 22.19, 22.08, 22.17, 22.18, 22.13, 22.23, 22.43, 22.24, 22.29]
```

---

### `ema_smoothing`

#### Description

Calculates the Exponential Moving Average (EMA) of a given price series using a specified smoothing factor. This allows for customized weighting of the price data.

#### Syntax

```python
def ema_smoothing(prices: List[float], smoothing_factor: float) -> List[float]:
```

#### Parameters

- **prices** (`List[float]`): A list containing the price series data.
- **smoothing_factor** (`float`): The smoothing factor used in EMA calculation, typically `2.0 / (window_size + 1)`.

#### Returns

- **List[float]**: A list containing the EMA values corresponding to the input price series.

#### Usage Example

```python
from finmath.TimeSeries import ema_smoothing

prices = [22.27, 22.19, 22.08, 22.17, 22.18, 22.13, 22.23, 22.43, 22.24, 22.29]
smoothing_factor = 2.0 / (10 + 1)
ema = ema_smoothing(prices, smoothing_factor)

print(ema)
# Output: [22.27, 22.19, 22.08, 22.17, 22.18, 22.13, 22.23, 22.43, 22.24, 22.29]
```

---

## Relative Strength Index (RSI) Functions

### `rsi`

#### Description

Calculates the Relative Strength Index (RSI) for a given price series and window size. RSI is a momentum oscillator that measures the speed and change of price movements, typically used to identify overbought or oversold conditions.

#### Syntax

```python
def rsi(prices: List[float], window_size: int) -> List[float]:
```

#### Parameters

- **prices** (`List[float]`): A list containing the price series data.
- **window_size** (`int`): The number of periods over which to calculate the RSI.

#### Returns

- **List[float]**: A list containing the RSI values corresponding to the input price series.

#### Usage Example

```python
from finmath.TimeSeries import rsi

prices = [44.34, 44.09, 44.15, 43.61, 44.33, 44.83, 45.10, 45.42, 45.84, 46.08]
window_size = 14
rsi_values = rsi(prices, window_size)

print(rsi_values)
# Output: [70.53, 66.24, 66.48, ...]
```

---

## Simple Moving Average (SMA) Functions

### `simple_moving_average`

#### Description

Calculates the Simple Moving Average (SMA) of a given data series over a specified window size. SMA is the unweighted mean of the previous n data points.

#### Syntax

```python
def simple_moving_average(prices: List[float], window_size: int) -> List[float]:
```

#### Parameters

- **prices** (`List[float]`): A list containing the price series data.
- **window_size** (`int`): The number of periods over which to calculate the SMA.

#### Returns

- **List[float]**: A list containing the SMA values corresponding to the input price series.

#### Usage Example

```python
from finmath.TimeSeries import simple_moving_average

prices = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]
window_size = 3
sma = simple_moving_average(prices, window_size)

print(sma)
# Output: [20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0]
```

---
