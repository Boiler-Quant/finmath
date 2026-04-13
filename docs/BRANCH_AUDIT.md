# Branch audit (merge decisions)

| Branch | Decision | Notes |
|--------|----------|--------|
| `origin/OLS` | Reimplemented | Portable Cholesky OLS without Eigen |
| `origin/13-autocorrelation-function-acf` | Reimplemented | Upstream did not compile; clean ACF |
| `feature/rolling-corr`, `feature/returns-zscore`, `feature/vwap-volume`, `feature/orderbook-features` | Merged (adapted) | Integrated with tests |
| `origin/MonteCarlo` | No code | MC implemented in-tree |
| `origin/Option-Greeks` | Obsolete | Greeks live in `black_scholes.cpp` |
