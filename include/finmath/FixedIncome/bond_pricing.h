/**
 * Bond price (coupon bond)
 * Calculates the theoretical price of a bond
 *
 * @param face_value Face value (par value) of the bond
 * @param coupon_rate Annual coupon rate (e.g., 0.05 for 5%)
 * @param yield_to_maturity Yield to maturity (discount rate)
 * @param periods Number of coupon payment periods per year
 * @param time_to_maturity Time to maturity in years
 * @return Bond price
 *
 * Formula: Price = sum(Coupon / (1+r)^i) + Face / (1+r)^n
 *
 * Example:
 *   10-year bond, $1000 face, 5% coupon, paid semi-annually, 4% YTM
 *   Coupon per period = $1000 * 0.05 / 2 = $25
 *   Number of periods = 10 * 2 = 20
 *   Rate per period = 0.04 / 2 = 0.02
 */
double bond_price(
    double face_value,
    double coupon_rate,
    double yield_to_maturity,
    int periods,
    double time_to_maturity
);

/**
 * Bond yield (simplified, iterative)
 * Finds YTM given bond price
 * Uses Newton-Raphson method
 *
 * @param face_value Face value of the bond
 * @param coupon_rate Annual coupon rate
 * @param price Current market price of the bond
 * @param periods Number of coupon payment periods per year
 * @param time_to_maturity Time to maturity in years
 * @return Yield to maturity
 */
double bond_yield(
    double face_value,
    double coupon_rate,
    double price,
    int periods,
    double time_to_maturity
);

/**
 * Duration (Macaulay)
 * Measures interest rate sensitivity
 *
 * @param face_value Face value of the bond
 * @param coupon_rate Annual coupon rate
 * @param yield_to_maturity Yield to maturity
 * @param periods Number of coupon payment periods per year
 * @param time_to_maturity Time to maturity in years
 * @return Macaulay duration in years
 *
 * Formula: Duration = sum(t * PV(CF_t)) / Price
 */
double bond_duration(
    double face_value,
    double coupon_rate,
    double yield_to_maturity,
    int periods,
    double time_to_maturity
);
