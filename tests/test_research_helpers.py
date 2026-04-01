import pytest


def test_align_series_truncates_to_min_len():
    from finmath_research.features import align_series

    a, b, c = align_series([1, 2, 3], [10, 20], [7, 8, 9, 10])
    assert a == [1, 2]
    assert b == [10, 20]
    assert c == [7, 8]


def test_warmup_mask():
    from finmath_research.features import warmup_mask

    assert warmup_mask(5, 2) == [False, False, True, True, True]


def test_book_to_arrays_depth():
    from finmath_research.orderbook import book_to_arrays

    book = {"bids": [(99, 10), (98, 5)], "asks": [(101, 1), (102, 2)]}
    bid_p, bid_s, ask_p, ask_s = book_to_arrays(book, depth=1)
    assert bid_p == [99.0]
    assert bid_s == [10.0]
    assert ask_p == [101.0]
    assert ask_s == [1.0]


def test_pnl_tracker_mtm():
    from finmath_research.pnl import PnLTracker

    t = PnLTracker()
    t.trade(qty=2, price=100)   # buy 2
    t.trade(qty=-1, price=110)  # sell 1
    assert t.position == pytest.approx(1.0)
    assert t.cash == pytest.approx(-90.0)
    assert t.mark_to_market(105) == pytest.approx(15.0)

