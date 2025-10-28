#!/usr/bin/env python3
"""
Test suite for rolling_volatility_np function (NumPy version)

This tests the C++ implementation that accepts NumPy arrays.
You can debug the C++ code by setting breakpoints in:
  - src/cpp/TimeSeries/rolling_volatility.cpp (line 73+)

To debug in VS Code:
  1. Open rolling_volatility.cpp and set breakpoints
  2. Run this file with the Python debugger (F5)
  3. VS Code will stop at your C++ breakpoints!
"""

import sys
import os

# Add the src directory to Python path to import the compiled module
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '../../../../src'))

import numpy as np

try:
    from finmath import rolling_volatility_np
except ImportError as e:
    print(f"Error importing finmath module: {e}")
    print("\nMake sure you've built the project first:")
    print("  cd /Users/shashank/Desktop/finmath")
    print("  ./scripts/build.sh debug")
    sys.exit(1)


def approx_equal(a, b, tolerance=1e-6):
    """Helper to compare floating point numbers"""
    return abs(a - b) <= tolerance * max(1.0, max(abs(a), abs(b)))


def test_basic_calculation():
    """Test Case 1: Basic rolling volatility calculation"""
    print("Test Case 1: Basic calculation")
    
    prices = np.array([100.0, 101.0, 102.0, 100.0, 99.0, 98.0, 100.0, 102.0, 103.0, 104.0, 105.0])
    window = 5
    
    # Call the C++ function (set breakpoints in rolling_volatility.cpp!)
    result = rolling_volatility_np(prices, window)
    
    # Expected 6 volatility values
    expected_length = len(prices) - window
    assert len(result) == expected_length, f"Expected {expected_length} results, got {len(result)}"
    
    # Check that all values are positive (volatility can't be negative)
    for i, vol in enumerate(result):
        assert vol >= 0, f"Volatility at index {i} is negative: {vol}"
    
    print(f"  ✅ Result length: {len(result)}")
    print(f"  ✅ Sample values: {result[:3]}")
    print("  ✅ Test Case 1 Passed\n")


def test_different_window_sizes():
    """Test Case 2: Different window sizes"""
    print("Test Case 2: Different window sizes")
    
    prices = np.array([100.0, 101.0, 102.0, 103.0, 104.0, 105.0, 106.0, 107.0, 108.0, 109.0])
    
    for window in [2, 3, 5, 7]:
        result = rolling_volatility_np(prices, window)
        expected_length = len(prices) - window
        assert len(result) == expected_length, f"Window {window}: expected {expected_length} results, got {len(result)}"
        print(f"  ✅ Window {window}: {len(result)} results")
    
    print("  ✅ Test Case 2 Passed\n")


def test_edge_case_minimum_data():
    """Test Case 3: Edge case with minimum data"""
    print("Test Case 3: Edge case - minimum data")
    
    # Minimum is window+1 prices (need at least window log returns)
    prices = np.array([100.0, 101.0, 102.0])
    window = 2
    
    result = rolling_volatility_np(prices, window)
    assert len(result) == 1, f"Expected 1 result, got {len(result)}"
    
    print(f"  ✅ Result: {result[0]}")
    print("  ✅ Test Case 3 Passed\n")


def test_exception_insufficient_data():
    """Test Case 4: Exception - insufficient data"""
    print("Test Case 4: Exception - insufficient data")
    
    try:
        prices = np.array([100.0])  # Only 1 price
        rolling_volatility_np(prices, 1)
        assert False, "Should have raised an exception"
    except RuntimeError as e:
        assert "Insufficient data" in str(e)
        print(f"  ✅ Caught expected exception: {e}")
    
    print("  ✅ Test Case 4 Passed\n")


def test_exception_zero_window():
    """Test Case 5: Exception - zero window size"""
    print("Test Case 5: Exception - zero window size")
    
    try:
        prices = np.array([100.0, 101.0, 102.0])
        rolling_volatility_np(prices, 0)
        assert False, "Should have raised an exception"
    except RuntimeError as e:
        assert "Window size cannot be zero" in str(e)
        print(f"  ✅ Caught expected exception: {e}")
    
    print("  ✅ Test Case 5 Passed\n")


