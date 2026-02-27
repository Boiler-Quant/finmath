#!/usr/bin/env python3
"""
Benchmark rolling_volatility (scalar) vs rolling_volatility_simd.
Run from repo root. Use --scalar or --simd to profile one path; default runs both and reports timing.

  python profiling/run_rolling_volatility_bench.py --scalar   # scalar only
  python profiling/run_rolling_volatility_bench.py --simd       # SIMD only
  python profiling/run_rolling_volatility_bench.py            # both, print comparison

For perf on Linux:
  ./profiling/profile perf-stat rolling_volatility
  ./profiling/profile perf-stat python -- python profiling/run_rolling_volatility_bench.py --simd
"""

import sys
import time
import argparse

def main():
    parser = argparse.ArgumentParser(description="Benchmark rolling_volatility vs rolling_volatility_simd")
    parser.add_argument("--scalar", action="store_true", help="Run only scalar (vector) path")
    parser.add_argument("--simd", action="store_true", help="Run only SIMD (NumPy) path")
    parser.add_argument("--size", type=int, default=100_000, help="Number of prices (default 100000)")
    parser.add_argument("--window", type=int, default=252, help="Window size (default 252)")
    parser.add_argument("--iterations", type=int, default=50, help="Iterations per path (default 50)")
    args = parser.parse_args()

    if not args.scalar and not args.simd:
        args.scalar = args.simd = True

    # Ensure we can import finmath (repo root or src)
    try:
        import finmath
    except ImportError:
        sys.path.insert(0, "src")
        import finmath

    import numpy as np
    np.random.seed(42)
    prices = np.abs(np.cumsum(np.random.randn(args.size)) + 100.0).astype(np.float64)
    prices_list = prices.tolist()

    def run_scalar():
        for _ in range(args.iterations):
            finmath.rolling_volatility(prices_list, args.window)

    def run_simd():
        for _ in range(args.iterations):
            finmath.rolling_volatility_simd(prices, args.window)

    if args.scalar:
        t0 = time.perf_counter()
        run_scalar()
        elapsed = time.perf_counter() - t0
        print(f"rolling_volatility (scalar): {elapsed:.4f}s  ({args.iterations} iter, n={args.size}, window={args.window})")
    if args.simd:
        t0 = time.perf_counter()
        run_simd()
        elapsed = time.perf_counter() - t0
        print(f"rolling_volatility_simd:      {elapsed:.4f}s  ({args.iterations} iter, n={args.size}, window={args.window})")

    if args.scalar and args.simd:
        # Run again for a quick ratio (same order)
        t_scalar = time.perf_counter()
        run_scalar()
        t_scalar = time.perf_counter() - t_scalar
        t_simd = time.perf_counter()
        run_simd()
        t_simd = time.perf_counter() - t_simd
        ratio = t_scalar / t_simd if t_simd > 0 else 0
        print(f"Ratio (scalar/simd): {ratio:.2f}x")

if __name__ == "__main__":
    main()
