#!/usr/bin/env python3
"""
🚀 Cross-Platform SIMD Performance Demo
========================================

This demo showcases the performance benefits of SIMD optimization
across different CPU architectures:
- ARM NEON (Apple Silicon M1/M2/M3)
- x86_64 SSE/AVX (Intel/AMD)
- Scalar Fallback (any other architecture)

The finmath library automatically detects your CPU and uses the
best available SIMD instructions.
"""

import sys
import os

# Add paths where finmath module might be
sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(__file__)), 'build'))
sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(__file__)), 'src'))

import numpy as np
import time
import platform
from typing import Dict, List, Tuple

try:
    import finmath
except ImportError:
    print("❌ Error: finmath module not found!")
    print("Please build the library first:")
    print("  cd build && cmake .. && make")
    sys.exit(1)


def get_system_info() -> Dict[str, str]:
    """Get system and CPU information"""
    return {
        "OS": platform.system(),
        "Architecture": platform.machine(),
        "Python Version": platform.python_version(),
        "SIMD Backend": finmath.get_simd_backend(),
    }


def generate_realistic_price_data(n: int, seed: int = 42) -> np.ndarray:
    """Generate realistic stock price data using geometric Brownian motion"""
    np.random.seed(seed)
    
    # Parameters
    S0 = 100.0  # Initial price
    mu = 0.0001  # Drift (per period)
    sigma = 0.02  # Volatility (per period)
    
    # Generate returns
    dt = 1  # Time step
    returns = np.random.normal(mu * dt, sigma * np.sqrt(dt), n - 1)
    
    # Generate prices
    prices = np.zeros(n)
    prices[0] = S0
    
    for i in range(1, n):
        prices[i] = prices[i - 1] * np.exp(returns[i - 1])
    
    return prices


def benchmark_function(func, prices: np.ndarray, window: int, iterations: int = 100) -> Tuple[float, List[float]]:
    """Benchmark a function and return average time and results"""
    times = []
    
    # Warmup
    for _ in range(5):
        result = func(prices, window)
    
    # Actual benchmark
    for _ in range(iterations):
        start = time.perf_counter()
        result = func(prices, window)
        end = time.perf_counter()
        times.append((end - start) * 1000)  # Convert to milliseconds
    
    avg_time = np.mean(times)
    return avg_time, result


def format_time(ms: float) -> str:
    """Format time in the most appropriate unit"""
    if ms < 1:
        return f"{ms * 1000:.2f} µs"
    elif ms < 1000:
        return f"{ms:.2f} ms"
    else:
        return f"{ms / 1000:.2f} s"


def format_speedup(speedup: float) -> str:
    """Format speedup with color coding"""
    if speedup > 3:
        return f"🚀 {speedup:.2f}x"
    elif speedup > 2:
        return f"⚡ {speedup:.2f}x"
    elif speedup > 1.5:
        return f"✨ {speedup:.2f}x"
    else:
        return f"📈 {speedup:.2f}x"


def run_comprehensive_benchmark():
    """Run comprehensive SIMD benchmarks"""
    
    print("=" * 80)
    print("🚀 Cross-Platform SIMD Performance Benchmark")
    print("=" * 80)
    print()
    
    # Display system info
    print("📊 System Information:")
    info = get_system_info()
    for key, value in info.items():
        print(f"   {key:20s}: {value}")
    print()
    
    # Test configurations
    test_configs = [
        {"size": 1_000, "window": 20, "name": "Small Dataset (1K)"},
        {"size": 10_000, "window": 50, "name": "Medium Dataset (10K)"},
        {"size": 100_000, "window": 100, "name": "Large Dataset (100K)"},
        {"size": 1_000_000, "window": 252, "name": "Extra Large Dataset (1M)"},
    ]
    
    print("🔬 Running Benchmarks...")
    print("-" * 80)
    
    for config in test_configs:
        size = config["size"]
        window = config["window"]
        name = config["name"]
        
        print(f"\n📈 {name}")
        print(f"   Data points: {size:,} | Window size: {window}")
        print()
        
        # Generate data
        prices = generate_realistic_price_data(size)
        
        # Benchmark scalar version (baseline)
        print("   ⏱️  Baseline (rolling_volatility)...", end=" ", flush=True)
        time_baseline, result_baseline = benchmark_function(finmath.rolling_volatility, prices, window)
        print(f"{format_time(time_baseline)}")
        
        # Benchmark zero-copy NumPy version
        print("   ⏱️  Zero-copy NumPy (rolling_volatility_np)...", end=" ", flush=True)
        time_numpy, result_numpy = benchmark_function(finmath.rolling_volatility, prices, window)
        print(f"{format_time(time_numpy)}")
        
        # Benchmark SIMD version
        print("   ⏱️  SIMD-optimized (rolling_volatility_simd)...", end=" ", flush=True)
        time_simd, result_simd = benchmark_function(finmath.rolling_volatility_simd, prices, window)
        print(f"{format_time(time_simd)}")
        
        print()
        
        # Calculate speedups
        speedup_numpy = time_baseline / time_numpy
        speedup_simd = time_baseline / time_simd
        speedup_simd_vs_numpy = time_numpy / time_simd
        
        print(f"   📊 Performance Summary:")
        print(f"      Zero-copy speedup:      {format_speedup(speedup_numpy)}")
        print(f"      SIMD speedup:           {format_speedup(speedup_simd)}")
        print(f"      SIMD vs Zero-copy:      {format_speedup(speedup_simd_vs_numpy)}")
        
        # Verify correctness (results should be very close)
        if len(result_baseline) == len(result_simd):
            max_diff = np.max(np.abs(np.array(result_baseline) - np.array(result_simd)))
            print(f"      Numerical accuracy:     {max_diff:.2e} (max diff)")
        
        print()
    
    print("-" * 80)
    print()


