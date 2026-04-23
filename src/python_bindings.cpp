#include <pybind11/pybind11.h>
#include <pybind11/stl.h>   // Automatic conversion between Python lists and std::vector
#include <pybind11/numpy.h> // Add numpy include

#include "finmath/InterestAndAnnuities/compound_interest.h"
#include "finmath/InterestAndAnnuities/discount_factor.h"
#include "finmath/InterestAndAnnuities/present_future_value.h"
#include "finmath/InterestAndAnnuities/annuity.h"
#include "finmath/InterestAndAnnuities/cash_flow.h"
#include "finmath/OptionPricing/black_scholes.h"
#include "finmath/OptionPricing/binomial_tree.h"
#include "finmath/TimeSeries/rolling_volatility.h"
#include "finmath/TimeSeries/rolling_volatility_simd.h"
#include "finmath/TimeSeries/simple_moving_average.h"
#include "finmath/TimeSeries/simple_moving_average_simd.h"
#include "finmath/TimeSeries/rsi.h"
#include "finmath/TimeSeries/rsi_simd.h"
#include "finmath/TimeSeries/ema.h"
#include "finmath/TimeSeries/ema_simd.h"
#include "finmath/TimeSeries/returns.h"
#include "finmath/TimeSeries/rolling_zscore.h"
#include "finmath/Helper/simd_helper.h"
#include "finmath/GraphAlgos/bellman_arbitrage.h"
#include "finmath/FixedIncome/bond_pricing.h"

namespace py = pybind11;

// Forward declarations for NumPy-compatible functions
std::vector<double> rolling_volatility_np(py::array_t<double> prices_arr, size_t window_size);
std::vector<double> simple_moving_average_np(py::array_t<double> data_arr, size_t window_size);
std::vector<double> compute_smoothed_rsi_np(py::array_t<double> prices_arr, size_t window_size);
std::vector<double> log_returns_np(py::array_t<double> prices_arr);
std::vector<double> pct_returns_np(py::array_t<double> prices_arr);
std::vector<double> rolling_mean_np(py::array_t<double> data_arr, size_t window_size);
std::vector<double> rolling_zscore_np(py::array_t<double> data_arr, size_t window_size);

