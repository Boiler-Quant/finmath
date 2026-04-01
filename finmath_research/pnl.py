from __future__ import annotations

from dataclasses import dataclass


@dataclass
class PnLTracker:
    """
    Minimal inventory + cash tracker for toy simulations.
    Convention: buying decreases cash, selling increases cash.
    """

    cash: float = 0.0
    position: float = 0.0

    def trade(self, qty: float, price: float) -> None:
        if price < 0:
            raise ValueError("price must be >= 0")
        self.position += qty
        self.cash -= qty * price

    def mark_to_market(self, mid_price: float) -> float:
        return self.cash + self.position * mid_price