def test_exception_window_too_large():
    """Test Case 6: Exception - window too large"""
    print("Test Case 6: Exception - window too large")
    
    try:
        prices = np.array([100.0, 101.0, 102.0])
        rolling_volatility_np(prices, 3)  # Window >= num_prices
        assert False, "Should have raised an exception"
    except RuntimeError as e:
        assert "Window size must be smaller" in str(e)
        print(f"  ✅ Caught expected exception: {e}")
    
    print("  ✅ Test Case 6 Passed\n")


def test_exception_multidimensional_array():
    """Test Case 7: Exception - multi-dimensional array"""
    print("Test Case 7: Exception - multi-dimensional array")
    
    try:
        prices_2d = np.array([[100.0, 101.0], [102.0, 103.0]])
        rolling_volatility_np(prices_2d, 1)
        assert False, "Should have raised an exception"
    except RuntimeError as e:
        assert "1-dimensional" in str(e)
        print(f"  ✅ Caught expected exception: {e}")
    
    print("  ✅ Test Case 7 Passed\n")


def test_data_types():
    """Test Case 8: Different NumPy data types"""
    print("Test Case 8: Different data types")
    
    prices_int = np.array([100, 101, 102, 103, 104], dtype=np.int32)
    prices_float32 = np.array([100.0, 101.0, 102.0, 103.0, 104.0], dtype=np.float32)
    prices_float64 = np.array([100.0, 101.0, 102.0, 103.0, 104.0], dtype=np.float64)
    
    # float64 should work
    result_f64 = rolling_volatility_np(prices_float64, 2)
    assert len(result_f64) == 3
    print("  ✅ float64 works")
    
    # Other types might need conversion - test graceful handling
    try:
        result_int = rolling_volatility_np(prices_int.astype(np.float64), 2)
        print("  ✅ Converted int32 to float64 works")
    except RuntimeError as e:
        print(f"  ⚠️  int32 conversion note: {e}")
    
    print("  ✅ Test Case 8 Passed\n")


def test_realistic_stock_prices():
    """Test Case 9: Realistic stock price scenario"""
    print("Test Case 9: Realistic stock prices")
    
    # Simulate 30 days of stock prices with some volatility
    np.random.seed(42)
    base_price = 100.0
    returns = np.random.normal(0.001, 0.02, 30)  # ~1% drift, 2% daily vol
    prices = base_price * np.exp(np.cumsum(returns))
    
    window = 20  # 20-day rolling volatility
    result = rolling_volatility_np(prices, window)
    
    expected_length = len(prices) - window
    assert len(result) == expected_length
    
    print(f"  ✅ 30 days of prices, 20-day window")
    print(f"  ✅ Generated {len(result)} volatility values")
    print(f"  ✅ Mean volatility: {np.mean(result):.4f}")
    print(f"  ✅ Min/Max: {np.min(result):.4f} / {np.max(result):.4f}")
    print("  ✅ Test Case 9 Passed\n")


def run_all_tests():
    """Run all test cases"""
    print("=" * 60)
    print("Testing rolling_volatility_np (NumPy C++ implementation)")
    print("=" * 60)
    print()
    
    tests = [
        test_basic_calculation,
        test_different_window_sizes,
        test_edge_case_minimum_data,
        test_exception_insufficient_data,
        test_exception_zero_window,
        test_exception_window_too_large,
        test_exception_multidimensional_array,
        test_data_types,
        test_realistic_stock_prices,
    ]
    
    failed = []
    for test in tests:
        try:
            test()
        except Exception as e:
            print(f"  ❌ {test.__name__} FAILED: {e}\n")
            failed.append((test.__name__, e))
    
    print("=" * 60)
    if not failed:
        print("✅ ALL TESTS PASSED!")
        print("=" * 60)
        return 0
    else:
        print(f"❌ {len(failed)} TEST(S) FAILED:")
        for name, error in failed:
            print(f"   - {name}: {error}")
        print("=" * 60)
        return 1


if __name__ == "__main__":
    exit_code = run_all_tests()
    sys.exit(exit_code)