PYBIND11_MODULE(finmath, m)
{
      m.doc() = "Financial Math Library";

      // Expose the OptionType enum class
      py::enum_<OptionType>(m, "OptionType")
          .value("CALL", OptionType::CALL)
          .value("PUT", OptionType::PUT)
          .export_values();

      // Bind compound interest function
      m.def("compound_interest", &compound_interest, "Calculate compound interest",
            py::arg("principal"), py::arg("rate"), py::arg("time"), py::arg("frequency"));

      // Bind Black-Scholes function
      m.def("black_scholes", &black_scholes, "Black Scholes Option Pricing",
            py::arg("type"), py::arg("strike"), py::arg("price"), py::arg("time"), py::arg("rate"), py::arg("volatility"));

      // Bind binomial option pricing function
      m.def("binomial_option_pricing", &binomial_option_pricing, "Binomial Option Pricing",
            py::arg("type"), py::arg("S0"), py::arg("K"), py::arg("T"), py::arg("r"), py::arg("sigma"), py::arg("N"));

      // Bind rolling volatility
      m.def("rolling_volatility", &rolling_volatility, "Rolling Volatility (List input)",
            py::arg("prices"), py::arg("window_size"));
      m.def("rolling_volatility", &rolling_volatility_np, "Rolling Volatility (NumPy/Pandas input)",
            py::arg("prices"), py::arg("window_size"));
      m.def("rolling_volatility_simd", &rolling_volatility_simd, "Rolling Volatility (SIMD-optimized, zero-copy NumPy)",
            py::arg("prices"), py::arg("window_size"));

      // Bind simple moving average
      m.def("simple_moving_average", &simple_moving_average, "Simple Moving Average (List input)",
            py::arg("prices"), py::arg("window_size"));
      m.def("simple_moving_average", &simple_moving_average_np, "Simple Moving Average (NumPy/Pandas input)",
            py::arg("prices"), py::arg("window_size"));
      m.def("simple_moving_average_simd", &simple_moving_average_simd, "Simple Moving Average (SIMD-optimized, zero-copy NumPy)",
            py::arg("prices"), py::arg("window_size"));

      // Bind RSI
      m.def("smoothed_rsi", &compute_smoothed_rsi, "Relative Strength Index(RSI) (List input)",
            py::arg("prices"), py::arg("window_size"));
      m.def("smoothed_rsi", &compute_smoothed_rsi_np, "Relative Strength Index(RSI) (NumPy/Pandas input)",
            py::arg("prices"), py::arg("window_size"));
      m.def("smoothed_rsi_simd", &compute_smoothed_rsi_simd, "Relative Strength Index (SIMD-optimized, zero-copy NumPy)",
            py::arg("prices"), py::arg("window_size"));

      // Bind EMA (window)
      m.def("ema_window", &compute_ema, "Exponential Moving Average - Window (List input)",
            py::arg("prices"), py::arg("window_size"));

      // Bind EMA (smoothing factor)
      m.def("ema_smoothing", &compute_ema_with_smoothing, "Exponential Moving Average - Smoothing Factor (List input)",
            py::arg("prices"), py::arg("smoothing_factor"));

      // Bind SIMD-optimized EMA
      m.def("ema_window_simd", &compute_ema_simd, "Exponential Moving Average - Window (SIMD-optimized, zero-copy NumPy)",
            py::arg("prices"), py::arg("window_size"));
      m.def("ema_smoothing_simd", &compute_ema_with_smoothing_simd, "Exponential Moving Average - Smoothing Factor (SIMD-optimized, zero-copy NumPy)",
            py::arg("prices"), py::arg("smoothing_factor"));

      // Returns + z-score utilities
      m.def("log_returns", &log_returns, "Log returns (List input)", py::arg("prices"));
      m.def("log_returns", &log_returns_np, "Log returns (NumPy/Pandas input)", py::arg("prices"));
      m.def("pct_returns", &pct_returns, "Percent returns (List input)", py::arg("prices"));
      m.def("pct_returns", &pct_returns_np, "Percent returns (NumPy/Pandas input)", py::arg("prices"));
      m.def("rolling_mean", &rolling_mean, "Rolling mean (List input)", py::arg("window_size"), py::arg("data"));
      m.def("rolling_mean", &rolling_mean_np, "Rolling mean (NumPy/Pandas input)", py::arg("data"), py::arg("window_size"));
      m.def("rolling_zscore", &rolling_zscore, "Rolling z-score (List input)", py::arg("window_size"), py::arg("data"));
      m.def("rolling_zscore", &rolling_zscore_np, "Rolling z-score (NumPy/Pandas input)", py::arg("data"), py::arg("window_size"));

      // Utility function to get SIMD backend
      m.def("get_simd_backend", &finmath::simd::get_simd_backend, "Get the active SIMD backend (AVX, SSE, NEON, or Scalar)");

      // Bellman-based arbitrage detection (from main)
      m.def("detect_arbitrage", &detectArbitrageBellman<std::string>,
            "Detect arbitrage opportunities in a currency graph",
            py::arg("graph"));

      // discount factors
      // Discount factors
      m.def("discount_factor", &discount_factor,
            "Discount factor (discrete compounding)",
            py::arg("rate"), py::arg("time"));

      m.def("discount_factor_continuous", &discount_factor_continuous,
          "Discount factor (continuous compounding)",
          py::arg("rate"), py::arg("time"));

      m.def("future_value_factor", &future_value_factor,
          "Future value factor",
          py::arg("rate"), py::arg("time"));

      // Present/Future value
      m.def("present_value", &present_value,
           "Present value (discrete compounding)",
           py::arg("future_value"), py::arg("rate"), py::arg("time"));

      m.def("future_value", &future_value,
           "Future value (discrete compounding)",
           py::arg("present_value"), py::arg("rate"), py::arg("time"));

      m.def("present_value_continuous", &present_value_continuous,
              "Present value (continuous compounding)",
              py::arg("future_value"), py::arg("rate"), py::arg("time"));

      m.def("future_value_continuous", &future_value_continuous,
              "Future value (continuous compounding)",
              py::arg("present_value"), py::arg("rate"), py::arg("time"));

      // Annuity functions
      m.def("annuity_present_value", &annuity_present_value,
            "Present value of ordinary annuity (payments at END of period)",
            py::arg("payment"), py::arg("rate"), py::arg("periods"));

      m.def("annuity_future_value", &annuity_future_value,
            "Future value of ordinary annuity",
            py::arg("payment"), py::arg("rate"), py::arg("periods"));

      m.def("annuity_due_present_value", &annuity_due_present_value,
            "Present value of annuity due (payments at BEGINNING of period)",
            py::arg("payment"), py::arg("rate"), py::arg("periods"));

      m.def("annuity_due_future_value", &annuity_due_future_value,
            "Future value of annuity due",
            py::arg("payment"), py::arg("rate"), py::arg("periods"));

      // Cash flow functions
      m.def("net_present_value", &net_present_value,
            "Net present value of a series of cash flows",
            py::arg("cash_flows"), py::arg("rate"), py::arg("initial_investment") = 0.0);

      m.def("internal_rate_of_return", &internal_rate_of_return,
            "Internal rate of return (Newton-Raphson)",
            py::arg("cash_flows"),
            py::arg("initial_guess")   = 0.1,
            py::arg("max_iterations")  = 100,
            py::arg("tolerance")       = 1e-6);

      m.def("payback_period", &payback_period,
            "Number of periods until cumulative cash flows recover the initial investment (-1 if never)",
            py::arg("cash_flows"), py::arg("initial_investment"));

      // Bond pricing functions
      m.def("bond_price", &bond_price,
            "Theoretical price of a coupon bond",
            py::arg("face_value"), py::arg("coupon_rate"), py::arg("yield_to_maturity"),
            py::arg("periods"), py::arg("time_to_maturity"));

      m.def("bond_yield", &bond_yield,
            "Yield to maturity given a bond's market price (Newton-Raphson)",
            py::arg("face_value"), py::arg("coupon_rate"), py::arg("price"),
            py::arg("periods"), py::arg("time_to_maturity"));

      m.def("bond_duration", &bond_duration,
            "Macaulay duration of a bond in years",
            py::arg("face_value"), py::arg("coupon_rate"), py::arg("yield_to_maturity"),
            py::arg("periods"), py::arg("time_to_maturity"));

}
