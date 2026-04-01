from __future__ import annotations

from typing import Any, Dict, List, Tuple


def book_to_arrays(book: Dict[str, Any], depth: int) -> Tuple[List[float], List[float], List[float], List[float]]:
    """
    Convert a simple book dict into (bid_prices, bid_sizes, ask_prices, ask_sizes).

    Expected shapes:
    - book["bids"] = [(price, size), ...] sorted best-to-worst
    - book["asks"] = [(price, size), ...] sorted best-to-worst
    """
    if depth <= 0:
        raise ValueError("depth must be > 0")
    bids = list(book.get("bids", []))[:depth]
    asks = list(book.get("asks", []))[:depth]

    bid_prices = [float(p) for p, _ in bids]
    bid_sizes = [float(s) for _, s in bids]
    ask_prices = [float(p) for p, _ in asks]
    ask_sizes = [float(s) for _, s in asks]

    return bid_prices, bid_sizes, ask_prices, ask_sizes

