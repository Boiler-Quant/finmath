/**
 * Present value with discrete compounding
 * Calculates what a future amount is worth today
 *
 * @param future_value Amount to be received in the future
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years until the future value is received
 * @return Present value of the future amount
 *
 * Formula: PV = FV / (1 + r)^t
 */
double present_value(double future_value, double rate, double time);

/**
 * Future value with discrete compounding
 * Calculates what a current amount will be worth in the future
 *
 * @param present_value Current amount
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Future value of the present amount
 *
 * Formula: FV = PV * (1 + r)^t
 */
double future_value(double present_value, double rate, double time);

/**
 * Present value with continuous compounding
 * Uses exponential compounding
 *
 * @param future_value Amount to be received in the future
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Present value with continuous compounding
 *
 * Formula: PV = FV * e^(-r*t)
 */
double present_value_continuous(double future_value, double rate, double time);

/**
 * Future value with continuous compounding
 *
 * @param present_value Current amount
 * @param rate Annual interest rate (e.g., 0.05 for 5%)
 * @param time Time in years
 * @return Future value with continuous compounding
 *
 * Formula: FV = PV * e^(r*t)
 */
double future_value_continuous(double present_value, double rate, double time);
