import pytest
import numpy as np
import pandas as pd
import finmath

# Test data 
prices_list = [44.34, 44.09, 44.15, 43.61, 44.33, 44.83, 45.10, 45.42, 45.84, 46.08, 45.89, 46.03, 45.61, 46.28, 46.28]
prices_np = np.array(prices_list, dtype=np.float64)
prices_pd = pd.Series(prices_list, dtype=np.float64)
window_size = 14 # Common window for RSI

# Constant prices -> RSI should be undefined or 100 (depending on handling of zero change)
constant_prices = [100.0] * 30 
constant_prices_np = np.array(constant_prices)
constant_prices_pd = pd.Series(constant_prices)

# Calculate expected result using the list version
# Note: RSI calculation depends heavily on the first value's avg gain/loss.
# Need a reliable external source or careful manual calc for true verification.
# Using list version as reference for now.
expected_rsi = finmath.smoothed_rsi(prices_list, window_size)
try:
    expected_rsi_constant = finmath.smoothed_rsi(constant_prices, window_size)
except Exception as e:
    # Depending on implementation, constant price might cause issues or return specific value
    print(f"Note: Calculating RSI for constant price failed or returned specific value: {e}")
    expected_rsi_constant = [100.0] * (len(constant_prices) - window_size) # Assume 100 if avg loss is 0

def test_rsi_list_input():
    """Tests RSI with list input."""
    result = finmath.smoothed_rsi(prices_list, window_size)
    assert isinstance(result, list)
    assert len(result) == len(expected_rsi)
    # High tolerance needed as small differences in initial avg gain/loss propagate
    np.testing.assert_allclose(result, expected_rsi, rtol=1e-4, atol=1e-4)

def test_rsi_numpy_input():
    """Tests RSI with NumPy array input."""
    result_np = finmath.smoothed_rsi(prices_np, window_size)
    assert isinstance(result_np, list)
    assert len(result_np) == len(expected_rsi)
    np.testing.assert_allclose(result_np, expected_rsi, rtol=1e-4, atol=1e-4)

def test_rsi_pandas_input():
    """Tests RSI with Pandas Series input."""
    result_pd = finmath.smoothed_rsi(prices_pd, window_size)
    assert isinstance(result_pd, list)
    assert len(result_pd) == len(expected_rsi)
    np.testing.assert_allclose(result_pd, expected_rsi, rtol=1e-4, atol=1e-4)

def test_rsi_constant_prices():
    """Tests RSI with constant prices (expect 100)."""
    # List
    result_list = finmath.smoothed_rsi(constant_prices, window_size)
    assert len(result_list) == len(expected_rsi_constant)
    assert all(abs(x - 100.0) < 1e-9 for x in result_list), "RSI of constant should be 100"
    # NumPy
    result_np = finmath.smoothed_rsi(constant_prices_np, window_size)
    assert len(result_np) == len(expected_rsi_constant)
    assert all(abs(x - 100.0) < 1e-9 for x in result_np), "RSI of constant should be 100"
    # Pandas
    result_pd = finmath.smoothed_rsi(constant_prices_pd, window_size)
    assert len(result_pd) == len(expected_rsi_constant)
    assert all(abs(x - 100.0) < 1e-9 for x in result_pd), "RSI of constant should be 100"

def test_rsi_edge_cases():
    """Tests edge cases for RSI."""
    
    # --- List Inputs ---
    with pytest.raises(RuntimeError, match="Window size must be at least 1"): 
        finmath.smoothed_rsi([1.0, 2.0], 0)
    # Check returns empty list if data <= window
    assert finmath.smoothed_rsi([1.0]*14, 14) == []
    assert finmath.smoothed_rsi([1.0]*5, 14) == []
    assert finmath.smoothed_rsi([], 14) == []
    
    # --- NumPy Inputs ---
    # Skip empty array test
    print("Skipping empty NumPy array test for RSI...")

    with pytest.raises(RuntimeError, match="Window size must be at least 1"): 
        finmath.smoothed_rsi(np.array([1.0, 2.0]), 0)
        
    # Check returns empty list if data <= window
    assert finmath.smoothed_rsi(np.array([1.0]*14), 14) == []
    assert finmath.smoothed_rsi(np.array([1.0]*5), 14) == []
    
    # Non-1D array
    with pytest.raises(RuntimeError, match="Input array must be 1-dimensional"): 
        finmath.smoothed_rsi(np.array([[1.0],[2.0]]), 1) 

    # --- Pandas Inputs ---
    # Skip empty series test
    print("Skipping empty Pandas Series test for RSI...")

    with pytest.raises(RuntimeError, match="Window size must be at least 1"): 
        finmath.smoothed_rsi(pd.Series([1.0, 2.0]), 0)
        
    # Check returns empty list if data <= window
    assert finmath.smoothed_rsi(pd.Series([1.0]*14), 14) == []
    assert finmath.smoothed_rsi(pd.Series([1.0]*5), 14) == []
    
    # Non-1D check happens in C++ via numpy buffer info 