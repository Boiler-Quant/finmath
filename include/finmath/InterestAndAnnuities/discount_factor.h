/**
 * Discrete compounding discount factor
 * Converts a future value to present value using discrete compounding
 *
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Discount factor (0 < DF <= 1)
 *
 * Formula: DF = 1 / (1 + r)^t
 *
 * @throws std::invalid_argument if rate < 0 or time < 0
 */
double discount_factor(double rate, double time);

/**
 * Continuous compounding discount factor
 * Uses exponential compounding for continuous interest
 *
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Discount factor (0 < DF <= 1)
 *
 * Formula: DF = e^(-r*t)
 *
 * @throws std::invalid_argument if rate < 0 or time < 0
 */
double discount_factor_continuous(double rate, double time);

/**
 * Future value factor
 * Converts a present value to future value
 *
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Future value factor (FVF >= 1)
 *
 * Formula: FVF = (1 + r)^t
 *
 * @throws std::invalid_argument if rate < 0 or time < 0
 */
double future_value_factor(double rate, double time);
