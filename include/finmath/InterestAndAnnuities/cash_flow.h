#include <vector>
/**
 * Net Present Value
 * Calculates the present value of all cash flows
 *
 * @param cash_flows Vector of cash flows (negative = outflow, positive = inflow)
 * @param rate Discount rate (e.g., 0.10 for 10%)
 * @param initial_investment Optional initial investment (default: 0.0)
 * @return Net present value
 *
 * Formula: NPV = sum(CF_i / (1+r)^i) - Initial Investment
 *
 * Example:
 *   cash_flows = [-1000, 100, 200, 300, 400]
 *   rate = 0.10
 *   NPV = -1000 + 100/(1.1) + 200/(1.1)^2 + 300/(1.1)^3 + 400/(1.1)^4
 */
double net_present_value(
    const std::vector<double>& cash_flows,
    double rate,
    double initial_investment = 0.0
);

/**
 * Internal Rate of Return
 * Finds the discount rate that makes NPV = 0
 * Uses Newton-Raphson iterative method
 *
 * @param cash_flows Vector of cash flows
 * @param initial_guess Starting guess for IRR (default: 0.1 = 10%)
 * @param max_iterations Maximum iterations for convergence (default: 100)
 * @param tolerance Convergence tolerance (default: 1e-6)
 * @return Internal rate of return
 *
 * Algorithm:
 * 1. Start with initial guess
 * 2. Calculate NPV and dNPV/dr at current guess
 * 3. Update: r_new = r_old - NPV / dNPV/dr
 * 4. Repeat until |NPV| < tolerance
 *
 * @throws std::runtime_error if convergence fails
 */
double internal_rate_of_return(
    const std::vector<double>& cash_flows,
    double initial_guess = 0.1,
    int max_iterations = 100,
    double tolerance = 1e-6
);

/**
 * Payback period
 * Returns the number of periods until cumulative cash flows exceed initial investment
 *
 * @param cash_flows Vector of cash flows (first element is typically initial investment)
 * @param initial_investment Initial investment amount
 * @return Number of periods until payback (returns -1 if never pays back)
 *
 * Example:
 *   cash_flows = [100, 200, 300, 400]
 *   initial_investment = 500
 *   Cumulative: 100, 300, 600 (payback at period 3)
 */
int payback_period(
    const std::vector<double>& cash_flows,
    double initial_investment
);
