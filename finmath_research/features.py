from __future__ import annotations

from dataclasses import dataclass
from typing import Iterable, List, Sequence, Tuple


def align_series(*series: Sequence[float]) -> Tuple[List[float], ...]:
    """
    Truncate all series to the same length (minimum length).
    Useful when combining indicators with different warmup behavior.
    """
    if not series:
        return tuple()
    n = min(len(s) for s in series)
    return tuple(list(s[:n]) for s in series)


def warmup_mask(length: int, warmup: int) -> List[bool]:
    """
    False for indices < warmup, True afterwards.
    """
    if length < 0:
        raise ValueError("length must be >= 0")
    if warmup < 0:
        raise ValueError("warmup must be >= 0")
    return [i >= warmup for i in range(length)]

