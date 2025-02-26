import finmath

# Test compound interest
ci = finmath.compound_interest(1000, 0.05, 10, 4)
print("Compound Interest:", ci)

# Test Black-Scholes
bs_price = finmath.black_scholes(
    finmath.OptionType.CALL, 100, 110, 1, 0.05, 0.2
)
print("Black-Scholes Option Price:", bs_price)

# Test other functions
sma = finmath.simple_moving_average([1, 2, 3, 4, 5], window_size=3)
print("Simple Moving Average:", sma)