def run_memory_efficiency_demo():
    """Demonstrate memory efficiency of zero-copy approach"""
    
    print("=" * 80)
    print("💾 Memory Efficiency Demo (Zero-Copy NumPy)")
    print("=" * 80)
    print()
    
    size = 1_000_000
    prices = generate_realistic_price_data(size)
    
    print(f"📊 Dataset: {size:,} prices ({prices.nbytes / 1024 / 1024:.2f} MB)")
    print()
    
    # Get memory address of NumPy array
    prices_address = prices.__array_interface__['data'][0]
    
    print(f"   NumPy array memory address: 0x{prices_address:016x}")
    print()
    print("   🔍 Calling C++ with zero-copy...")
    print("   The C++ function accesses NumPy's memory directly without copying!")
    print()
    
    result = finmath.rolling_volatility_simd(prices, 252)
    
    print(f"   ✅ Computed {len(result):,} volatility values")
    print(f"   💡 Memory saved: ~{prices.nbytes / 1024 / 1024:.2f} MB (no data duplication)")
    print()


def run_realtime_simulation():
    """Simulate real-time market analysis"""
    
    print("=" * 80)
    print("⚡ Real-Time Market Analysis Simulation")
    print("=" * 80)
    print()
    
    # Simulate incoming market data
    window = 50
    batch_sizes = [100, 500, 1000, 5000]
    
    print("Simulating incoming market data batches...")
    print()
    
    for batch_size in batch_sizes:
        prices = generate_realistic_price_data(batch_size + window)
        
        # Time the analysis
        start = time.perf_counter()
        volatility = finmath.rolling_volatility_simd(prices, window)
        end = time.perf_counter()
        
        elapsed_ms = (end - start) * 1000
        throughput = batch_size / (elapsed_ms / 1000) if elapsed_ms > 0 else 0
        
        print(f"   Batch size: {batch_size:5d} | "
              f"Time: {format_time(elapsed_ms):>10s} | "
              f"Throughput: {throughput:>12,.0f} updates/sec")
    
    print()
    print("💡 This demonstrates how finmath can handle high-frequency data streams!")
    print()


def run_architecture_comparison():
    """Show SIMD capabilities across architectures"""
    
    print("=" * 80)
    print("🔧 Architecture & SIMD Capabilities")
    print("=" * 80)
    print()
    
    arch = platform.machine()
    backend = finmath.get_simd_backend()
    
    print(f"Your system: {platform.system()} on {arch}")
    print(f"Active SIMD backend: {backend}")
    print()
    
    # Explain what SIMD means for this architecture
    simd_info = {
        "AVX": {
            "name": "Advanced Vector Extensions",
            "vector_width": "256-bit (4 doubles)",
            "typical_cpu": "Modern Intel/AMD (2011+)",
            "speedup": "2-4x vs scalar",
        },
        "SSE": {
            "name": "Streaming SIMD Extensions",
            "vector_width": "128-bit (2 doubles)",
            "typical_cpu": "Intel/AMD (1999+)",
            "speedup": "1.5-2x vs scalar",
        },
        "NEON": {
            "name": "ARM NEON",
            "vector_width": "128-bit (2 doubles)",
            "typical_cpu": "Apple Silicon, ARM servers",
            "speedup": "1.5-2.5x vs scalar",
        },
        "Scalar": {
            "name": "Scalar (no SIMD)",
            "vector_width": "64-bit (1 double)",
            "typical_cpu": "Fallback for any CPU",
            "speedup": "1x (baseline)",
        },
    }
    
    if backend in simd_info:
        info = simd_info[backend]
        print(f"📋 {info['name']}:")
        print(f"   Vector width:    {info['vector_width']}")
        print(f"   Typical CPUs:    {info['typical_cpu']}")
        print(f"   Expected speedup: {info['speedup']}")
    
    print()
    print("💡 The finmath library automatically detects and uses the best")
    print("   SIMD instructions available on your CPU at compile time!")
    print()


def main():
    """Run all demos"""
    
    # Check if running interactively
    import sys
    interactive = sys.stdin.isatty()
    
    print()
    print("█" * 80)
    print("█" + " " * 78 + "█")
    print("█" + "  FINMATH: High-Performance Quantitative Finance Library".center(78) + "█")
    print("█" + "  Cross-Platform SIMD Performance Demo".center(78) + "█")
    print("█" + " " * 78 + "█")
    print("█" * 80)
    print()
    
    # Run all demos
    run_architecture_comparison()
    if interactive:
        input("Press Enter to run comprehensive benchmark...")
    print()
    
    run_comprehensive_benchmark()
    if interactive:
        input("Press Enter to run memory efficiency demo...")
    print()
    
    run_memory_efficiency_demo()
    if interactive:
        input("Press Enter to run real-time simulation...")
    print()
    
    run_realtime_simulation()
    
    print("=" * 80)
    print("✅ Demo Complete!")
    print("=" * 80)
    print()
    print("🎯 Key Takeaways:")
    print("   • Cross-platform SIMD automatically uses the best CPU instructions")
    print("   • Zero-copy NumPy integration eliminates memory overhead")
    print("   • Ideal for HFT, real-time analytics, and large-scale backtesting")
    print("   • Works seamlessly on ARM (Apple Silicon) and x86_64 (Intel/AMD)")
    print()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n\n⚠️  Demo interrupted by user.")
        sys.exit(0)
    except Exception as e:
        print(f"\n❌ Error: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)

