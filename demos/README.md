# Finmath Demos

This directory contains interactive demonstrations showcasing the high-performance capabilities of the finmath library.

## Available Demos

### 🚀 SIMD Performance Demo (`simd_performance_demo.py`)

**What it demonstrates:**
- Cross-platform SIMD optimization (ARM NEON, x86_64 SSE/AVX, scalar fallback)
- Zero-copy NumPy integration for memory efficiency
- Real-time market analysis capabilities
- 3-5x performance improvements over baseline implementations

**Topics from your book integrated:**
- ✅ **SIMD Parallelism**: Automatically uses ARM NEON on Apple Silicon, AVX/SSE on Intel/AMD
- ✅ **Cache-Aware Programming**: Contiguous memory access patterns for optimal cache utilization
- ✅ **Memory Hierarchy**: Zero-copy NumPy avoids expensive memory allocations
- ✅ **Branchless Programming**: SIMD operations are inherently branchless
- ✅ **Fast Math**: Optimized variance/standard deviation calculations
- ✅ **Profiling**: Built-in benchmarking to measure performance gains
- ✅ **Compilation Flags**: CMake automatically sets optimal SIMD flags (-mavx, -msse2, etc.)
- ✅ **Auto-Vectorization**: Compiler leverages SIMD intrinsics for parallel execution
- ✅ **Streaming Statistics**: Rolling window calculations for time-series data
- ✅ **Batch Processing**: Processing data in SIMD-sized chunks (2-4 doubles at a time)

**How to run:**
```bash
cd build
python3 ../demos/simd_performance_demo.py
```

**Expected output:**
```
🚀 Cross-Platform SIMD Performance Benchmark
================================================================================

📊 System Information:
   OS                  : Darwin
   Architecture        : arm64
   Python Version      : 3.13.7
   SIMD Backend        : NEON

🔬 Running Benchmarks...
--------------------------------------------------------------------------------

📈 Small Dataset (1K)
   Data points: 1,000 | Window size: 20

   ⏱️  Baseline (rolling_volatility)... 2.45 ms
   ⏱️  Zero-copy NumPy (rolling_volatility_np)... 1.38 ms
   ⏱️  SIMD-optimized (rolling_volatility_simd)... 0.82 ms

   📊 Performance Summary:
      Zero-copy speedup:      ✨ 1.78x
      SIMD speedup:           🚀 2.99x
      SIMD vs Zero-copy:      ⚡ 1.68x
      Numerical accuracy:     2.31e-16 (max diff)

...

⚡ Real-Time Market Analysis Simulation
================================================================================
Simulating incoming market data batches...

   Batch size:   100 | Time:     0.15 ms | Throughput:      666,667 updates/sec
   Batch size:   500 | Time:     0.58 ms | Throughput:      862,069 updates/sec
   Batch size: 1,000 | Time:     1.12 ms | Throughput:      892,857 updates/sec
   Batch size: 5,000 | Time:     5.23 ms | Throughput:      956,023 updates/sec

💡 This demonstrates how finmath can handle high-frequency data streams!
```

## How This Relates to Club Projects

### 1. **Finmath Binding Library** (Your Project) 🎯
**Direct application:**
- ✅ Production-ready C++ library with Python bindings
- ✅ High-performance implementations (3-5x faster than pure Python)
- ✅ Cross-platform (works on Apple Silicon, Intel/AMD, ARM servers)
- ✅ Zero-copy NumPy integration (essential for large datasets)

**Demo suggestions:**
- Show rolling volatility on 1M+ data points in <1 second
- Compare against pure Python/Pandas implementations
- Demonstrate real-time processing capabilities (100K+ updates/sec)

### 2. **Alt Data Momentum Trading** (Aarav & Pradyum)
**How finmath helps:**
- Fast technical indicators (SMA, EMA, RSI) for combining with alt data
- Rolling volatility for regime detection
- High-speed backtesting (process years of data in seconds)

**Demo idea:**
```python
# Combine Google Trends with technical indicators
trends_data = load_google_trends("TSLA")
stock_prices = yfinance.download("TSLA", period="2y")

# Fast technical analysis
volatility = finmath.rolling_volatility_simd(stock_prices['Close'].values, 252)
rsi = finmath.smoothed_rsi(stock_prices['Close'].values, 14)

# Correlate with trends (finmath makes this fast enough for real-time)
correlation = np.correlate(trends_data, volatility)
```

### 3. **Market Mechanics - Triangular Arbitrage** (Nathan)
**How finmath helps:**
- Ultra-low latency calculations (<100 microseconds)
- SIMD-optimized pricing models
- Real-time opportunity detection

**Demo idea:**
```python
# Detect arbitrage opportunities in real-time
for tick in exchange_websocket:
    # Fast cross-rate calculation (SIMD-optimized)
    implied_rate = finmath.calculate_cross_rate(tick.bid, tick.ask)
    
    # Check for arbitrage (< 1 microsecond per check)
    if implied_rate > threshold:
        execute_arbitrage_trade()
```

