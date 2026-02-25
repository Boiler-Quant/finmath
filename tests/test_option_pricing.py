"""Tests for Black-Scholes and binomial option pricing Python bindings."""
import pytest

try:
    import finmath
except ImportError as e:
    pytest.skip(f"finmath not built or not on PYTHONPATH: {e}", allow_module_level=True)


# Black-Scholes: (type, strike, price, time, rate, volatility) per bindings
def test_black_scholes_call():
    price = finmath.black_scholes(
        finmath.OptionType.CALL, 105, 100, 1.0, 0.05, 0.2
    )
    assert price > 0
    assert price < 100  # call < spot for OTM/ATM-ish


def test_black_scholes_put():
    price = finmath.black_scholes(
        finmath.OptionType.PUT, 95, 100, 1.0, 0.05, 0.2
    )
    assert price > 0
    assert price < 100


def test_binomial_option_pricing_call():
    price = finmath.binomial_option_pricing(
        finmath.OptionType.CALL, 100.0, 100.0, 1.0, 0.05, 0.2, 100
    )
    assert price > 0
    assert price < 100


def test_binomial_option_pricing_put():
    price = finmath.binomial_option_pricing(
        finmath.OptionType.PUT, 100.0, 100.0, 1.0, 0.05, 0.2, 100
    )
    assert price > 0
    assert price < 100


def test_black_scholes_vs_binomial_convergence():
    """Binomial with large N should be close to Black-Scholes."""
    S0, K, T, r, sigma = 100.0, 105.0, 1.0, 0.05, 0.2
    bs = finmath.black_scholes(finmath.OptionType.CALL, K, S0, T, r, sigma)
    bin_price = finmath.binomial_option_pricing(
        finmath.OptionType.CALL, S0, K, T, r, sigma, 500
    )
    assert abs(bs - bin_price) / max(bs, 1e-9) < 0.05  # within 5%
