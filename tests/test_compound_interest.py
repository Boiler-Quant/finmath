"""Tests for compound_interest Python binding."""
import pytest

try:
    import finmath
except ImportError as e:
    pytest.skip(f"finmath not built or not on PYTHONPATH: {e}", allow_module_level=True)


def test_compound_interest_basic():
    result = finmath.compound_interest(1000, 5, 10, 1)
    assert 1600 < result < 1650  # 1000 * (1.05)^10 ≈ 1628.89


def test_compound_interest_quarterly():
    result = finmath.compound_interest(1000, 5, 10, 4)
    assert 1640 < result < 1650  # quarterly compounding


def test_compound_interest_zero_principal():
    result = finmath.compound_interest(0, 5, 10, 1)
    assert result == 0.0


def test_compound_interest_zero_rate():
    result = finmath.compound_interest(1000, 0, 10, 1)
    assert result == 1000.0