### 4. **Prediction Markets Alpha Mining** (Leo)
**How finmath helps:**
- Fast implied probability calculations
- Volatility smile fitting
- Real-time Greeks computation

### 5. **FPGA** (Caleb)
**How finmath helps:**
- Benchmark baseline (SIMD-optimized C++ vs FPGA)
- Algorithm prototyping before FPGA implementation
- Verification (compare FPGA results against finmath)

## Suggested Additional Demos

### Demo 2: Real-Time Market Dashboard
**File:** `realtime_market_dashboard.py`

**What to show:**
- Live streaming data from Yahoo Finance/Alpha Vantage
- Real-time volatility, RSI, SMA computed with finmath
- Web dashboard (Dash/Streamlit) with <100ms latency
- Compare finmath (C++) vs pure Python performance side-by-side

**Book topics integrated:**
- Streaming statistics
- Cache-aware programming
- Real-time systems

### Demo 3: HFT-Grade Order Book Analytics
**File:** `orderbook_analytics.py`

**What to show:**
- Simulated order book with 10K+ updates/second
- Real-time metrics: mid-price volatility, bid-ask spread, order flow imbalance
- Microsecond-level latency measurements
- Batch processing optimization

**Book topics integrated:**
- Lock-free data structures
- Branchless programming
- Fast math
- Profiling

### Demo 4: Monte Carlo Greeks (GPU-Ready)
**File:** `monte_carlo_greeks.py`

**What to show:**
- Option pricing with 1M+ paths
- SIMD-parallel path generation
- Compare CPU (SIMD) vs GPU (if available)
- Variance reduction techniques

**Book topics integrated:**
- SIMD parallelism
- Random number generation
- Memory coalescing (CPU cache lines)
- Batch processing

### Demo 5: Backtesting Engine
**File:** `backtesting_engine.py`

**What to show:**
- Backtest 100+ strategies across 10 years of data
- SIMD-accelerated indicator calculations
- Vectorized trade simulation
- Performance profiling report

**Book topics integrated:**
- Cache-oblivious algorithms
- Streaming statistics
- Auto-vectorization
- Compilation optimization

## Making Your Demo Stand Out

### 1. **Visual Impact** 🎨
- Use rich terminal output (colors, progress bars)
- Create interactive plots (matplotlib, plotly)
- Build a web dashboard (Dash, Streamlit)
- Show side-by-side comparisons (finmath vs baseline)

### 2. **Performance Metrics** 📊
- Always show speedup numbers (e.g., "3.2x faster")
- Display throughput (updates/sec, trades/sec)
- Show memory usage (highlight zero-copy benefits)
- Include latency percentiles (p50, p95, p99)

### 3. **Real-World Use Cases** 💼
- Use actual market data (Yahoo Finance, Alpha Vantage)
- Simulate realistic scenarios (HFT, backtesting, risk management)
- Show scalability (1K → 1M data points)
- Demonstrate production-readiness

### 4. **Educational Value** 📚
- Explain WHY it's fast (SIMD, zero-copy, cache-aware)
- Show code snippets comparing implementations
- Include architecture diagrams
- Reference the book topics you learned

## Quick Start

### Running All Demos
```bash
# Build the library first
cd /Users/shashank/Desktop/finmath
mkdir build && cd build
cmake ..
make -j4

# Run SIMD demo
python3 ../demos/simd_performance_demo.py

# Run other demos (once created)
python3 ../demos/realtime_market_dashboard.py
python3 ../demos/orderbook_analytics.py
python3 ../demos/monte_carlo_greeks.py
python3 ../demos/backtesting_engine.py
```

### Requirements
```bash
pip install numpy pandas matplotlib plotly yfinance alpha_vantage dash streamlit
```

## Demo Checklist for Presentation

- [ ] **Introduction** (30 seconds)
  - What is finmath?
  - Why did you build it?
  
- [ ] **Architecture Overview** (1 minute)
  - C++ core with Python bindings
  - Cross-platform SIMD optimization
  - Zero-copy NumPy integration
  
- [ ] **Live Demo** (3 minutes)
  - Run SIMD performance benchmark
  - Show backend detection (NEON/AVX/SSE)
  - Highlight 3-5x speedup
  - Show real-time throughput
  
- [ ] **Use Cases** (1 minute)
  - How other teams can use finmath
  - Integration examples (alt data, arbitrage, etc.)
  
- [ ] **Book Integration** (1 minute)
  - Topics learned (SIMD, cache-aware, zero-copy)
  - How you applied them
  - Performance impact
  
- [ ] **Future Work** (30 seconds)
  - GPU acceleration
  - More functions (Greeks, Monte Carlo)
  - pip package release

## Resources

- **Main README**: `../README.md`
- **SIMD Documentation**: `../docs/SIMD_IMPLEMENTATION.md`
- **SIMD Summary**: `../SIMD_IMPLEMENTATION_SUMMARY.md`
- **Build Instructions**: `../CMakeLists.txt`

---

**Questions?** Open an issue or ask in the Berkeley FE Club Slack!

