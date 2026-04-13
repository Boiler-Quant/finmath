# finmath Python API (overview)

Build the extension, then `import finmath`.

- **Black-Scholes price:** `black_scholes(type, strike, spot, time, rate, volatility, dividend_yield=0.0)`
- **Greeks:** `black_scholes_delta`, `black_scholes_gamma`, `black_scholes_vega`, `black_scholes_theta`, `black_scholes_rho`
- **Binomial:** `binomial_option_pricing(type, S0, K, T, r, sigma, N)`
- **Markov:** transition matrix `P` is row-stochastic
- **Risk:** `historical_var`, `historical_cvar`, `parametric_var` with tail `alpha` in `(0,1)`

See module docstrings and C++ headers for full lists.
