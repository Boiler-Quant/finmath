"""Tests for time series Python bindings (SMA, RSI, EMA, rolling volatility)."""
import pytest

try:
    import finmath
except ImportError as e:
    pytest.skip(f"finmath not built or not on PYTHONPATH: {e}", allow_module_level=True)


def test_simple_moving_average_list():
    prices = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0]
    result = finmath.simple_moving_average(prices, 3)
    assert len(result) == len(prices) - 3 + 1
    assert result[0] == pytest.approx(2.0)   # (1+2+3)/3
    assert result[-1] == pytest.approx(9.0)  # (8+9+10)/3


def test_simple_moving_average_simd_numpy():
    import numpy as np
    prices = np.array([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0])
    result = finmath.simple_moving_average_simd(prices, 3)
    assert len(result) == len(prices) - 3 + 1
    assert result[0] == pytest.approx(2.0)
    assert result[-1] == pytest.approx(9.0)


def test_smoothed_rsi_list():
    # Classic 14-period RSI example (e.g. first 15 prices)
    prices = [44.34, 44.09, 44.15, 43.61, 44.33, 44.83, 45.10, 45.42, 45.84, 46.08, 45.89, 46.03, 45.61, 46.28, 46.28]
    result = finmath.smoothed_rsi(prices, 14)
    assert len(result) >= 1
    assert 0 <= result[-1] <= 100


def test_smoothed_rsi_simd_numpy():
    import numpy as np
    prices = np.array([44.34, 44.09, 44.15, 43.61, 44.33, 44.83, 45.10, 45.42, 45.84, 46.08])
    result = finmath.smoothed_rsi_simd(prices, 5)
    assert len(result) >= 1
    assert all(0 <= r <= 100 for r in result)


def test_rolling_volatility_list():
    prices = [100.0, 101.0, 102.0, 101.0, 100.0, 99.0, 100.0, 102.0]
    result = finmath.rolling_volatility(prices, 3)
    assert len(result) >= 1
    assert all(r >= 0 for r in result)


def test_rolling_volatility_simd_numpy():
    import numpy as np
    prices = np.array([100.0, 101.0, 102.0, 101.0, 100.0, 99.0, 100.0, 102.0])
    result = finmath.rolling_volatility_simd(prices, 3)
    assert len(result) >= 1
    assert all(r >= 0 for r in result)


def test_ema_window_list():
    prices = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0]
    result = finmath.ema_window(prices, 3)
    assert len(result) == len(prices)
    assert result[-1] > 0


def test_ema_window_simd_numpy():
    import numpy as np
    prices = np.array([1.0, 2.0, 3.0, 4.0, 5.0])
    result = finmath.ema_window_simd(prices, 2)
    assert len(result) == len(prices)
    assert result[-1] > 0


def test_get_simd_backend():
    backend = finmath.get_simd_backend()
    assert isinstance(backend, str)
    assert backend  # non-empty
