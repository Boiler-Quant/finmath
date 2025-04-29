import pytest
import numpy as np
import finmath

# Test data
prices_list = [100, 101, 102, 100, 99, 98, 100, 102, 103, 104, 105]
prices_np = np.array(prices_list, dtype=np.float64)
window = 5
smoothing = 0.5 # Example smoothing factor

constant_prices = [100.0] * 20
constant_prices_np = np.array(constant_prices)

# Use list versions to get expected results
expected_ema_w = finmath.ema_window(prices_list, window)
expected_ema_s = finmath.ema_smoothing(prices_list, smoothing)
expected_ema_w_const = finmath.ema_window(constant_prices, window)
expected_ema_s_const = finmath.ema_smoothing(constant_prices, smoothing)

# --- EMA Window Tests --- 

def test_ema_window_list_input():
    result = finmath.ema_window(prices_list, window)
    assert isinstance(result, list)
    assert len(result) == len(expected_ema_w)
    np.testing.assert_allclose(result, expected_ema_w, rtol=1e-6)

def test_ema_window_numpy_input():
    result_np = finmath.ema_window(prices_np, window)
    assert isinstance(result_np, list)
    assert len(result_np) == len(expected_ema_w)
    np.testing.assert_allclose(result_np, expected_ema_w, rtol=1e-6)

def test_ema_window_constant():
    """EMA (window) of constant series should be constant."""
    # List
    res_list = finmath.ema_window(constant_prices, window)
    assert len(res_list) == len(expected_ema_w_const)
    np.testing.assert_allclose(res_list, expected_ema_w_const)
    assert all(abs(x - 100.0) < 1e-9 for x in res_list)
    # NumPy
    res_np = finmath.ema_window(constant_prices_np, window)
    assert len(res_np) == len(expected_ema_w_const)
    np.testing.assert_allclose(res_np, expected_ema_w_const)
    assert all(abs(x - 100.0) < 1e-9 for x in res_np)

def test_ema_window_edge_cases():
    # List
    with pytest.raises(RuntimeError, match="EMA window cannot be zero"):
        finmath.ema_window([1.0], 0)
    assert finmath.ema_window([], 5) == []
    # Numpy
    with pytest.raises(RuntimeError, match="EMA window cannot be zero"):
        finmath.ema_window(np.array([1.0]), 0)
    assert finmath.ema_window(np.array([]), 5) == []
    print("Skipping empty NumPy array test for EMA Window...")
    with pytest.raises(RuntimeError, match="Input array must be 1-dimensional"):
        finmath.ema_window(np.array([[1.0]]), 5)

# --- EMA Smoothing Factor Tests --- 

def test_ema_smoothing_list_input():
    result = finmath.ema_smoothing(prices_list, smoothing)
    assert isinstance(result, list)
    assert len(result) == len(expected_ema_s)
    np.testing.assert_allclose(result, expected_ema_s, rtol=1e-6)

def test_ema_smoothing_numpy_input():
    result_np = finmath.ema_smoothing(prices_np, smoothing)
    assert isinstance(result_np, list)
    assert len(result_np) == len(expected_ema_s)
    np.testing.assert_allclose(result_np, expected_ema_s, rtol=1e-6)

def test_ema_smoothing_constant():
    """EMA (smoothing) of constant series should be constant."""
    # List
    res_list = finmath.ema_smoothing(constant_prices, smoothing)
    assert len(res_list) == len(expected_ema_s_const)
    np.testing.assert_allclose(res_list, expected_ema_s_const)
    assert all(abs(x - 100.0) < 1e-9 for x in res_list)
    # NumPy
    res_np = finmath.ema_smoothing(constant_prices_np, smoothing)
    assert len(res_np) == len(expected_ema_s_const)
    np.testing.assert_allclose(res_np, expected_ema_s_const)
    assert all(abs(x - 100.0) < 1e-9 for x in res_np)

def test_ema_smoothing_edge_cases():
    # List
    with pytest.raises(RuntimeError, match="EMA smoothing factor must be between 0 and 1"):
        finmath.ema_smoothing([1.0], 0)
    with pytest.raises(RuntimeError, match="EMA smoothing factor must be between 0 and 1"):
        finmath.ema_smoothing([1.0], 1)
    assert finmath.ema_smoothing([], 0.5) == []
    # Numpy
    with pytest.raises(RuntimeError, match="EMA smoothing factor must be between 0 and 1"):
        finmath.ema_smoothing(np.array([1.0]), 0)
    with pytest.raises(RuntimeError, match="EMA smoothing factor must be between 0 and 1"):
        finmath.ema_smoothing(np.array([1.0]), 1.5)
    assert finmath.ema_smoothing(np.array([]), 0.5) == []
    print("Skipping empty NumPy array test for EMA Smoothing...")
    with pytest.raises(RuntimeError, match="Input array must be 1-dimensional"):
        finmath.ema_smoothing(np.array([[1.0]]), 0.5) 