/**
 * Present value of ordinary annuity
 * Payments are made at the END of each period
 *
 * @param payment Payment amount per period
 * @param rate Interest rate per period (not annualized if periods are not annual)
 * @param periods Number of payment periods
 * @return Present value of the annuity
 *
 * Formula: PV = P * [1 - (1+r)^(-n)] / r
 *
 * Edge cases:
 * - If rate == 0: return payment * periods
 * - If periods == 0: return 0
 */
double annuity_present_value(double payment, double rate, int periods);

/**
 * Future value of ordinary annuity
 *
 * @param payment Payment amount per period
 * @param rate Interest rate per period
 * @param periods Number of payment periods
 * @return Future value of the annuity
 *
 * Formula: FV = P * [(1+r)^n - 1] / r
 */
double annuity_future_value(double payment, double rate, int periods);

/**
 * Present value of annuity due
 * Payments are made at the BEGINNING of each period
 *
 * @param payment Payment amount per period
 * @param rate Interest rate per period
 * @param periods Number of payment periods
 * @return Present value of annuity due
 *
 * Formula: PV = P * [1 - (1+r)^(-n)] / r * (1 + r)
 */
double annuity_due_present_value(double payment, double rate, int periods);

/**
 * Future value of annuity due
 *
 * @param payment Payment amount per period
 * @param rate Interest rate per period
 * @param periods Number of payment periods
 * @return Future value of annuity due
 *
 * Formula: FV = P * [(1+r)^n - 1] / r * (1 + r)
 */
double annuity_due_future_value(double payment, double rate, int periods);
