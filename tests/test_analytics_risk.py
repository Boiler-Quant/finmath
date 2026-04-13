"""Tests for OLS, Markov chains, autocorrelation, returns, risk metrics, and Monte Carlo."""
import math

import pytest

try:
    import finmath
except ImportError as e:
    pytest.skip(f"finmath not built: {e}", allow_module_level=True)


def test_simple_interest():
    # Returns principal * (1 + rate * time)
    assert finmath.simple_interest(1000.0, 0.05, 2.0) == pytest.approx(1100.0)


def test_black_scholes_greeks_atm():
    S0, K, T, r, q, sigma = 100.0, 100.0, 1.0, 0.05, 0.0, 0.2
    d = finmath.black_scholes_delta(finmath.OptionType.CALL, S0, K, T, r, q, sigma)
    assert 0.4 < d < 0.7
    g = finmath.black_scholes_gamma(S0, K, T, r, q, sigma)
    assert g > 0
    assert finmath.black_scholes_vega(S0, K, T, r, q, sigma) > 0


def test_black_scholes_dividend_yield():
    p0 = finmath.black_scholes(finmath.OptionType.CALL, 100.0, 100.0, 1.0, 0.05, 0.2, 0.0)
    p1 = finmath.black_scholes(finmath.OptionType.CALL, 100.0, 100.0, 1.0, 0.05, 0.2, 0.03)
    assert p1 < p0


def test_markov_steady_state():
    P = [[0.9, 0.1], [0.5, 0.5]]
    pi = finmath.markov_steady_state(P)
    assert pi[0] == pytest.approx(5.0 / 6.0, rel=1e-5)
    assert pi[1] == pytest.approx(1.0 / 6.0, rel=1e-5)


def test_ols():
    X = [[1.0], [2.0], [3.0], [4.0]]
    y = [2.0, 4.0, 6.0, 8.0]
    m = finmath.OLS(X, y)
    assert m.intercept() == pytest.approx(0.0, abs=1e-6)
    assert m.coefficients()[0] == pytest.approx(2.0, rel=1e-6)


def test_autocorrelation_lag0():
    vals = [1.0, 2.0, 3.0, 4.0, 5.0]
    acf = finmath.autocorrelation(vals, 2)
    assert acf[0] == pytest.approx(1.0)


def test_log_returns():
    p = [100.0, 110.0]
    r = finmath.log_returns(p)
    assert r[1] == pytest.approx(math.log(1.1))


def test_historical_var_cvar():
    rets = [-0.05, -0.02, 0.01, 0.02, 0.03]
    var = finmath.historical_var(rets, 0.2)
    assert var >= 0
    cvar = finmath.historical_cvar(rets, 0.4)
    assert cvar >= 0


def test_parametric_var():
    v = finmath.parametric_var(0.0, 1.0, 0.05)
    assert v > 0


def test_monte_carlo_european():
    price = finmath.monte_carlo_european(
        finmath.OptionType.CALL, 100.0, 100.0, 1.0, 0.05, 0.0, 0.2, 50_000, 42
    )
    bs = finmath.black_scholes(finmath.OptionType.CALL, 100.0, 100.0, 1.0, 0.05, 0.2, 0.0)
    assert abs(price - bs) / bs < 0.02


def test_orderbook_mid():
    m = finmath.orderbook_midprice(99.0, 101.0)
    assert m == 100.0
