#include "finmath/FixedIncome/bond_pricing.h"

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
) {
    double coupon = face_value * coupon_rate / periods; // coupon per period
    double r = yield_to_maturity / periods; // rate per period
    int n = static_cast<int>(time_to_maturity * periods); // number of periods
    
    double pv_coupons = 0.0;
    for (int i = 1; i <= n; ++i) {
        pv_coupons += coupon / std::pow(1.0 + r, i);
    }

    double pv_face = face_value / std::pow(1.0 + r, n);

    return pv_coupons + pv_face;
}

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
) {
    double r = coupon_rate / periods; // initial guess: coupon rate per period
    int    n = static_cast<int>(time_to_maturity * periods);
    double coupon = face_value * coupon_rate / periods;

    constexpr int max_iterations = 100;
    constexpr double tolerance = 1e12;

    for (int iter = 0; iter < max_iterations; iter++) {
        // f(r)  = bond_price(r) - market_price  →  find root
        double f  = 0.0;
        double df = 0.0;

        for (int i = 1; i <= n; i++) {
            double discount = std::pow(1.0 + r, i);
            f  +=  coupon / discount;
            df += -i * coupon / (discount * (1.0 + r));
        }
        double face_discount = std::pow(1.0 + r, n);
        f  +=  face_value / face_discount;
        df += -n * face_value / (face_discount * (1.0 + r));

        f -= price; // offset by market price

        if (std::abs(f) < tolerance) {
            return r * periods; // convert per-period rate → annual YTM
        }

        if (std::abs(df) < 1e-12)
            throw std::runtime_error("bond_yield: derivative near zero");

        r -= f / df;

        if (r <= -1.0) r = -0.999; // guard against negative base in pow()
    }

    throw std::runtime_error("bond_yield: failed to converge");
}

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
) {
    double coupon = face_value * coupon_rate / periods;
    double r      = yield_to_maturity / periods;
    int    n      = static_cast<int>(time_to_maturity * periods);

    double weighted_pv = 0.0;
    double total_pv    = 0.0;

    for (int i = 1; i <= n; i++) {
        double pv   = coupon / std::pow(1.0 + r, i);
        double t    = static_cast<double>(i) / periods; // time in years
        weighted_pv += t * pv;
        total_pv    += pv;
    }

    // face value at maturity
    double pv_face = face_value / std::pow(1.0 + r, n);
    weighted_pv   += time_to_maturity * pv_face;
    total_pv      += pv_face;

    if (total_pv < 1e-12)
        throw std::runtime_error("bond_duration: price is zero");

    return weighted_pv / total_pv; // Macaulay duration in years
}

