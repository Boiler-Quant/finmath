# finmath

**finmath** is a high-performance financial mathematics library written in C++ with Python bindings using `pybind11`. The library includes various functions for calculating compound interest, option pricing (including Black-Scholes and Binomial Tree models), and time series analysis. The goal is to provide a fast and reliable tool for financial calculations that can be used in Python.

## Installation

### Prerequisites

- C++17 or later
- Python 3.6 or later
- `pybind11` (for Python bindings)
- `CMake` 3.5 or later (for building the library)
- `ninja` For optimizing cmake build

### Build Instructions

1. **Clone the repository**:

   ```bash
   git clone https://github.com/prajwalshah19/finmath.git
   cd finmath
   ```

2. **Create a build directory**:

   ```bash
   mkdir build
   cd build
   ```

3. **Run CMake**:

   ```bash
   cmake ..
   ```

4. **Build the library**:

   ```bash
   make
   ```

5. **Install the Python bindings** (optional):

   ```bash
   pip install .
   ```

This will build the `finmath` library and create the Python bindings so that you can use `finmath` directly in Python.

## Usage

### Python

After installing the `finmath` library, you can import and use it in Python:

```python
import finmath

# Example: Calculate compound interest
principal = 1000
rate = 5
time = 10
frequency = 4
result = finmath.compound_interest(principal, rate, time, frequency)
print(f"Compound Interest: {result}")

# Example: Calculate option price using Black-Scholes model
option_price = finmath.black_scholes(finmath.OptionType.CALL, 95, 100, 1, 0.05, 0.2)
print(f"Black-Scholes Option Price: {option_price}")

# Example: Calculate rolling volatility over a 22-day window
prices = [100, 101, 102, 100, 99, 98, 100, 102, 103, 104, 105]  # Example price series
vol = finmath.rolling_volatility(prices, 22)
print(f"Rolling Volatility: {vol}")
```

### C++

If you want to use the library directly in C++:

```cpp
#include "finmath/InterestAndAnnuities/compound_interest.h"
#include "finmath/OptionPricing/black_scholes.h"
#include "finmath/TimeSeries/rolling_volatility.h"
#include <iostream>
#include <vector>

int main() {
    double principal = 1000;
    double rate = 5;
    int time = 10;
    int frequency = 4;

    double compound_result = compound_interest(principal, rate, time, frequency);
    std::cout << "Compound Interest: " << compound_result << std::endl;

    std::vector<double> prices = {100, 101, 102, 100, 99, 98, 100, 102, 103, 104, 105};
    auto vol = rolling_volatility(prices, 22);
    std::cout << "Rolling Volatility: " << vol.back() << std::endl;

    return 0;
}
```

## Benchmarking

You can compare the performance of `finmath` functions with other implementations (e.g., `gs_quant`) to see the speedup provided by the C++ implementations:

```python
import timeit
import gs_quant.timeseries as ts
import functions
import finmath

# Example data for 1000 days
prices = ts.generate_series(1000)

# Timing the finmath C++ implementation of rolling volatility
def test_cpp_implementation():
    return finmath.rolling_volatility(prices.tolist(), 22)

cpp_time = timeit.timeit(test_cpp_implementation, number=100)
print(f"C++ implementation time: {cpp_time:.6f} seconds")

```

## Contributing

We welcome contributions to **finmath**! To contribute:

1. Fork the repository.
2. Create a new branch for your feature or bug fix:

   ```bash
   git checkout -b feature/my-new-feature

   ```

3. Commit your changes:

   ```bash
   git commit -m "Add some feature"

   ```

4. Push the branch to your fork:

   ```bash
   git push origin feature/my-new-feature

   ```

5. Open a Pull Request on the original repository.

## License

This project is licensed under the AGPL-3.0 License - see the [LICENSE](https://github.com/prajwalshah19/finmath/blob/main/LICENSE) file for details.

## Acknowledgments

- Inspired by the `gs_quant` library for financial time series analysis.
- Thanks to `pybind11` for making it easy to integrate C++ and Python.
